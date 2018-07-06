#include "HUD.h"
#include "Graphics.h"
#include "Player.h"

HUD::HUD() {}

HUD::HUD(Graphics &graphics, Player &player)
{
	this->_player = player;
	this->_healthBarSprite = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 40, 64, 8, 35, 70);
	this->_healthNumber1 = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 56, 8, 8, 66, 70);
	this->_currentHealthBar = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 25, 39, 5, 83, 72);

	this->_lvWord = Sprite(graphics, "Content/Sprites/TextBox.png", 81, 81, 11, 7, 38, 55);
	this->_lvNumber = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 56, 8, 8, 66, 52);
	this->_expBar = Sprite(graphics, "Content/Sprites/TextBox.png", 0, 72, 40, 8, 83, 52);

	this->_slash = Sprite(graphics, "Content/Sprites/TextBox.png", 72, 48, 8, 8, 100, 36);
	this->_dashes = Sprite(graphics, "Content/Sprites/TextBox.png", 81, 51, 15, 11, 132, 26);
}

void HUD::Update(int elapsedTime, Player &player)
{
	this->_player = player;
	this->_healthNumber1.SetSourceRectX(8 * this->_player.GetCurrentHealth());

	//calculate width of health bar (39 width is max)
	float num = (float)this->_player.GetCurrentHealth() / (float)this->_player.GetMaxHealth();
	this->_currentHealthBar.SetSourceRectW((int)std::floor(num * 39));
}

void HUD::Draw(Graphics &graphics)
{
	this->_healthBarSprite.Draw(graphics, (int)this->_healthBarSprite.GetX(), (int)this->_healthBarSprite.GetY());
	this->_healthNumber1.Draw(graphics, (int)this->_healthNumber1.GetX(), (int)this->_healthNumber1.GetY());
	this->_currentHealthBar.Draw(graphics, (int)this->_currentHealthBar.GetX(), (int)this->_currentHealthBar.GetY());
	this->_lvWord.Draw(graphics, (int)this->_lvWord.GetX(), (int)this->_lvWord.GetY());
	this->_lvNumber.Draw(graphics, (int)this->_lvNumber.GetX(), (int)this->_lvNumber.GetY());
	this->_expBar.Draw(graphics, (int)this->_expBar.GetX(), (int)this->_expBar.GetY());
	this->_slash.Draw(graphics, (int)this->_slash.GetX(), (int)this->_slash.GetY());
	this->_dashes.Draw(graphics, (int)this->_dashes.GetX(), (int)this->_dashes.GetY());
}