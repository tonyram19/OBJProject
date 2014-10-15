#include "stdafx.h"
#include "Image2D.h"

Image2D::Image2D()
{
}

Image2D::Image2D(ConsoleColor fg, ConsoleColor bg, string text)
{
	this->setText(text);
	this->setBg(bg);
	this->setFg(fg);
}

Image2D::Image2D(const Image2D& obj)
{
	this->setText(obj.getText());
	this->setBg(obj.getBg());
	this->setFg(obj.getFg());
	this->width = obj.getWidth();
	this->height = obj.getHeight();
}

Image2D& Image2D::operator=(const Image2D& obj)
{
	if (this != &obj)
	{
		this->setText(obj.getText());
		this->setBg(obj.getBg());
		this->setFg(obj.getFg());
		this->width = obj.getWidth();
		this->height = obj.getHeight();
	}
	
	return *this;
}


Image2D::~Image2D()
{
}

void Image2D::setText(string text)
{	
	this->text = text;
}

unsigned short Image2D::getWidth() const
{
	unsigned int i;
	for (i = 0; i < this->text.length(); i++)
	{
		if (this->getText()[i] == '\n')
			break;
	}
	return i;
}

unsigned short Image2D::getHeight() const
{
	int lineCounter = 0;
	for (unsigned int i = 0; i < this->text.length(); i++)
	{
		if (this->getText()[i] == '\n')
			lineCounter++;
	}
	return lineCounter + 1;
}

void Image2D::Show() const
{
	Console::BackgroundColor = this->getBg();
	Console::ForegroundColor = this->getFg();

	cout << this->getText();

	Console::ResetColor();
}

std::ostream& operator<<(std::ostream& ostr, const Image2D& obj)
{
	Console::BackgroundColor = obj.getBg();
	Console::ForegroundColor = obj.getFg();

	for (unsigned int i = 0; i < obj.getText().length(); i++)
		ostr << obj.getText()[i];

	Console::ResetColor();
	return ostr;
}
