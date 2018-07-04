#pragma once
#include <map>
#include <string>
#include <SDL_IMAGE/SDL_image.h>
#include <SDL/SDL.h>

//forward declare:
struct SDL_Window;
struct SDL_Renderer;

/*
Graphics class
Holds all info for dealing with graphics for the game
*/
class Graphics 
{
public:
	Graphics();
	~Graphics();

	/*LoadImage
	Loads an image into the sprite sheets map if it doesn't already exist.
	Returns the image from the map regardless of whether or not it was just loaded
	*/
	SDL_Surface* LoadImage(const std::string &filePath);

	/*BlitSurface
	Draws a texture to a certain part of screen
	*/
	void BlitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);

	/*Flip
	Renders everything to screen
	*/
	void Flip();

	/*Clear
	clears the screen
	*/
	void Clear();

	/*GetRenderer
	returns the renderer
	*/
	SDL_Renderer* GetRenderer() const;
private:
	SDL_Window * _window;
	SDL_Renderer* _renderer;

	std::map<std::string, SDL_Surface*> _spriteSheets;
};