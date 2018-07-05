#pragma once
#include "AnimatedSprite.h"
#include "Slope.h"
#include "Door.h"
#include "Level.h"
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

	const float GetX() const;
	const float GetY() const;

	const inline int GetMaxHealth() const { return this->_maxHealth; }
	const inline int GetCurrentHealth() const { return this->_currentHealth; }
private:
	float _dx, _dy; //change in x and y positions
	
	Direction _facing;

	bool _grounded;

	bool _lookingUp;
	bool _lookingDown;

	int _maxHealth;
	int _currentHealth;
};