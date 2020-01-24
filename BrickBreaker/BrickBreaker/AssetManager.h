#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H
#include "Graphics.h"
#include <map>
#include <sstream>


namespace SDLFramework {
	class AssetManager {
	private:
		static AssetManager* sInstance;

	public:
		static AssetManager* Instance();
		static void Release();
		SDL_Texture* GetTexture(std::string filename, bool managed = false);
		void DestroyTexture(SDL_Texture* texture);

		SDL_Texture* GetText(std::string text, std::string filename, int size, SDL_Color
			color, bool managed = false);

	private:
		std::map<std::string, SDL_Texture*> mTextures;

		std::map<SDL_Texture*, unsigned> mTextureRefCount;

		std::map<std::string, TTF_Font*> mFonts;
		std::map<std::string, SDL_Texture*> mText;

		TTF_Font* GetFont(std::string filename, int size);

		AssetManager();
		~AssetManager();
	};
}

#endif