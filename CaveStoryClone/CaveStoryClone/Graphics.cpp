#include "Graphics.h"
#include <SDL/SDL.h>
#include <SDL_IMAGE/SDL_image.h>
#include "Globals.h"

/*
Graphics class
Holds all info for dealing with graphics for the game
*/
Graphics::Graphics() 
{
	//Cave Story runs in 640 by 480
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Cave Story");

}

Graphics::~Graphics() 
{
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::LoadImage(const std::string &filePath) 
{
	if(this->_spriteSheets.count(filePath) == 0)
	{
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

void Graphics::BlitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle)
{
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::Flip()
{
	SDL_RenderPresent(this->_renderer);
}

void Graphics::Clear()
{
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::GetRenderer() const
{
	return this->_renderer;
}