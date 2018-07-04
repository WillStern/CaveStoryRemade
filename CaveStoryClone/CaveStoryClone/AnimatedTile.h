#pragma once
#include "Tile.h"
#include "Globals.h"

#include <vector>

class AnimatedTile : public Tile {
public:
	AnimatedTile();
	AnimatedTile(std::vector<Vector2> tilesetPositions, int duration, SDL_Texture* tileset, Vector2 size, Vector2 position);
	void Update(int elapsedTime);
	void Draw(Graphics &graphics);
protected:
	int _amountOfTime = 0;
	bool _notDone = false;

private:
	std::vector<Vector2> _tilesetPositions;
	int _tileToDraw;
	int _duration;

};

struct AnimatedTileInfo
{
public:
	int TilesetsFirstGid;
	int StartTileId;
	std::vector<int> TileIds;
	int Duration;

};