/*
 * \brief  Console backend for NOVA
 * \author Norman Feske
 * \author Alexander Boettcher
 * \date   2009-12-28
 */

/*
 * Copyright (C) 2009-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/stdint.h>
#include <base/console.h>
#include <base/printf.h>
#include <util/string.h>

extern "C" unsigned short * vga_base; 

/**
 * Read byte from I/O port
 */
inline Genode::uint8_t inb(Genode::uint16_t port)
{
	Genode::uint8_t res;
	asm volatile ("inb %%dx, %0" :"=a"(res) :"Nd"(port));
	return res;
}


/**
 * Write byte to I/O port
 */
inline void outb(Genode::uint16_t port, Genode::uint8_t val)
{
	asm volatile ("outb %b0, %w1" : : "a" (val), "Nd" (port));
}


/**
 * Definitions of PC serial ports
 */
enum {
	MAP_ADDR_BDA       = 0x1000,

	BDA_SERIAL_BASE_COM1 = 0x400,
	BDA_EQUIPMENT_WORD   = 0x410,
	BDA_EQUIPMENT_SERIAL_COUNT_MASK  = 0x7,
	BDA_EQUIPMENT_SERIAL_COUNT_SHIFT = 9,

	COMPORT_DATA_OFFSET   = 0,
	COMPORT_STATUS_OFFSET = 5,

	STATUS_THR_EMPTY = 0x20,  /* transmitter hold register empty */
	STATUS_DHR_EMPTY = 0x40,  /* data hold register empty - data completely sent */
};


/**
 * Initialize serial port
 *
 * Based on 'init_serial' of L4ka::Pistachio's 'kdb/platform/pc99/io.cc'
 */
static void init_comport(Genode::uint16_t port, unsigned baud)
{
	if (!port)
		return;

	const unsigned
		IER  = port + 1,
		EIR  = port + 2,
		LCR  = port + 3,
		MCR  = port + 4,
		LSR  = port + 5,
		MSR  = port + 6,
		DLLO = port + 0,
		DLHI = port + 1;

	outb(LCR, 0x80);  /* select bank 1 */
	for (volatile int i = 10000000; i--; );
	outb(DLLO, (115200/baud) >> 0);
	outb(DLHI, (115200/baud) >> 8);
	outb(LCR, 0x03);  /* set 8,N,1 */
	outb(IER, 0x00);  /* disable interrupts */
	outb(EIR, 0x07);  /* enable FIFOs */
	outb(MCR, 0x0b);  /* force data terminal ready */
	outb(IER, 0x01);  /* enable RX interrupts */
	inb(IER);
	inb(EIR);
	inb(LCR);
	inb(MCR);
	inb(LSR);
	inb(MSR);
}


/**
 * Output character to serial port
 */
inline void serial_out_char(Genode::uint16_t comport, Genode::uint8_t c)
{
	/* wait until serial port is ready */
	Genode::uint8_t ready = STATUS_THR_EMPTY;
	while ((inb(comport + COMPORT_STATUS_OFFSET) & ready) != ready);

	/* output character */
	outb(comport + COMPORT_DATA_OFFSET, c);
}

inline void vga_configure(unsigned reg_offset, unsigned reg_index,
                          unsigned value)
{
	/* select the register by programming the index of the wanted register */   
  	outb(0x3c0 + reg_offset, reg_index);
	/* write the value to the actual register */
	outb(0x3c1 + reg_offset, value);
}


namespace Genode {

	class Core_console : public Console
	{
		private:

			uint16_t _comport;

			unsigned _vga_pos;
			unsigned _buf_pos;

			unsigned short _color;
			unsigned char _buf[10];

			struct {
				const char * color_string;
				unsigned short color;
			} _map [6];

		protected:

			void _out_char(char c)
			{
				if (_comport) {
					if (c == '\n')
						serial_out_char(_comport, '\r');
					serial_out_char(_comport, c);
				}

				if (!vga_base) return;

				/* remember last 4 chars */
				_buf[_buf_pos++] = c;

				unsigned i;
				for (i=0; i < sizeof(_map) / sizeof(_map[0]); i++) {
					/* look for color string */
					if (memcmp(_buf, _map[i].color_string, _buf_pos)) continue;
					break;
				}

				/* check for matching color string */
				if (i < sizeof(_map) / sizeof(_map[0])) {
					if ((_buf_pos == 4) && (i == 5)) {
						_color = _map[i].color;
						_buf_pos = 0;
					} else
					if (_buf_pos == 5) {
						_color = _map[i].color;
						_buf_pos = 0;
					}
					return;
				}

				/* if nothing was found we're done */
				if (_buf_pos > 1) {
					_buf_pos = 0;
					return;
				}
				/* reset buffer index */
				_buf_pos = 0;

				if (c != '\n')
					vga_base[_vga_pos++] = _color | c;
				else {
					/* clear the line */
					while (_vga_pos % 80)
						vga_base[_vga_pos++] = _color | ' ';

				}
				_vga_pos %= 24 * 80;

				/* set the cursor after last character */
				vga_configure(0x14, 0xe, _vga_pos >> 8);
				vga_configure(0x14, 0xf, _vga_pos);
			}

		public:

			Core_console() : _comport(0), _vga_pos(0), _buf_pos(0), _color(0xf00)
			{
				/**
				 * Read BDA (Bios Data Area) to obtain I/O ports of COM
				 * interfaces. The page must be mapped by the platform code !
				 */
				char * map_bda = reinterpret_cast<char *>(MAP_ADDR_BDA);
				uint16_t serial_count = *reinterpret_cast<uint16_t *>(map_bda + BDA_EQUIPMENT_WORD);
				serial_count >>= BDA_EQUIPMENT_SERIAL_COUNT_SHIFT;
				serial_count &= BDA_EQUIPMENT_SERIAL_COUNT_MASK;

				if (serial_count > 0)
					_comport = *reinterpret_cast<uint16_t *>(
					            map_bda + BDA_SERIAL_BASE_COM1);

				init_comport(_comport, 115200);

				_map [0].color_string = ESC_WRN;
				_map [0].color        = 0x100;
				_map [1].color_string = ESC_INF;
				_map [1].color        = 0x200;
				_map [2].color_string = ESC_LOG;
				_map [2].color        = 0xe00;
				_map [3].color_string = ESC_ERR;
				_map [3].color        = 0x400;
				_map [4].color_string = ESC_DBG;
				_map [4].color        = 0x100;
				_map [5].color_string = ESC_END;
				_map [5].color        = 0xf00;

			}
	};
}

