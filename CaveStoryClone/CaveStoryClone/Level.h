#pragma once
#include "Globals.h"
#include "Tile.h"
#include "Rectangle.h"
#include "Slope.h"
#include "AnimatedTile.h"
#include "Door.h"

#include <string>
#include <vector>

//forward declare
class Enemy;
class Graphics;
class Player;
struct SDL_Texture;
struct SDL_Rect;
struct Tileset;

class Level
{
public:
	Level();
	Level(std::string mapName, Graphics &graphics);
	~Level();

	void Update(int elapsedTime, Player &player);
	void Draw(Graphics &graphics);

	std::vector<Rectangle> CheckTileCollisions(const Rectangle &other);
	std::vector<Slope> CheckSlopeCollisions(const Rectangle &other);
	std::vector<Door> CheckDoorCollisions(const Rectangle &other);
	std::vector<Enemy*> CheckEnemyCollisions(const Rectangle &other);

	const Vector2 GetPlayerSpawnPoint() const;
private:
	std::string _mapName;
	Vector2 _spawnPoint;

	Vector2 _size;
	Vector2 _tileSize;


	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	std::vector<Slope> _slopes;

	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfos;

	std::vector<Door> _doorList;

	std::vector<Enemy*> _enemies;

	//============== NEEDS WORK ==============
	//add a _itemList that would contain the items in the level

	/*LoadMap
	Loads a map
	*/
	void LoadMap(std::string mapName, Graphics &graphics);

	Vector2 GetTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};


//Tileset Structure
struct Tileset
{
	SDL_Texture* Texture;
	int FirstGid;

	Tileset()
	{
		this->FirstGid = -1;
	}

	Tileset(SDL_Texture* texture, int firstGid)
	{
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};