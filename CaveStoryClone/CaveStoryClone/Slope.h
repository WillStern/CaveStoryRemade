#pragma once
#include "Globals.h"
#include "Rectangle.h"

#include <cmath>

class Slope
{
public:
	Slope() {}
	Slope(Vector2 p1, Vector2 p2) :
		_p1(p1),
		_p2(p2)
	{
		if (this->_p2.x - this->_p1.x != 0)
		{
			this->_slope = (float)(abs(this->_p2.y) - abs(this->_p1.y)) / (float)(abs(this->_p2.x) - abs(this->_p1.x));
		}
	}

	const inline float GetSlope() const { return this->_slope; }

	const bool CollidesWith(const Rectangle &other) const
	{
		//checks every case in which our given rect is colliding with the slope
		return
			(	other.GetRight()	>= this->_p2.x &&
				other.GetLeft()		<= this->_p1.x &&
				other.GetTop()		<= this->_p2.y &&
				other.GetBottom()	>= this->_p1.y ) ||

			(	other.GetRight()	>= this->_p1.x &&
				other.GetLeft()		<= this->_p2.x &&
				other.GetTop()		<= this->_p1.y &&
				other.GetBottom()	>= this->_p2.y ) ||

			(	other.GetRight()	>= this->_p2.x &&
				other.GetLeft()		<= this->_p1.x &&
				other.GetTop()		<= this->_p1.y &&
				other.GetBottom()	>= this->_p2.y)  ||

			(	other.GetRight()	>= this->_p1.x &&
				other.GetLeft()		<= this->_p2.x &&
				other.GetTop()		<= this->_p2.y &&
				other.GetBottom()	>= this->_p1.y );
	}

	const inline Vector2 GetP1() const { return this->_p1; }
	const inline Vector2 GetP2() const { return this->_p2; }
private:		
	Vector2 _p1, _p2;
	float _slope;
};