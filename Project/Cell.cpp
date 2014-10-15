#include "stdafx.h"
#include "Cell.h"


Cell::Cell(Offset oX, Offset oY, ConsoleColor fg, ConsoleColor bg, Symbol sym)
{
	this->oX = oX;
	this->oY = oY;
	this->fg = fg;
	this->bg = bg;
	this->sym = sym;
}

bool Cell::Show(int left, int top) const
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