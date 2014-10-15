#include "stdafx.h"
#include "Missile.h"


Missile::Missile(ConsoleColor fg, ConsoleColor bg, string text, string name, int left, int top, Delta dX, Delta dY) :
Sprite(fg, bg, text, name, left, top)
{
	this->Disable();
	this->velocity.resize(2);
	this->SetDeltas(dX, dY);
}

void Missile::SetDeltas(Delta dx, Delta dy)
{
	this->velocity[velX] = dx;
	this->velocity[velY] = dy;
}

void Missile::GetDeltas(Delta& dx, Delta& dy)
{
	dx = this->velocity[velX];
	dy = this->velocity[velY];
}

void Missile::Show() const
{
	if (this->isEnabled())
		Sprite::Show();
}