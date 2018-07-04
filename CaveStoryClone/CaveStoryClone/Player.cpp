#include "Player.h"
#include "Graphics.h"

namespace player_constants
{
	const float WALK_SPEED = 0.2f;
	const float JUMP_SPEED = 0.4f;
	const float GRAVITY = 0.001f;
	const float GRAVITY_CAP = 0.5f;
}

Player::Player() {};

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, "Content/Sprites/MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_lookingDown(false),
	_lookingUp(false),
	_maxHealth(3),
	_currentHealth(3)
{
	graphics.LoadImage("Content/Sprites/MyChar.png");

	this->SetupAnimations();
	this->PlayAnimation("IdleRight");
}

void Player::SetupAnimations()
{
	this->AddAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->AddAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0));

 	this->AddAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->AddAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));

	this->AddAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->AddAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));

	this->AddAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->AddAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));

	this->AddAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->AddAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));

	this->AddAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->AddAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
}

void Player::AnimationDone(std::string currentAnimation)
{

}

const float Player::GetX() const {
	return this->_x;
}

const float Player::GetY() const {
	return this->_y;
}

void Player::MoveLeft()
{
	if (this->_lookingDown == true && this->_grounded == true)
	{
		return;
	}
	this->_dx = -player_constants::WALK_SPEED;
	if (this->_lookingUp == false)
	{
		this->PlayAnimation("RunLeft");
	}

	this->_facing = LEFT;
}

void Player::MoveRight()
{
	if (this->_lookingDown == true && this->_grounded == true)
	{
		return;
	}
	this->_dx = player_constants::WALK_SPEED;
	if (this->_lookingUp == false)
	{	
		this->PlayAnimation("RunRight");
	}

	this->_facing = RIGHT;
}

void Player::StopMoving()
{
	this->_dx = 0;
	if (this->_lookingUp == false && this->_lookingDown == false)
	{
		this->PlayAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft");
	}
}

void Player::LookUp()
{
	this->_lookingUp = true;
	if (this->_dx == 0)
	{
		this->PlayAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else
	{
		this->PlayAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::StopLookingUp()
{
	this->_lookingUp = false;
}

void Player::LookDown()
{
	this->_lookingDown = true;
	if (this->_grounded)
	{
		//need to interact
		this->PlayAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else
	{
		this->PlayAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::StopLookingDown()
{
	this->_lookingDown = false;
}

void Player::Jump()
{
	if (this->_grounded)
	{
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_SPEED;
		this->_grounded = false;
	}
}


//handles collisions with ALL tiles that the player is colliding with
void Player::HandleTileCollisions(std::vector<Rectangle> &others)
{
	//figure out what side the collision happened on and move player accordingly
	for (int i=0; i < others.size(); i++)
	{
		sides::Side collisionSide = Sprite::GetCollisionSide(others.at(i));
		if (collisionSide != sides::NONE)
		{
			switch (collisionSide)
			{
			case sides::TOP:
				this->_dy = 0;
				this->_y = others.at(i).GetBottom() + 1;
				if (this->_grounded)
				{
					this->_dx = 0;
					this->_x -= this->_facing == RIGHT ? 1.0f : -1.0f;
				}
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).GetTop() - this->_boundingBox.GetHeight() - 1;
				this->_dy = 0;
				this->_grounded = true;
				break;
			case sides::LEFT:
				this->_x = others.at(i).GetRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).GetLeft() - this->_boundingBox.GetWidth() - 1;
				break;
			}

		}
	}
}

//Handle slope collisions
//handles collisions with all slopes player is colliding with
void Player::HandleSlopeCollisions(std::vector<Slope> &others)
{
	for (int i=0; i<others.size(); i++)
	{
		//Calculate where on the slope the players bottom center is touching
		//use y= mx + b to figure out y position to place them
		//calculate b (slope intercept) by using one of the points (b = y - mx)
		int b = others.at(i).GetP1().y - (others.at(i).GetSlope() * fabs(others.at(i).GetP1().x));

		//then get the players center x
		int centerX = this->_boundingBox.GetCenterX();

		//pass that x into the equation to get the new y position
		int newY = (others.at(i).GetSlope() * centerX) + b - 8; //8 is a temp fix to a problem

		//reposition player to correct y
		if (this->_grounded)
		{
			this->_y = newY - this->_boundingBox.GetHeight();
			this->_grounded = true;
		}
	}
}

void Player::Update(float elapsedTime)
{
	//apply gravity
	if (this->_dy < player_constants::GRAVITY_CAP)
	{
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	//Move by dx
	this->_x += this->_dx * elapsedTime;
	//move by dy
	this->_y += this->_dy * elapsedTime;

	AnimatedSprite::Update(elapsedTime);
}

void Player::Draw(Graphics &graphics)
{
	AnimatedSprite::Draw(graphics, this->_x, this->_y);
}