#pragma once
#include "Player.h"
#include "Level.h"
#include "HUD.h"
class Graphics;

class Game
{
public:
	Game();
	~Game();

private:
	void GameLoop();
	void Draw(Graphics &graphics);
	void Update(float elapsedTime);

	Player _player;
	Level _level;
	HUD _hud;
	Graphics _graphics;
};