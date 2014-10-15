#pragma once
#define Offset signed char
#define Symbol wchar_t

struct Cell
{
	Offset oX, oY;
	ConsoleColor fg, bg;
	Symbol sym;

	Cell() { }
	Cell(Offset oX, Offset oY, ConsoleColor fg, ConsoleColor bg, Symbol sym);

	bool Show(int left, int top) const;
};
