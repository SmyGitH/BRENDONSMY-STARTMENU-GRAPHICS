#ifndef _LEVEL_LOADER_H
#define _LEVEL_LOADER_H

#include <vector>
#include "Window.h"
#include "Entity.h"
#include "Ball.h"
#include "Mods.h"
#include "Log.h"
#include "GameManager.h"
#include <iostream>
#include <vector>



#define BLOCK_WIDTH 90

class LevelLoader
{
    public:

		LevelLoader(GameManager* gm);
		void buildMap(int getMaxHealth, int& maxBlocks);

    private:
		GameManager* gameManager;
};

#endif
