#pragma once
#include "Sprite.h"

#define velX 0
#define velY 1

class Missile :	public Sprite
{

public:

	typedef signed char Delta;
	
	Missile(ConsoleColor fg, ConsoleColor bg, string text, string name, int left, int top, Delta dX, Delta dY);

	void SetDeltas(Delta x, Delta y);
	void GetDeltas(Delta& x, Delta& y);
	void Show() const;
	void Enable() { this->enabled = true; }
	void Disable() { this->enabled = false; }
	bool isEnabled() const { return this->enabled; }

private:

	vector<Delta> velocity;
	bool enabled;

	

};

