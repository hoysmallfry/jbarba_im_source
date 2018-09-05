#ifndef COLOR_H
#define COLOR_H

#include "SqueakyCleanWindows.h"
#include <iosfwd>

namespace Colors
{
	const unsigned Black = NULL;
	const unsigned DarkRed = FOREGROUND_RED;
	const unsigned DarkGreen = FOREGROUND_GREEN;
	const unsigned DarkBlue = FOREGROUND_BLUE;
	const unsigned bright = FOREGROUND_INTENSITY;

	const unsigned DarkYellow = DarkRed | DarkGreen;
	const unsigned DarkCyan = DarkGreen | DarkBlue;
	const unsigned DarkMagenta = DarkRed | DarkBlue;

	const unsigned Red = DarkRed | bright;
	const unsigned Green = DarkGreen | bright;
	const unsigned Blue = DarkBlue | bright;

	const unsigned Yellow = DarkYellow | bright;
	const unsigned Cyan = DarkCyan | bright;
	const unsigned Maganta = DarkMagenta | bright;

	const unsigned Gray =  DarkRed | DarkGreen | DarkBlue;
	const unsigned White = Gray | bright;
}
typedef unsigned Color;

namespace Console
{
	namespace detail
	{
		struct color_manipulator
		{
			Color color;

			~color_manipulator(void);
		};

		std::ostream& operator<<(std::ostream& sink, const color_manipulator& color);
	}

	//Takes a color value and creates a background color.
	unsigned BackgroundColor( const unsigned& color );
	//Takes a value for a foreground and background color and makes a composite value.
	unsigned CompositeColor( const unsigned& foreground, const unsigned& background );

	//Sets the color of the console window text.
	void SetConsoleColor( const unsigned& color );

	const detail::color_manipulator SetColor( Color color );
};

#endif
