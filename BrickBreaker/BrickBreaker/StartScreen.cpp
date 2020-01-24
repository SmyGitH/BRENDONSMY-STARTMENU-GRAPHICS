#include "StartScreen.h"

StartScreen::StartScreen() {
	mTimer = Timer::Instance();
	mInputManager = InputManager::Instance();

	mTopBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f, 80);
	mPlayerOne = new Texture("1UP", "emulogic.ttf", 32, { 200,0,0 });
	mPlayerTwo = new Texture("2UP", "emulogic.ttf", 32, { 200,0,0 });
	mHiScore = new Texture("HI SCORE", "emulogic.ttf", 32, { 200,0,0 });

	mPlayerOneScore = new Scoreboard();
	mPlayerTwoScore = new Scoreboard();
	mTopScore = new Scoreboard();

	mTopBar->Parent(this);
	mPlayerOne->Parent(mTopBar);
	mPlayerTwo->Parent(mTopBar);
	mHiScore->Parent(mTopBar);

	mPlayerOneScore->Parent(mTopBar);
	mPlayerTwoScore->Parent(mTopBar);
	mTopScore->Parent(mTopBar);

	mPlayerOne->Position(-Graphics::SCREEN_WIDTH * 0.35f, 0.0f);
	mPlayerTwo->Position(Graphics::SCREEN_WIDTH * 0.2f, 0.0f);
	mHiScore->Position(-30.0f, 0.0f);

	mPlayerOneScore->Position(-Graphics::SCREEN_WIDTH * 0.23f, 40.0f);
	mPlayerTwoScore->Position(Graphics::SCREEN_WIDTH * 0.32f, 40.0f);
	mTopScore->Position(Graphics::SCREEN_WIDTH * 0.05f, 40.0f);

	mTopScore->Score(645987);

	//Logo entities
	mLogo = new Texture("GalagaLogo.png", 0, 0, 360, 180);
	mAnimatedLogo = new AnimatedTexture("GalagaLogo.png", 0, 0, 360, 180, 3, 0.2f,
		AnimatedTexture::(VERTICAL);

	mLogo->Parent(this);
	mAnimatedLogo->Parent(this);

	mLogo->Position(Graphics::SCREEN_WIDTH * 0.5f,
		Graphics::SCREEN_HEIGHT * 0.32f);

	mAnimatedLogo->Position(Graphics::SCREEN_WIDTH * 0.5f,
		Graphics::SCREEN_HEIGHT * 0.32f);

	//Play mode entities
	mPlayModes = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f,
		Graphics::SCREEN_HEIGHT * 0.55f);

	mOnePlayerMode = new Texture("1 Player ", "emulogic.ttf", 32,
		{ 230,230,230 });

	mTwoPlayerMode = new Texture("2 Players", "emulogic.ttf", 32,
		{ 230,230,230 });

	mCursor = new Texture("cursor.png");

	mCursorStartPos = mCursor->Position(LOCAL);
	mCursorOffset = Vector2(0.0f, 70.0f);
	mSelectedMode = 0;

	mPlayModes->Parent(this);
	mOnePlayerMode->Parent(mPlayModes);
	mTwoPlayerMode->Parent(mPlayModes);
	mCursor->Parent(mPlayModes);

	mOnePlayerMode->Position(0.0f, -35.0f);
	mTwoPlayerMode->Position(0.0f, 35.0f);
	mCursor->Position(-175.0f, -35.0f);

	//Bottom bar entites
	mBottomBar = new GameEntity(Graphics::SCREEN_WIDTH * 0.5f,
		Graphics::SCREEN_HEIGHT * 0.7f);

	mNamco = new Texture("namcot", "namco__.ttf", 36, { 200,0,0 });
	mDates = new Texture("1981 1985 NAMCO LTD.", "emulogic.ttf", 32,
		{ 230,230,230 });
	mRights = new Texture("ALL RIGHTS RESERVED", "emulogic.ttf", 32,
		{ 230,230,230 });

	mBottomBar->Parent(this);
	mNamco->Parent(mBottomBar);
	mDates->Parent(mBottomBar);
	mRights->Parent(mBottomBar);

	mNamco->Position(Vec2_Zero);
	mDates->Position(0.0f, 90.0f);
	mRights->Position(0.0f, 170.0f);

	//Screen Animation Variables
	mAnimationStartPos = Vector2(0.0f, Graphics::SCREEN_HEIGHT);
	mAnimationEndPos = Vec2_Zero;
	mAnimationTotalTime = 5.0f;
	mAnimationTimer = 0;
	mAnimationDone = false;

	Position(mAnimationStartPos);

	ResetAnimation();

	mStars = BackgroundStars::Instance();
	mStars->Scroll(true);
}

StartScreen::~StartScreen() {
	//top bar entities
	delete mTopBar;
	mTopBar = nullptr;

	delete mPlayerOne;
	mPlayerOne = nullptr;

	delete mPlayerTwo;
	mPlayerTwo = nullptr;

	delete mHiScore;
	mHiScore = nullptr;

	delete mPlayerOneScore;
	mPlayerOneScore = nullptr;

	delete mPlayerTwoScore;
	mPlayerTwoScore = nullptr;

	delete mTopScore;
	mTopScore = nullptr;

	mTimer = nullptr;
	mInputManager = nullptr;

	//play mode entities
	delete mPlayModes;
	mPlayModes = nullptr;

	delete mOnePlayerMode;
	mOnePlayerMode = nullptr;

	delete mTwoPlayerMode;
	mTwoPlayerMode = nullptr;

	delete mCursor;
	mCursor = nullptr;

	//Bottom bar entities
	delete mBottomBar;
	mBottomBar = nullptr;

	delete mNamco;
	mNamco = nullptr;

	delete mDates;
	mDates = nullptr;

	delete mRights;
	mRights = nullptr;

	//Logo entities
	delete mLogo;
	mLogo = nullptr;

	delete mAnimatedLogo;
	mAnimatedLogo = nullptr;
}

void StartScreen::Update() {
	if (!mAnimationDone) {
		mAnimationTimer += mTimer->DeltaTime();

		Position(Lerp(mAnimationStartPos, mAnimationEndPos,
			mAnimationTimer / mAnimationTotalTime));

		if (mAnimationTimer >= mAnimationTotalTime) {
			mAnimationDone = true;
		}
	}
	else {
		mAnimatedLogo->Update();

		if (mInputManager->KeyPressed(SDL_SCANCODE_DOWN)) {
			ChangeSelectedMode(1);
		}
		else if (mInputManager->KeyPressed(SDL_SCANCODE_UP)) {
			ChangeSelectedMode(-1);
		}
	}

	if (mInputManager->KeyPressed(SDL_SCANCODE_DOWN) ||
		mInputManager->KeyPressed(SDL_SCANCODE_UP)) {
		mAnimationTimer = mAnimationTotalTime;
	}
}

void StartScreen::ResetAnimation() {
	mAnimationStartPos = Vector2(0.0f, Graphics::SCREEN_HEIGHT);
	mAnimationEndPos = Vec2_Zero;
	mAnimationTotalTime = 5.0f;
	mAnimationTimer = 0.0f;
	mAnimationDone = false;

	Position(mAnimationStartPos);
}

int StartScreen::SelectedMode() {
	return mSelectedMode;
}

void StartScreen::ChangeSelectedMode(int change) {
	mSelectedMode += change;

	if (mSelectedMode < 0) {
		mSelectedMode = 1;
	}
	else if (mSelectedMode > 1) {
		mSelectedMode = 0;
	}

	mCursor->Position(mCursorStartPos + mCursorOffset * (float)mSelectedMode);
}

void StartScreen::Render() {
	mPlayerOne->Render();
	mPlayerTwo->Render();
	mHiScore->Render();
	mPlayerOneScore->Render();
	mPlayerTwoScore->Render();
	mTopScore->Render();

	if (!mAnimationDone) {
		mLogo->Render();
	}
	else {
		mAnimatedLogo->Render();
	}


	mOnePlayerMode->Render();
	mTwoPlayerMode->Render();
	mCursor->Render();

	mNamco->Render();
	mDates->Render();
	mRights->Render();
}