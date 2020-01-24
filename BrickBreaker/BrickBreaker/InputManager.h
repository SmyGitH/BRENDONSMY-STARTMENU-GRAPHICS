#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include <SDL.h>
#include "MathHelper.h"
#include <string>

namespace SDLFramework {
	class InputManager {
	public:
		enum MouseButton {LEFT = 0, RIGHT, MIDDLE, BACK, FORWARD};


	private:
		static InputManager* sInstance;

		const Uint8* mKeyboardState;
		Uint8* mPrevKeyboardState;
		int mKeyLength;

		Uint32 mPrevMouseState;
		Uint32 mMouseState;

		int mMouseXPos;
		int mMouseYPos;

	public:
		static InputManager* Instance();
		static void Release();

		bool KeyDown(SDL_Scancode scancode);
		bool KeyPressed(SDL_Scancode scancode);
		bool KeyReleased(SDL_Scancode scancode);

		bool MouseButtonDown(MouseButton button);
		bool MouseButtonPressed(MouseButton button);
		bool MouseButtonReleased(MouseButton button);

		Vector2 MousePosition();

		void Update();
		void UpdatePrevInput();

	private:
		InputManager();
		~InputManager();

	};
}

#endif