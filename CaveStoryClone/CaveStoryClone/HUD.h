#pragma once
#include "Sprite.h"
#include "Player.h"

//forward declare
class Graphics;

class HUD
{
public:
	HUD();
	HUD(Graphics &graphics, Player &player);

	void Update(int elapsedTime, Player &player);
	void Draw(Graphics &graphics);

private:

	Player _player;

	//health sprites
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;

	//EXP sprites
	Sprite _lvWord;
	Sprite _lvNumber;
	Sprite _expBar;

	//Weapon Info
	Sprite _slash;
	Sprite _dashes;

};