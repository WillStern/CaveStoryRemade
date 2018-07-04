#pragma once
#include "Graphics.h"
#include "Rectangle.h"
#include "Globals.h"
#include <string>
#include <SDL/SDL.h>

/*Sprite class
holds all info for individual sprites
*/

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height, float posX, float posY);
	virtual ~Sprite();
	virtual void Update();
	void Draw(Graphics &graphics, int x, int y);

	const Rectangle GetBoundingBox() const;
	const sides::Side GetCollisionSide(Rectangle &other) const;

	const inline float GetX() const { return this->_x; }
	const inline float GetY() const { return this->_y; }

	void SetSourceRectX(int value);
	void SetSourceRectY(int value);
	void SetSourceRectW(int value);
	void SetSourceRectH(int value);

protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;

	Rectangle _boundingBox;

	float _x, _y;
private:

};