#ifndef _SCREENMANAGER_H
#define _SCREENMANAGER_H

#include "StartScreen.h"
#include "BackgroundStars.h"
#include "PlayScreen.h"

class ScreenManager {
private:
	static ScreenManager* sInstance;

	enum Screens { START, PLAY };
	Screens mCurrentScreen;

	InputManager* mInput;
	BackgroundStars* mBackgroundStars;
	StartScreen* mStartScreen;
	PlayScreen* mPlayScreen;

public:
	static ScreenManager* Instance();
	static void Release();

	void Update();
	void Render();

private:
	ScreenManager();
	~ScreenManager();
};
#endif // !_SCREENMANAER_H


