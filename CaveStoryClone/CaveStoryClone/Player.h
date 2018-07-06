#pragma once
#include "AnimatedSprite.h"
#include "Slope.h"
#include "Door.h"
#include "Level.h"
#include "Enemy.h"
//forward declare
class Graphics;

class Player : public AnimatedSprite{
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void Draw(Graphics &graphics);
	void Update(float elapsedTime);

	/*MoveLeft
	moves the player left by -dx
	*/
	void MoveLeft();
	
	/*MoveRight
	moves the player right by dx
	*/
	void MoveRight();
	
	/*StopMoving
	stops moving the player
	*/
	void StopMoving();

	/*LookUp
	Player looks up
	*/
	void LookUp();

	/*StopLookingUp
	player stops looking up
	*/
	void StopLookingUp();

	/*LookDown
	player looks down OR interacts with an object by turning around 
	*/
	void LookDown();

	/*StopLookingDown
	player stops looking down OR stops interacting
	*/
	void StopLookingDown();


	//Jump
	//starts jumping
	void Jump();

	virtual void AnimationDone(std::string currentAnimation);
	virtual void SetupAnimations();

	void HandleTileCollisions(std::vector<Rectangle> &others);
	void HandleSlopeCollisions(std::vector<Slope> &others);
	void HandleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics);
	void HandleEnemyCollision(std::vector<Enemy*> &others);
	//=================== NEEDS WORK =======================
	//add a HandleItemCollision function to check the given list of items
	//then have the player react according to the type of item


	const float GetX() const;
	const float GetY() const;

	const inline int GetMaxHealth() const { return this->_maxHealth; }
	const inline int GetCurrentHealth() const { return this->_currentHealth; }
	const inline Direction IsFacing() const { return this->_facing; }
	const inline int IsInvincible() const { return this->_isInvincible; }
	inline void IsInvincible(bool value) { this->_isInvincible = value; }

	void GainHealth(int amt);
private:
	float _dx, _dy; //change in x and y positions
	
	Direction _facing;

	bool _grounded;

	bool _lookingUp;
	bool _lookingDown;
	bool _isInvincible;

	int _maxHealth;
	int _currentHealth;
};