#pragma once
#include "Image2D.h"

class Sprite : public Image2D
{

private:

	string name;
	int left;
	int top;

public:

	//Constructors and trilogy
	Sprite();
	Sprite(ConsoleColor fg, ConsoleColor bg, string text, string name, int left, int top);
	Sprite(const Sprite& obj);
	Sprite& operator=(const Sprite& obj);
	~Sprite();

	//Mutators
	void setName(string name);
	void setLeft(int left) { this->left = left; }
	void setTop(int top) { this->top = top; }

	//Accesors
	string getName() const { return this->name; }
	int getLeft() const { return this->left; }
	int getTop() const { return this->top; }
	
	//Stuff
	virtual void Show() const;
	bool OutOfBounds(int x, int y) const;
	bool Collides(int x, int y, unsigned short w, unsigned short h) const;

};

