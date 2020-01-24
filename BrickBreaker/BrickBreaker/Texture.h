#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "Graphics.h"
#include "GameEntity.h"
#include "AssetManager.h"

namespace SDLFramework {
	class Texture : public GameEntity {
	private:
		SDL_Texture* mTex;
		Graphics* mGraphics;

		

		bool mClipped;
		SDL_Rect mDestinationRect;

	protected:
		int mWidth;
		int mHeight;
		SDL_Rect mSourceRect;

	public:
		Texture(std::string filename, bool managed = false);
		Texture(std::string filename, int x, int y, int w, int h, bool managed = false);
		Texture(std::string text, std::string fontPath, int size, SDL_Color color, bool managed = false);
		~Texture();

		Vector2 ScaleDimensions();

		void Render() override;
	};
}

#endif //Texture
