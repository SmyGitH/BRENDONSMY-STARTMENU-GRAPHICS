#include "Graphics.h"

namespace SDLFramework {
	Graphics* Graphics::sInstance = nullptr;
	bool Graphics::sInitialized = false;

	Graphics* Graphics::Instance() {
		if (sInstance == nullptr) {
			sInstance = new Graphics();
		}

		return sInstance;
	}

	bool Graphics::Initialized() {
		return sInitialized;
	}

	void Graphics::DrawTexture(SDL_Texture* tex, SDL_Rect* srcRect, SDL_Rect* dstRect, float angle, SDL_RendererFlip flip) {
		SDL_RenderCopyEx(mRenderer, tex, srcRect, dstRect, angle, nullptr, flip);
	}

	SDL_Texture* Graphics::CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color) {
		SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
		if (surface == nullptr) {
			std::cerr << "CreateTextTexture:: TTF_RenderText_Solid Error: " << TTF_GetError () << std::endl;
			return nullptr;
		}
		SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (tex == nullptr) {
			std::cerr << "CreateTextTexture:: SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
			return nullptr;
		}
		SDL_FreeSurface(surface);
		return tex;
	}

	SDL_Texture* Graphics::LoadTexture(std::string path) {
		SDL_Texture* tex = nullptr;

		SDL_Surface* surface = IMG_Load(path.c_str());

		if (surface == nullptr) {
			std::cerr << "Unable to Load" << path << ". IMG Error: " << IMG_GetError() << std::endl;

			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(mRenderer, surface);

		if (tex == nullptr) {
			std::cerr << "Unable to Create texture from surface! IMG Error: " << IMG_GetError() << std::endl;
			
			return nullptr;
		}

		SDL_FreeSurface(surface);

		return tex;
	}

	void Graphics::Release() {
		delete sInstance;
		sInstance = nullptr;
	}

	void Graphics::ClearBackBuffer() {
		SDL_RenderClear(mRenderer);

	}

	void Graphics::Render() {
		SDL_RenderPresent(mRenderer);


	}

	Graphics::Graphics() : mRenderer(nullptr) {
		sInitialized = Init();

	}

	Graphics::~Graphics() {
		SDL_DestroyRenderer(mRenderer);
		SDL_DestroyWindow(mWindow);
	}

	bool Graphics::Init() {
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
			std::cerr << "Unable to initialize SDL! SDL Error: " << SDL_GetError() << std::endl;
			return false;
		}

		mWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

		if (mWindow == nullptr) {
			std::cerr << "Unable to create Window! SDL Error: " << SDL_GetError() << std::endl;

			return false;
		}

		mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);

		if (mRenderer == nullptr) {
			std::cerr << "Unable to Create renderer! SDL Error: " << SDL_GetError() << std::endl;

			return false;
		}

		//Temp Background White
		//SDL_SetRenderDrawColor(mRenderer, 0xff, 0xff, 0xff, 0xff);

		if (TTF_Init() == -1) {
			std::cerr << "Unable to initialized SDL_ttf! TTF Error: " << TTF_GetError() << std::endl;
			return false;
		}

		return true;
	}

	

}

