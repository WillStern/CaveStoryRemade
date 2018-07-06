#pragma once
#include "AnimatedSprite.h"
#include "Globals.h"
#include "Player.h"

#include <string>

//foreward declare
class Graphics;


class Enemy : public AnimatedSprite
{
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	virtual void Update(int elapsedTime, Player &player);
	virtual void Draw(Graphics &graphics);
	virtual void TouchPlayer(Player* player) = 0;

	const inline int GetMaxHealth() const { return this->_maxHealth;  }
	const inline int GetCurrentHealth() const { return this->_currentHealth;  }
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;

};

class Bat : public Enemy
{
public:
	Bat();
	Bat(Graphics &graphics, Vector2 spawnPoint);
	void Update(int elapsedTime, Player &player);
	void Draw(Graphics &graphics);
	void TouchPlayer(Player* player);

	void AnimationDone(std::string currentAnimation);
	void SetupAnimations();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
};