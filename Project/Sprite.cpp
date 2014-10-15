#include "stdafx.h"
#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::Sprite(ConsoleColor fg, ConsoleColor bg, string text, string name, int left, int top) 
	: Image2D(fg, bg, text)
{
	this->setName(name);
	this->setLeft(left);
	this->setTop(top);
}

Sprite::Sprite(const Sprite& obj) : Image2D(obj)
{
	this->setName(obj.getName());
	this->setLeft(obj.getLeft());
	this->setTop(obj.getTop());
}

Sprite& Sprite::operator=(const Sprite& obj)
{
	if (this != &obj)
	{
		Image2D::operator=(obj);
		this->setName(obj.getName());
		this->setLeft(obj.getLeft());
		this->setTop(obj.getTop());
	}
	
	return *this;
}

Sprite::~Sprite()
{
}


void Sprite::setName(string name)
{
	this->name = name;
}

void Sprite::Show() const
{
	int lineCounter = 0;

	Console::SetCursorPosition(this->getLeft(), this->getTop() + lineCounter);
	Console::BackgroundColor = this->getBg();
	Console::ForegroundColor = this->getFg();

	for (unsigned int i = 0; i < this->getText().length(); ++i)
	{
		if (this->getText()[i] == '\n')
		{
			lineCounter++;
			Console::SetCursorPosition(this->getLeft(), this->getTop() + lineCounter);
		}
		else
			cout << this->getText()[i];
	}

	Console::ResetColor();
}

bool Sprite::OutOfBounds(int x, int y) const
{
	if (!(x >= 0 && x < Console::WindowWidth - this->getWidth()))
		return true;

	if (!(y >= 1 && y < Console::WindowHeight - this->getHeight()))
		return true;
	
	return false;
}

bool Sprite::Collides(int x, int y, unsigned short w, unsigned short h) const
{
	if ( x + w >= this->getLeft() && x <= this->getLeft() + this->getWidth() && 
		 y + h > this->getTop() && y < this->getTop() + this->getHeight() )
		return true;

	return false;
}