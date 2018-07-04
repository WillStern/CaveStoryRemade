#include "Input.h"

/*Input class
keeps track of keyboard state
*/

//function gets called at begining of each new frame to reset keys that are no longer relevant
void Input::BeginNewFrame() {
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();
}

//called when a key is pressed
void Input::KeyDownEvent(const SDL_Event &event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}

//called when a key is released
void Input::KeyUpEvent(const SDL_Event &event) {
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false;
}

//check if a key was pressed during the current frame
bool Input::WasKeyPressed(SDL_Scancode key) {
	return this->_pressedKeys[key];
}

//check if a key was released during the current frame
bool Input::WasKeyReleased(SDL_Scancode key) {
	return this->_releasedKeys[key];
}

//check if a key is currently begin held
bool Input::IsKeyHeld(SDL_Scancode key) {
	return this->_heldKeys[key];
}

