#pragma once
#include "Globals.h"

//forward declare
struct SDL_Texture;
class Graphics;

class Tile {
public:
	Tile();
	Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);
	void Update(int elapsedTime);
	void Draw(Graphics &graphics);
protected:
	Vector2 _position; // position in the map the tile is
	Vector2 _size;
	SDL_Texture * _tileset;
	Vector2 _tilesetPosition; // position in the tile set that the tile is
private:

};