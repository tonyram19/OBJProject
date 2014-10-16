#pragma once

#include "stdafx.h"

class Image2D
{

private:

	ConsoleColor fg;
	ConsoleColor bg;
	string text;
	unsigned short width;
	unsigned short height;

public:

	//Constructors and Trilogy 
	Image2D();
	Image2D(ConsoleColor fg, ConsoleColor bg, string text);
	Image2D(const Image2D& obj);
	Image2D& operator=(const Image2D& obj);
	virtual ~Image2D();

	//Mutators
	void setFg(ConsoleColor fg) { this->fg = fg; }
	void setBg(ConsoleColor bg) {this->bg = bg; }
	void setText(string text);

	//Accesors
	ConsoleColor getFg() const { return this->fg; }
	ConsoleColor getBg() const { return this->bg; }
	string getText() const { return text; }
	unsigned short getWidth() const;
	unsigned short getHeight() const;

	//Insertion operator overload
	friend std::ostream& operator<<(std::ostream& ostr, const Image2D& obj);

	//Stuff
	virtual void Show() const;

};

