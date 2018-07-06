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

	//potential additions (all found in the Textbox.png)
	//ESC to Quit text:			x = 3, y = 132, w = 49, h = 9
	//Yes / No Select Textbox:	x = 152, y = 48, w = 84, h = 32
	//Light Select Hand:		x = 113, y = 90, w = 13, h = 10
	//Darker Select Hand:		x = 129, y = 90, w = 13, h = 10
	//Air Text:					x = 121, y = 80, w = 23, h = 8
	//light exp bar:			x = 0, y = 33, w = 231, h = 5
	//Textbox background:		x = 0, y = 0, w = 244, h = 24
};