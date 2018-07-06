#include "Tile.h"
#include "Globals.h"
#include "Graphics.h"

#include <SDL/SDL.h>

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x * (int)globals::SPRITE_SCALE, position.y * (int)globals::SPRITE_SCALE))
{
}

void Tile::Update(int elapsedTime) {}

void Tile::Draw(Graphics &graphics)
{
	SDL_Rect destRect = { this->_position.x, this->_position.y, 
		this->_size.x * (int)globals::SPRITE_SCALE, this->_size.y * (int)globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };

	graphics.BlitSurface(this->_tileset, &sourceRect, &destRect);
}