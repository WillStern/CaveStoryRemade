#pragma once
#include "Rectangle.h"
#include "Globals.h"

#include <string>

class Door : public Rectangle
{
public:
	Door() {}
	Door(Rectangle r, std::string destination) :
		Rectangle(r.GetLeft() * (int)globals::SPRITE_SCALE, r.GetTop() * (int)globals::SPRITE_SCALE, r.GetWidth() * (int)globals::SPRITE_SCALE, r.GetHeight() * (int)globals::SPRITE_SCALE),
		_destination(destination)
	{}

	const inline Rectangle GetRectangle() const { return this->GetRect(); }
	const inline std::string GetDestination() const { return this->_destination; }

private:
	std::string _destination;

};