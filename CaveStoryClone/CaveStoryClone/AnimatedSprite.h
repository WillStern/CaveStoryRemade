#pragma once
#include "Sprite.h"
#include "Globals.h"
#include <string>
#include <map>
#include <vector>

//forward declare
class Graphics;

/*AnimatedSprite class
holds logic for animating our sprites
*/
class AnimatedSprite  : public Sprite
{
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string & filePath, int sourceX, int sourceY, int width, int height, float posX, float posY, float timeToUpdate);


	/*PlayAnimation
	plays information provided its not already playing
	*/
	void PlayAnimation(std::string animation, bool once = false);

	/*Update
	updates the animated sprite
	*/
	void Update(int elapsedTime);

	/*Draw
	Draws the Sprite to the screen
	*/
	void Draw(Graphics &graphics, int x, int y);


protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	std::string _currentAnimation;

	/*AddAnimation
	adds an animation to the map of animations for the sprite
	*/
	void AddAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);

	/*ResetAnimations
	resets all animations associated with this sprite
	*/
	void ResetAnimations();

	/*StopAnimation
	stops the current animation
	*/
	void StopAnimation();

	/*SetVisable
	changes the visibility of the animated sprite
	*/
	void SetVisable(bool visable);

	/*AnimationDone
	Logic that happens when an animation is done
	*/
	virtual void AnimationDone(std::string currentAnimation) = 0;

	/*SetupAnimaitons
	a required function that sets up all animations for a sprite
	*/
	virtual void SetupAnimations() = 0;
private:
	std::map< std::string, std::vector<SDL_Rect> > _animations;
	std::map< std::string, Vector2 > _offsets;

	int _frameIndex;
	double _timeElapsed;
	bool _visable;

};