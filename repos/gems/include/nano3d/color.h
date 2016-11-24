/*
 * \brief   Color representation
 * \date    2010-09-27
 * \author  Norman Feske
 */

#ifndef _NANO3D__COLOR_H_
#define _NANO3D__COLOR_H_

namespace Nano3d {

	class Color
	{
		public:

			enum {
				TRANSPARENT = 0,
				OPAQUE      = 255
			};

			int r, g, b, a;

			/**
			 * Constructors
			 */
			Color(int red, int green, int blue, int alpha = OPAQUE)
			{
				rgba(red, green, blue, alpha);
			}

			Color() { rgba(0, 0, 0); }

			/**
			 * Convenience function: Assign rgba values
			 */
			inline void rgba(int red, int green, int blue, int alpha = 255)
			{
				r = red;
				g = green;
				b = blue;
				a = alpha;
			}
	};
}

#endif /* _NANO3D__COLOR_H_ */
