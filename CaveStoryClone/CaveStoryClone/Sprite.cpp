#include "Sprite.h"
#include "Graphics.h"
#include "Globals.h"

Sprite::Sprite() 
{

}

Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY)
	: _x(posX),
	_y(posY)
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.GetRenderer(), graphics.LoadImage(filePath));
	if (this->_spriteSheet == NULL)
	{
		printf("\nError: Unable to load image\n");
	}

	this->_boundingBox = Rectangle(_x, _y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);

}

Sprite::~Sprite() {

}

void Sprite::Draw(Graphics &graphics, int x, int y) 
{
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * (int)globals::SPRITE_SCALE, this->_sourceRect.h * (int)globals::SPRITE_SCALE};
	graphics.BlitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::Update()
{
	this->_boundingBox = Rectangle(this->_x, this->_y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::GetBoundingBox() const
{
	return this->_boundingBox;
}

//determines what side the collision happened on
const sides::Side Sprite::GetCollisionSide(Rectangle &other) const
{
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->_boundingBox.GetRight() - other.GetLeft();
	amtLeft = other.GetRight() - this->_boundingBox.GetLeft();
	amtTop = other.GetBottom() - this->_boundingBox.GetTop();
	amtBottom = this->_boundingBox.GetBottom() - other.GetTop();

	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i=0; i<4; i++)
	{
		if (vals[i] < lowest)
			lowest = vals[i];
	}

	return
		lowest == abs(amtRight) ? sides::RIGHT :
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}


void Sprite::SetSourceRectX(int value)
{
	this->_sourceRect.x = value;
}

void Sprite::SetSourceRectY(int value)
{
	this->_sourceRect.y = value;
}

void Sprite::SetSourceRectW(int value)
{
	this->_sourceRect.w = value;
}

void Sprite::SetSourceRectH(int value)
{
	this->_sourceRect.h = value;
}
