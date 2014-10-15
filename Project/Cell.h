#pragma once

template <class Offset = signed char, class Symbol = wchar_t>
struct Cell
{
	Offset oX, oY;
	ConsoleColor fg, bg;
	Symbol sym;

	Cell() { }
	Cell(Offset oX, Offset oY, ConsoleColor fg, ConsoleColor bg, Symbol sym);

	bool Show(int left, int top) const;
};

template <class Offset = signed char, class Symbol = wchar_t>
Cell<Offset, Symbol>::Cell(Offset oX, Offset oY, ConsoleColor fg, ConsoleColor bg, Symbol sym)
{
	this->oX = oX;
	this->oY = oY;
	this->fg = fg;
	this->bg = bg;
	this->sym = sym;
}

template <class Offset = signed char, class Symbol = wchar_t>
bool Cell<Offset, Symbol>::Show(int left, int top) const
{
	if (this->oX + left < 0 || this->oX + left >= Console::WindowWidth ||
		this->oY + top < 0 || this->oY + top >= Console::WindowHeight)
		return false;

	Console::BackgroundColor = this->bg;
	Console::ForegroundColor = this->fg;
	Console::SetCursorPosition(this->oX + left, this->oY + top);
	Console::Write(this->sym);
	Console::ResetColor();

	return true;
}