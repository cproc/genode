/*
 * \brief  Less minimalistic nitpicker pointer
 * \author Norman Feske
 * \author Christian Helmuth
 * \date   2014-07-02
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/sleep.h>
#include <os/attached_dataspace.h>
#include <os/attached_ram_dataspace.h>
#include <os/attached_rom_dataspace.h>
#include <os/surface.h>
#include <os/pixel_alpha8.h>
#include <os/pixel_rgb565.h>
#include <os/pixel_rgb888.h>
#include <os/texture_rgb888.h>
#include <os/texture.h>
#include <util/xml_node.h>
#include <nitpicker_session/connection.h>
#include <vbox_pointer/dither_painter.h>
#include <vbox_pointer/shape_report.h>

/* local includes */
#include "big_mouse.h"

template <typename PT>
void convert_default_cursor_data_to_pixels(PT *pixel, Nitpicker::Area size)
{
	unsigned char *alpha = (unsigned char *)(pixel + size.count());

	for (unsigned y = 0; y < size.h(); y++) {
		for (unsigned x = 0; x < size.w(); x++) {

			/* the source is known to be in RGB565 format */
			Genode::Pixel_rgb565 src =
				*(Genode::Pixel_rgb565 *)(&big_mouse.pixels[y][x]);

			unsigned const i = y*size.w() + x;
			pixel[i] = PT(src.r(), src.g(), src.b());
			alpha[i] = src.r() ? 255 : 0;
		}
	}
}

template <typename PT>
void convert_vbox_cursor_data_to_pixels(PT *pixel, unsigned char *shape,
                                        Nitpicker::Area size)
{
	Genode::Attached_ram_dataspace texture_pixel_ds { Genode::env()->ram_session(),
	                                                  size.count() *
	                                                  sizeof(Genode::Pixel_rgb888) };

	Genode::Attached_ram_dataspace texture_alpha_ds { Genode::env()->ram_session(),
	                                                  size.count() };

	Genode::Texture<Genode::Pixel_rgb888>
		texture(texture_pixel_ds.local_addr<Genode::Pixel_rgb888>(),
		        texture_alpha_ds.local_addr<unsigned char>(),
		        size);

	for (unsigned int y = 0; y < size.h(); y++)
		texture.rgba(shape, size.w(), y);

	Genode::Pixel_alpha8 *alpha =
		reinterpret_cast<Genode::Pixel_alpha8*>(pixel + size.count());

	Genode::Surface<PT>	pixel_surface(pixel, size);
	Genode::Surface<Genode::Pixel_alpha8> alpha_surface(alpha, size);

	Dither_painter::paint(pixel_surface, texture);
	Dither_painter::paint(alpha_surface, texture);
}

//struct Log { Log(char const *msg) { PINF("Log: %s", msg); } };


typedef Genode::String<64> Domain_name;

class Domain : public Genode::List<Domain>::Element
{
	public:

		struct Name_too_long { };

	private:

		Domain_name _name;

	public:

		Domain(char const *name) : _name(name)
		{
			if (Genode::strlen(name) + 1 > _name.capacity())
				throw Name_too_long();
		}

		Domain_name const & name() { return _name; }
};


struct Domain_list : private Genode::List<Domain>
{
	void add(char const *name)
	{
		Domain *d = new (Genode::env()->heap()) Domain(name);
		insert(d);
	}

	bool contains(Domain_name const &name)
	{
		for (Domain *d = first(); d; d = d->next())
			if (d->name() == name)
				return true;

		return false;
	}
};


struct Main
{
	Genode::Attached_rom_dataspace hover_ds { "hover" };
	Genode::Attached_rom_dataspace xray_ds  { "xray" };
	Genode::Attached_rom_dataspace shape_ds { "shape" };

	Genode::Signal_receiver sig_rec;

	void handle_hover(unsigned num = 0);
	void handle_xray(unsigned num = 0);
	void handle_shape(unsigned num = 0);

	Genode::Signal_dispatcher<Main> hover_signal_dispatcher {
		sig_rec, *this, &Main::handle_hover };
	Genode::Signal_dispatcher<Main> xray_signal_dispatcher {
		sig_rec, *this, &Main::handle_xray };
	Genode::Signal_dispatcher<Main> shape_signal_dispatcher {
		sig_rec, *this, &Main::handle_shape };

	Nitpicker::Connection nitpicker;

	Nitpicker::Session::View_handle view = nitpicker.create_view();

	Domain_list hidden_domains;
	Domain_name current_domain;

	bool xray                       = false;
	bool vbox_pointer_shown         = false;
	bool vbox_pointer_shape_changed = false;

	void show_default_pointer()
	{
		if (vbox_pointer_shown) {

			vbox_pointer_shown = false;

			Nitpicker::Area const cursor_size { big_mouse.w, big_mouse.h };

			Framebuffer::Mode const mode { (int)cursor_size.w(), (int)cursor_size.h(),
	                               	   	   Framebuffer::Mode::RGB565 };

			nitpicker.buffer(mode, true /* use alpha */);

			Genode::Attached_dataspace ds { nitpicker.framebuffer()->dataspace() };

			convert_default_cursor_data_to_pixels(ds.local_addr<Genode::Pixel_rgb565>(),
			                                      cursor_size);
			nitpicker.framebuffer()->refresh(0, 0, cursor_size.w(), cursor_size.h());

			Nitpicker::Rect geometry(Nitpicker::Point(0, 0), cursor_size);
			nitpicker.enqueue<Nitpicker::Session::Command::Geometry>(view, geometry);
			nitpicker.execute();
		}
	}

	void show_vbox_pointer()
	{
		if (!vbox_pointer_shown || vbox_pointer_shape_changed) {

			vbox_pointer_shown = true;
			vbox_pointer_shape_changed = false;

			Vbox_pointer::Shape_report *shape_report =
				shape_ds.local_addr<Vbox_pointer::Shape_report>();

			PLOG("w = %u, h = %u", shape_report->width, shape_report->height);

			if (shape_report->visible) {

				Nitpicker::Area const cursor_size { shape_report->width,
				                                    shape_report->height };

				Framebuffer::Mode const mode { (int)cursor_size.w(),
				                               (int)cursor_size.h(),
	                               	   	   	   Framebuffer::Mode::RGB565 };

				nitpicker.buffer(mode, true /* use alpha */);

				Genode::Attached_dataspace ds { nitpicker.framebuffer()->dataspace() };

				unsigned int and_mask_size = (shape_report->width + 7) / 8 *
	                             	 	 	 shape_report->height;

				unsigned char *shape = shape_report->shape +
				                       ((and_mask_size + 3) & ~3);

#if 0
				PDBG("shape_report->shape = %p", shape_report->shape);
				PDBG("and_mask_size = %u", and_mask_size);
				PDBG("shape = %p", shape);
#endif

				for (unsigned int y = 0; y < shape_report->height; y++) {
					for (unsigned int x = 0; x < shape_report->width; x++) {
						//Genode::printf("%8x ", *(unsigned int*)&shape[y * shape_report->width * 4 + x]);
						if (*(unsigned int*)&shape[y * shape_report->width * 4 + x * 4] & 0xffffff)
							Genode::printf(".");
						else
							Genode::printf(" ");
					}
					Genode::printf("\n");
				}


				convert_vbox_cursor_data_to_pixels(ds.local_addr<Genode::Pixel_rgb565>(),
				                                   shape,
				                                   cursor_size);
				nitpicker.framebuffer()->refresh(0, 0, cursor_size.w(), cursor_size.h());

				Nitpicker::Rect geometry(Nitpicker::Point(0, 0), cursor_size);
				nitpicker.enqueue<Nitpicker::Session::Command::Geometry>(view, geometry);

			} else {

				Nitpicker::Rect geometry(Nitpicker::Point(0, 0), Nitpicker::Area(0, 0));
				nitpicker.enqueue<Nitpicker::Session::Command::Geometry>(view, geometry);

			}

			nitpicker.execute();
		}
	}

	void update_pointer()
	{
		if (xray /*|| !hidden_domains.contains(current_domain)*/)
			show_default_pointer();
		else
			show_vbox_pointer();
	}

	Main()
	{
		/* TODO should be read from config */
		hidden_domains.add("vbox");

		/* register signal handlers */
		hover_ds.sigh(hover_signal_dispatcher);
		xray_ds.sigh(xray_signal_dispatcher);
		shape_ds.sigh(shape_signal_dispatcher);

		nitpicker.enqueue<Nitpicker::Session::Command::To_front>(view);
		nitpicker.execute();

		/* import initial state */
		handle_hover();
		handle_xray();
		handle_shape();
	}
};


static Domain_name read_string_attribute(Genode::Xml_node const &node,
                                         char             const *attr,
                                         Domain_name      const &default_value)
{
	try {
		char buf[Domain_name::capacity()];
		node.attribute(attr).value(buf, sizeof(buf));
		return Domain_name(buf);
	}
	catch (...) {
		return default_value; }
}


void Main::handle_hover(unsigned)
{
	hover_ds.update();
	if (!hover_ds.is_valid())
		return;

	/* read new hover information from nitpicker's hover report */
	try {
		Genode::Xml_node node(hover_ds.local_addr<char>());

		current_domain = read_string_attribute(node, "domain", Domain_name());

		update_pointer();
	}
	catch (...) {
		PWRN("could not parse hover report");
	}
}


void Main::handle_xray(unsigned)
{
	xray_ds.update();
	if (!xray_ds.is_valid())
		return;

	try {
		Genode::Xml_node node(xray_ds.local_addr<char>());

		xray = node.has_attribute("enabled")
		    && node.attribute("enabled").has_value("yes");

		update_pointer();
	}
	catch (...) {
		PWRN("could not parse xray report");
	}
}


void Main::handle_shape(unsigned)
{
	shape_ds.update();
	if (!shape_ds.is_valid())
		return;

PDBG("called");

	vbox_pointer_shape_changed = true;

	update_pointer();
}


int main()
{
	static Main main;

	/* dispatch signals */
	for (;;) {

		Genode::Signal sig = main.sig_rec.wait_for_signal();
		Genode::Signal_dispatcher_base *dispatcher =
			dynamic_cast<Genode::Signal_dispatcher_base *>(sig.context());

		if (dispatcher)
			dispatcher->dispatch(sig.num());
	}
}
