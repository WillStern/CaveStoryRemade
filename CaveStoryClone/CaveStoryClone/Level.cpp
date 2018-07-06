#include "Level.h"
#include "Graphics.h"
#include "Rectangle.h"
#include "Utils.h"
#include "Enemy.h"
#include "Player.h"

#include "tinyxml2.h"

#include <SDL_IMAGE/SDL_image.h>

#include <sstream>
#include <algorithm>
#include <cmath>

using namespace tinyxml2;

Level::Level() 
{

}

Level::Level(std::string mapName, Graphics &graphics) :
	_mapName(mapName),
	_size(Vector2(0,0))
{
	this->LoadMap(mapName, graphics);
}

Level::~Level()
{

}

void Level::LoadMap(std::string mapName, Graphics &graphics)
{
	//TEMPORARY CODE=========================================
	XMLDocument animationDoc;
	std::stringstream ss2;
	//TEMPORARY HACK: DIRECTLY WRITING IN THE TILESET WE NEED BUT WILL NEED TO PASS IT IN SOMEHOW LATER
	ss2 << "Content/TileSets/NpcSym.tsx";
	animationDoc.LoadFile(ss2.str().c_str());
	XMLElement* tilesetNode = animationDoc.FirstChildElement("tileset");
	//TEMP CODE END ========================================

	//parse the .xml file
	XMLDocument doc;
	std::stringstream ss;
	//to work with new version of tiled must copy tmx file and turn into xml, then change the source of the tileset to: Content/TileSets/TemplateName.png (.png not .tsx)
	ss << "Content/Maps/" << mapName << ".xml"; 
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");
	//Get the width and height of the whole map and store in size variable
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//Get width and height of the Tiles and store in tileSize variable
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	//load the tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL)
	{
		while (pTileset)
		{
			int firstgid;
			const char* source = pTileset->Attribute("source"); // was: const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			std::stringstream ss;
			ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.GetRenderer(), graphics.LoadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			//Get animations for the tileset==========================================================================================
			//will only do this if the source for the tileset is the correct one
			if (pTileset->IntAttribute("firstgid") == 81)
			{
				XMLElement* pTileA = tilesetNode->FirstChildElement("tile");
				if (pTileA != NULL)
				{
					while (pTileA)
					{
						AnimatedTileInfo ati;
						ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
						ati.TilesetsFirstGid = firstgid;

						XMLElement* pAnimation = pTileA->FirstChildElement("animation");
						if (pAnimation != NULL)
						{
							while (pAnimation)
							{
								XMLElement* pFrame = pAnimation->FirstChildElement("frame");
								if (pFrame != NULL)
								{
									while (pFrame)
									{
										ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
										ati.Duration = pFrame->IntAttribute("duration");

										pFrame = pFrame->NextSiblingElement("frame");
									}
								}

								pAnimation = pAnimation->NextSiblingElement("animation");
							}
						}
						this->_animatedTileInfos.push_back(ati);
						pTileA = pTileA->NextSiblingElement("tile");
					}
				}
			}

			//========================================================================================== above code differs from tutorial

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	//Load the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL)
	{
		while (pLayer)
		{
			//loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL)
			{
				while (pData)
				{
					//loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL)
					{
						int tileCounter = 0;
						while (pTile)
						{
							//build each individual tile here
							//if gid = 0, no tile should be drawn
							if (pTile->IntAttribute("gid") == 0) 
							{
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}
							}

							//get the tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (unsigned int i=0;i<_tilesets.size();i++)
							{
								if (this->_tilesets[i].FirstGid <= gid)
								{
									if (this->_tilesets.at(i).FirstGid > closest)
									{
										closest = this->_tilesets[i].FirstGid;
										//this is the tileset we want
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.FirstGid == -1)
							{
								//no tileset found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile"))
								{
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else
								{
									break;
								}
							}

							//get the position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							Vector2 finalTilePosition = Vector2(xx, yy);

							//Calculate the position of the tile in the tileset
							Vector2 finalTilesetPosition = this->GetTilesetPosition(tls, gid, tileWidth, tileHeight);

							//Build the tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (unsigned int i=0; i< this->_animatedTileInfos.size(); i++)
							{
								if (this->_animatedTileInfos.at(i).StartTileId == gid)
								{
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile)
							{
								std::vector<Vector2> tilesetPositions;
								for (unsigned int i = 0; i < ati.TileIds.size(); i++)
								{
									tilesetPositions.push_back(this->GetTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
								}
									AnimatedTile tile(tilesetPositions, ati.Duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
									this->_animatedTileList.push_back(tile);
								
							}
							else
							{
								Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
								this->_tileList.push_back(tile);
							}
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL)
	{
		while (pObjectGroup)
		{
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
							(int)(std::ceil(x) * globals::SPRITE_SCALE),
							(int)(std::ceil(y) * globals::SPRITE_SCALE),
							(int)(std::ceil(width) * globals::SPRITE_SCALE),
							(int)(std::ceil(height) * globals::SPRITE_SCALE)
							));

						pObject = pObject->NextSiblingElement("object");
					}
				}

			}
			//other object groups go here with an else if ss.str == "whatever is it"
			else if (ss.str() == "slopes")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2((int)std::ceil(pObject->FloatAttribute("x")), (int)std::ceil(pObject->FloatAttribute("y")));

						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL)
						{
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::Split(ss.str(), pairs, ' ');
							//now we have each of the pairs, loop through pairs and split into Vector2s, then store in points vector
							for (unsigned int i=0; i < pairs.size(); i++)
							{
								std::vector<std::string> ps;
								Utils::Split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (unsigned int i=0; i< points.size(); i += 2)
						{
							this->_slopes.push_back(Slope(
								Vector2(	(p1.x + points.at(i < 2 ? i : i - 1).x) * (int)globals::SPRITE_SCALE,
											(p1.y + points.at(i < 2 ? i : i - 1).y) * (int)globals::SPRITE_SCALE) ,
								Vector2(	(p1.x + points.at(i < 2 ? i + 1 : i).x) * (int)globals::SPRITE_SCALE,
											(p1.y + points.at(i < 2 ? i + 1 : i).y) * (int)globals::SPRITE_SCALE)
							));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			else if (ss.str() == "spawn points")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player")
						{
							this->_spawnPoint = Vector2((int)(std::ceil(x) * globals::SPRITE_SCALE), (int)(std::ceil(y) * globals::SPRITE_SCALE));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//Parse through door objects
			else if (ss.str() == "doors")
			{
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle((int)x, (int)y, (int)w, (int)h);

						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL)
						{
							while (pProperties)
							{
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL)
								{
									while (pProperty)
									{
										const char* name = pProperty->Attribute("name");
										std::stringstream ss;
										ss << name;
										if (ss.str() == "destination")
										{
											const char* value = pProperty->Attribute("value");
											std::stringstream ss2;
											ss2 << value;
											Door door = Door(rect, ss2.str());
											this->_doorList.push_back(door);
										}

										pProperty = pProperty->NextSiblingElement("property");
									}
								}

								pProperties = pProperties->NextSiblingElement("properties");
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			else if (ss.str() == "enemies")
			{
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL)
				{
					while (pObject)
					{
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "bat")
						{
							this->_enemies.push_back(new Bat(graphics, Vector2((int)(std::floor(x) * globals::SPRITE_SCALE), (int)(std::floor(y) * globals::SPRITE_SCALE))));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}


}

void Level::Update(int elapsedTime, Player &player)
{
	for (unsigned int i =0; i< this->_animatedTileList.size(); i++)
	{
		this->_animatedTileList.at(i).Update(elapsedTime);
	}
	for (unsigned int i=0; i<this->_enemies.size(); i++)
	{
		this->_enemies.at(i)->Update(elapsedTime, player);
	}
}

void Level::Draw(Graphics &graphics)
{
	//draw the background
	for (unsigned int i = 0; i < this->_tileList.size(); i++)
	{
		this->_tileList.at(i).Draw(graphics);
	}
	for (unsigned int i =0; i< this->_animatedTileList.size(); i++)
	{
		this->_animatedTileList.at(i).Draw(graphics);
	}
	for (unsigned int i = 0; i < this->_enemies.size(); i++)
	{
		this->_enemies.at(i)->Draw(graphics);
	}
}

std::vector<Rectangle> Level::CheckTileCollisions(const Rectangle &other)
{
	std::vector<Rectangle> others;
	for (unsigned int i =0; i< this->_collisionRects.size(); i++)
	{
		if (this->_collisionRects.at(i).CollidesWith(other))
		{
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

std::vector<Slope> Level::CheckSlopeCollisions(const Rectangle &other)
{
	std::vector<Slope> others;
	for (unsigned int i =0; i<this->_slopes.size(); i++)
	{
		if (this->_slopes.at(i).CollidesWith(other)) 
		{
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::CheckDoorCollisions(const Rectangle &other)
{
	std::vector<Door> others;
	for (unsigned int i = 0; i < this->_doorList.size(); i++)
	{
		if (this->_doorList.at(i).CollidesWith(other))
		{
			others.push_back(this->_doorList.at(i));
		}
	}
	return others;
}

std::vector<Enemy*> Level::CheckEnemyCollisions(const Rectangle &other)
{
	std::vector<Enemy*> others;
	for (unsigned int i = 0; i < this->_enemies.size(); i++)
	{
		if (this->_enemies.at(i)->GetBoundingBox().CollidesWith(other))
		{
			others.push_back(this->_enemies.at(i));
		}
	}
	return others;
}

//========================== NEEDS WORK =============================================
//need to modify this in order to allow for multiple player spawn points on one map
//could add a custom property to spawn points in tiled that clarifies where player is coming from,
//then parse out those properties and use them in this function
//SIDE NOTE: could change map 2 so the door at the top is a tile down, so as not to be covered by HUD
const Vector2 Level::GetPlayerSpawnPoint() const 
{
	std::stringstream ss;
	ss << this->_mapName << "\n";
	printf(ss.str().c_str());
	return _spawnPoint;
}

Vector2 Level::GetTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight)
{
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = 0;
	int tsyy = 0;
	tsxx = (gid - 1) % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}