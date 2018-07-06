#include "Enemy.h"

Enemy::Enemy() {}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, (float)spawnPoint.x, (float)spawnPoint.y, (float)timeToUpdate),
	_direction(LEFT),
	_maxHealth(0),
	_currentHealth(0)
{

}

void Enemy::Update(int elapsedTime, Player &player)
{
	AnimatedSprite::Update(elapsedTime);
}

void Enemy::Draw(Graphics &graphics)
{
	AnimatedSprite::Draw(graphics, (int)this->_x, (int)this->_y);
}



//Bat Class
Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "Content/Sprites/NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX((float)spawnPoint.x),
	_startingY((float)spawnPoint.y),
	_shouldMoveUp(false)
{
	this->SetupAnimations();
	this->PlayAnimation("FlyLeft");
}

void Bat::Update(int elapsedTime, Player &player)
{
	this->_direction = player.GetX() > this->_x ? RIGHT : LEFT;
	this->PlayAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

	//Move Up or Down == right now the bats move a set amount and all together, this could be changed to make the bats feel more natural
	this->_y += this->_shouldMoveUp ? -0.02f : 0.02f;
	if (this->_y > (this->_startingY + 20) || this->_y < (this->_startingY - 20))
	{
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}

	Enemy::Update(elapsedTime, player);
}

void Bat::Draw(Graphics &graphics)
{
	Enemy::Draw(graphics);
}

//===============NEEDS WORK===================
//currently the players health is drained immediately upon hitting a bat
//this is because it subtracts 1 health every frame
//need to add invincibility frames in order to prevent this
void Bat::TouchPlayer(Player* player)
{
	if (player->GetCurrentHealth() > 0)
	{
		if (!player->IsInvincible())
		{
			player->GainHealth(-1);
		}
	}
}

void Bat::AnimationDone(std::string currentAnimation)
{
}

void Bat::SetupAnimations() 
{
	this->AddAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->AddAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}