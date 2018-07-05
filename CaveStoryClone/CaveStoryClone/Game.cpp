#include <SDL/SDL.h>
#include "Game.h"
#include "Graphics.h"
#include "Input.h"

#include <algorithm>

/*Game class
This holds all the info for the main game loop
*/

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 1000 / FPS;
}

Game::Game() 
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->GameLoop();
}

Game::~Game() 
{

}

void Game::GameLoop() 
{
	Graphics graphics;
	Input input;
	SDL_Event event;

	this->_level = Level("Map1-new", graphics);
	this->_player = Player(graphics, _level.GetPlayerSpawnPoint());
	this->_hud = HUD(graphics, this->_player);

	int LAST_UPDATE_TIME = SDL_GetTicks();
	//Start game loop
	while (true) 
	{
		input.BeginNewFrame();

		
		
		if (SDL_PollEvent(&event)) 
		{
			if (event.type == SDL_KEYDOWN) 
			{
				if (event.key.repeat == 0) //makes sure player is not holding down a key
				{
					input.KeyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP)
			{
				input.KeyUpEvent(event);
			}
			else if (event.type == SDL_QUIT)
			{
				return;
			}
		}
		if (input.WasKeyPressed(SDL_SCANCODE_ESCAPE) == true) 
		{
			return;
		}
		else if (input.IsKeyHeld(SDL_SCANCODE_LEFT) == true)
		{
			this->_player.MoveLeft();
		}
		else if (input.IsKeyHeld(SDL_SCANCODE_RIGHT) == true)
		{
			this->_player.MoveRight();
		}
		
		if (input.IsKeyHeld(SDL_SCANCODE_UP) == true)
		{
			this->_player.LookUp();
		}
		else if (input.IsKeyHeld(SDL_SCANCODE_DOWN) == true && !input.IsKeyHeld(SDL_SCANCODE_RIGHT) && !input.IsKeyHeld(SDL_SCANCODE_LEFT))
		{
			this->_player.LookDown();
		}

		if (input.WasKeyReleased(SDL_SCANCODE_UP) == true)
		{
			this->_player.StopLookingUp();
		}

		if (input.WasKeyReleased(SDL_SCANCODE_DOWN) == true)
		{
			this->_player.StopLookingDown();
		}


		if (input.IsKeyHeld(SDL_SCANCODE_Z) == true || input.IsKeyHeld(SDL_SCANCODE_SPACE) == true)
		{
			this->_player.Jump();
		}


		if (!input.IsKeyHeld(SDL_SCANCODE_LEFT) && !input.IsKeyHeld(SDL_SCANCODE_RIGHT))
		{
			this->_player.StopMoving();
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME; //how long current frame took

		this->_graphics = graphics;
		this->Update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->Draw(graphics);

	}
}

void Game::Draw(Graphics &graphics) 
{
	graphics.Clear();

	this->_level.Draw(graphics);
	this->_player.Draw(graphics);


	this->_hud.Draw(graphics);

	graphics.Flip();
}

void Game::Update(float elapsedTime) 
{
	this->_player.Update(elapsedTime);
	this->_level.Update(elapsedTime);

	this->_hud.Update(elapsedTime);
	//check collisions
	std::vector<Rectangle> others;
	if ((others = this->_level.CheckTileCollisions(this->_player.GetBoundingBox())).size() > 0)
	{
	//player collided with at least one tile, handle it
		this->_player.HandleTileCollisions(others);
	}
	//check slopes
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.CheckSlopeCollisions(this->_player.GetBoundingBox())).size() > 0)
	{
		this->_player.HandleSlopeCollisions(otherSlopes);
	}

	//check doors
	std::vector<Door> otherDoors;
	if ((otherDoors = this->_level.CheckDoorCollisions(this->_player.GetBoundingBox())).size() > 0)
	{
		this->_player.HandleDoorCollision(otherDoors, this->_level, this->_graphics);
	}
}