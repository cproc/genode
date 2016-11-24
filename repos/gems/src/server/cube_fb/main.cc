/*
 * \brief  Live backdrop showing the Genode process tree
 * \author Norman Feske
 * \date   2012-10-09
 */

/* Genode includes */
#include <timer_session/connection.h>
#include <cap_session/connection.h>
#include <base/rpc_server.h>
#include <framebuffer_session/connection.h>
#include <base/sleep.h>
#include <os/attached_ram_dataspace.h>
#include <root/component.h>
#include <os/static_root.h>

/* local includes */
#include <vertex_array.h>
#include <nano3d/canvas_rgb565.h>


typedef Nano3d::Pixel_rgb565 PT;


struct Animator
{
	Genode::Lock mutable _lock;

	unsigned _alpha, _beta, _gamma;

	Animator() : _alpha(0), _beta(0), _gamma(0) { }

	void animate()
	{
		Genode::Lock::Guard guard(_lock);

		_alpha = (_alpha + 3) % 1024;
		_beta  = (_beta  + 4) % 1024;
		_gamma = (_gamma + 5) % 1024;
	}

	unsigned alpha() const { Genode::Lock::Guard guard(_lock); return _alpha; }
	unsigned beta()  const { Genode::Lock::Guard guard(_lock); return _beta;  }
	unsigned gamma() const { Genode::Lock::Guard guard(_lock); return _gamma; }
};


static Animator *animator()
{
	static Animator inst;
	return &inst;
}


struct Heap_allocator : Nano3d::Allocator
{
	void *alloc(unsigned long size)
	{
		return Genode::env()->heap()->alloc(size);
	}

	void free(void *ptr, unsigned long size)
	{
		Genode::env()->heap()->free(ptr, size);
	}
};


enum { MAX_VERTICES_PER_FACE = 4 };
class Face
{
	private:

		unsigned _num_vertices;
		int _vertex_indices[MAX_VERTICES_PER_FACE];

	public:

		/**
		 * Constructor
		 */
		Face(int idx1, int idx2, int idx3, int idx4)
		: _num_vertices(4)
		{
			_vertex_indices[0] = idx1;
			_vertex_indices[1] = idx2;
			_vertex_indices[2] = idx3;
			_vertex_indices[3] = idx4;
		}

		/**
		 * Default constructor creates invalid face
		 */
		Face() : _num_vertices(0) { }

		/**
		 * Return number of face vertices
		 */
		unsigned num_vertices() { return _num_vertices; }

		/**
		 * Return vertex index of face point
		 *
		 * \param i  Index of face point. If 'i' is out of
		 *           range, the first face point is returned.
		 */
		int vertex_index(unsigned i) {
			return (i < _num_vertices) ? _vertex_indices[i]
			                           : _vertex_indices[0]; }
};


namespace Framebuffer {
	using namespace Genode;
	class Session_component;
	class Root;
}


class Framebuffer::Session_component : public Genode::Rpc_object<Session>
{
	private:

		struct Output : Connection
		{
			PT * const local;

			Mode const mode;

			Output()
			:
				local(env()->rm_session()->attach(dataspace())),
				mode(Connection::mode())
			{
				Genode::log("attached to ", local);
			}

		} _output;

		Nano3d::Area const _area;

		unsigned char *_input_alpha;
		unsigned char *_output_alpha;

		Attached_ram_dataspace _input;

		/*
		 * Dimension edge buffers such that they can hold the
		 * interpolated edge values of 5 different attributes.
		 */
		enum { MAX_FB_HEIGHT = 2000 };
		int _l_edge[MAX_FB_HEIGHT*5];
		int _r_edge[MAX_FB_HEIGHT*5];

		Nano3d::Chunky_canvas<PT> _canvas;

		Nano3d::Chunky_canvas<PT>::Texture _input_texture;

	public:

		Session_component()
		:
			_area(_output.mode.width(), _output.mode.height()),
			_input_alpha((unsigned char *)env()->heap()->alloc(_area.num_pixels())),
			_output_alpha((unsigned char *)env()->heap()->alloc(_area.num_pixels())),
			_input(env()->ram_session(), _area.num_pixels()*sizeof(PT)),
			_canvas(_output.local, _output_alpha, _area.num_pixels(),
			        _area, _l_edge, _r_edge),
			_input_texture(_input.local_addr<PT>(), _input_alpha, _area)
		{
			_canvas.clip(Nano3d::Rect(Nano3d::Point(0, 0), _area));

			Genode::log("input ds valid=", _input.cap().valid());

			/* init alpha buffer */
			for (unsigned j = 0; j < _area.h(); j++) {
				for (unsigned i = 0; i < _area.w(); i++) {
					_input_alpha[_area.w()*j + i] = 255;
					_output_alpha[_area.w()*j + i] = 255;
				}
			}
		}

		Genode::Dataspace_capability dataspace()
		{

			Genode::log("client requests fb, valid=", _input.cap().valid());
			return _input.cap();
		}

		void release() { }

		Mode mode() const
		{
			return _output.mode;
		}

		void mode_sigh(Genode::Signal_context_capability) { }

		void refresh(int x, int y, int w, int h)
		{
			using namespace Nano3d;

			PT *src = _input.local_addr<PT>();
			PT *dst = _output.local;

			for (unsigned j = 0; j < _area.h(); j++) {

				for (unsigned i = 0; i < _area.w(); i++) {

					*dst = PT::avr(*dst, PT(src->g(), src->b(), src->r()));

					dst++;
					src++;
				}
			}

			enum { SIZE = 10000 };
			Vertex_array<8> vertices;
			vertices[0] = Vertex(-SIZE, -SIZE, -SIZE);
			vertices[1] = Vertex(-SIZE, -SIZE,  SIZE);
			vertices[2] = Vertex(-SIZE,  SIZE,  SIZE);
			vertices[3] = Vertex(-SIZE,  SIZE, -SIZE);
			vertices[4] = Vertex( SIZE, -SIZE, -SIZE);
			vertices[5] = Vertex( SIZE, -SIZE,  SIZE);
			vertices[6] = Vertex( SIZE,  SIZE,  SIZE);
			vertices[7] = Vertex( SIZE,  SIZE, -SIZE);

			vertices.rotate_x(animator()->alpha());
			vertices.rotate_y(animator()->beta());
			vertices.rotate_z(animator()->gamma());

			vertices.project(1600, _area.h());

			vertices.translate(_area.w() / 2, _area.h() / 2, 0);

			enum { NUM_FACES = 6 };
			Face faces[NUM_FACES] = {
				Face(0, 4, 7, 3), /* front  */
				Face(6, 7, 4, 5), /* right  */
				Face(5, 1, 2, 6), /* back   */
				Face(0, 3, 2, 1), /* left   */
				Face(4, 0, 1, 5), /* top    */
				Face(2, 3, 7, 6), /* bottom */
			};

			unsigned pad_w = _area.w() / 8;
			unsigned pad_h = _area.h() / 8;

			for (unsigned i = 0; i < NUM_FACES; i++) {
				Textured_polypoint polygon[MAX_VERTICES_PER_FACE];

				Face &f = faces[i];
				for (unsigned j = 0; j < f.num_vertices(); j++) {
					int v = f.vertex_index(j);
					Point texpos;
					switch (j) {
					default:
					case 0: texpos = Point(pad_w, pad_h); break;
					case 1: texpos = Point(_area.w() - pad_w - 1, pad_h); break;
					case 2: texpos = Point(_area.w() - pad_w - 1, _area.h() - pad_h - 1); break;
					case 3: texpos = Point(pad_w, _area.h() - pad_h - 1); break;
					}
					polygon[j] = Textured_polypoint(vertices[v].x(),
					                                vertices[v].y(),
					                                texpos);
				}
				_canvas.draw_textured_polygon(polygon, f.num_vertices(), &_input_texture);
			}

			_output.refresh(0, 0, _area.w(), _area.h());
		}

		void sync_sigh(Genode::Signal_context_capability) override { }
};


int main(int argc, char **argv)
{
	using namespace Genode;

	Nano3d::init_sincos_tab();

	enum { STACK_SIZE = 4096 };
	static Cap_connection cap;
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "fade_fb_ep");

	static Framebuffer::Session_component fb_session;
	static Static_root<Framebuffer::Session> fb_root(ep.manage(&fb_session));

	env()->parent()->announce(ep.manage(&fb_root));

	for (;;) {
		static Timer::Connection timer;

		timer.msleep(10);

		animator()->animate();
	}

	sleep_forever();
	return 0;
}
