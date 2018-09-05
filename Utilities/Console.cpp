#include "Console.h"

namespace Console
{
	namespace detail
	{
		color_manipulator::~color_manipulator(void)
		{
			SetConsoleColor( Colors::White );
		}

		std::ostream& operator<<(std::ostream& sink, const color_manipulator& color)
		{
			SetConsoleColor( color.color );

			return sink;
		}
	}

	unsigned BackgroundColor( const unsigned& color )
	{
		//shifts the color up one bit.
		return color * 0x0010;
	}

	unsigned CompositeColor( const unsigned& foreground, const unsigned& background )
	{
		//combines the value of the two colors.
		return foreground | BackgroundColor(background);
	}

	void SetConsoleColor( const unsigned& color )
	{
		//Get a handle to the output.
		HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

		//Sets the console color back to the default.
		SetConsoleTextAttribute( hConsoleOutput, color );
	}

	const detail::color_manipulator SetColor( Color color )
	{
		const detail::color_manipulator result = { color };
		return result;
	}
}
