#include "GameManager.h"
#include "AnimatedTexture.h"

namespace SDLFramework {
	GameManager* GameManager::sInstance = nullptr;

	GameManager* GameManager::Instance() {
		if (sInstance == nullptr) {
			sInstance = new GameManager();
		}

		return sInstance;
	}

	void GameManager::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	void GameManager::Run() {
		//main Game loop
		while (!mQuit) {
			mTimer->Update();
			//event processing loop
			while (SDL_PollEvent(&mEvent)) {
				switch (mEvent.type) {
				case SDL_QUIT:
					mQuit = true;
					break;
				}
			}
			if (mTimer->DeltaTime() >= 1.0f / FRAME_RATE) {
				Update();
				LateUpdate();
				Render();
				mTimer->Reset();
			}
			
		}
	}

	void GameManager::Update() {
		mInputManager->Update();
		mScreenManager->Update();		
	}

	void GameManager::LateUpdate() {
		mInputManager->UpdatePrevInput();
	}

	GameManager::GameManager() : mQuit(false) {
		//creat Graphics singleton
		mGraphics = Graphics::Instance();

		//verify Graphics module is ready
		if (!Graphics::Initialized()) {
			mQuit = true;
		}

		mAssetManager = AssetManager::Instance();
	
		
		mInputManager = InputManager::Instance();
		mTimer = Timer::Instance();

		
		mScreenManager = ScreenManager::Instance();
		
		
	}

	GameManager::~GameManager() {
		//release modules
		Graphics::Release();
		mGraphics = nullptr;
		Timer::Release();
		mTimer = nullptr;

		ScreenManager::Release();
		mScreenManager = nullptr;

		BackgroundStars::Release();
		mStars = nullptr;

		InputManager::Release();
		mInputManager = nullptr;

		AssetManager::Release();
		mAssetManager = nullptr;

		//quit SDL
		SDL_Quit();
	}

	void GameManager::Render() {

		mGraphics->ClearBackBuffer();
		mScreenManager->Render();
		mGraphics->Render();

	}
}