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
#include <os/attached_rom_dataspace.h>
#include <os/surface.h>
#include <os/pixel_rgb565.h>
#include <util/xml_node.h>
#include <nitpicker_session/connection.h>

/* local includes */
#include "big_mouse.h"


template <typename PT>
static void convert_cursor_data_to_pixels(PT *pixel, Nitpicker::Area size,
                                          unsigned char alpha_value)
{
	unsigned char *alpha = (unsigned char *)(pixel + size.count());

	for (unsigned y = 0; y < size.h(); y++) {
		for (unsigned x = 0; x < size.w(); x++) {

			/* the source is known to be in RGB565 format */
			Genode::Pixel_rgb565 src =
				*(Genode::Pixel_rgb565 *)(&big_mouse.pixels[y][x]);

			unsigned const i = y*size.w() + x;
			pixel[i] = PT(src.r(), src.g(), src.b());
			/* XXX set alpha value if the pixel has some red */
			alpha[i] = src.r() ? alpha_value : 0;
		}
	}

	/* XXX debug */
	if (false && alpha_value == 0) {
		PT p(0,255,0);
		unsigned char a = 255;
		pixel[0*size.w() + 0] = p;
		pixel[1*size.w() + 1] = p;
		pixel[2*size.w() + 2] = p;
		alpha[0*size.w() + 0] = a;
		alpha[1*size.w() + 1] = a;
		alpha[2*size.w() + 2] = a;
	}
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

	Genode::Signal_receiver sig_rec;

	void handle_hover(unsigned num = 0);
	void handle_xray(unsigned num = 0);

	Genode::Signal_dispatcher<Main> hover_signal_dispatcher {
		sig_rec, *this, &Main::handle_hover };
	Genode::Signal_dispatcher<Main> xray_signal_dispatcher {
		sig_rec, *this, &Main::handle_xray };

	Nitpicker::Connection nitpicker;

	Nitpicker::Area const cursor_size { big_mouse.w, big_mouse.h };

	Framebuffer::Mode const mode { (int)cursor_size.w(), (int)cursor_size.h(),
	                               Framebuffer::Mode::RGB565 };

	Genode::Dataspace_capability init_buffer()
	{
		nitpicker.buffer(mode, true /* use alpha */);
		return nitpicker.framebuffer()->dataspace();
	}

	Genode::Attached_dataspace ds { init_buffer() };

	Domain_list hidden_domains;
	Domain_name current_domain;

	bool xray   = false;
	bool hidden = false;

	void show_pointer()
	{
		if (hidden) {
			convert_cursor_data_to_pixels(ds.local_addr<Genode::Pixel_rgb565>(),
			                              cursor_size, 255);
			hidden = false;
			nitpicker.framebuffer()->refresh(0, 0, cursor_size.w(), cursor_size.h());
		}
	}

	void hide_pointer()
	{
		if (!hidden) {
			convert_cursor_data_to_pixels(ds.local_addr<Genode::Pixel_rgb565>(),
			                              cursor_size, 0);
			hidden = true;
			nitpicker.framebuffer()->refresh(0, 0, cursor_size.w(), cursor_size.h());
		}
	}

	void update_pointer()
	{
		if (xray || !hidden_domains.contains(current_domain))
			show_pointer();
		else
			hide_pointer();
	}

	Main()
	{
		/* TODO should be read from config */
		hidden_domains.add("vbox");

		/* register signal handlers */
		hover_ds.sigh(hover_signal_dispatcher);
		xray_ds.sigh(xray_signal_dispatcher);

		convert_cursor_data_to_pixels(ds.local_addr<Genode::Pixel_rgb565>(),
		                              cursor_size, 255);

		Nitpicker::Session::View_handle view = nitpicker.create_view();
		Nitpicker::Rect geometry(Nitpicker::Point(0, 0), cursor_size);
		nitpicker.enqueue<Nitpicker::Session::Command::Geometry>(view, geometry);
		nitpicker.enqueue<Nitpicker::Session::Command::To_front>(view);
		nitpicker.execute();

		/* import initial state */
		handle_hover();
		handle_xray();
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
