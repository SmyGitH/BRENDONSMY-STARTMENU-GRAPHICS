#include <iostream>
#include <fstream>
#include <string>
#include "levelLoader.h"
#include "Brick.h"


using namespace std;

LevelLoader::LevelLoader(GameManager* gm)
{
	gameManager = gm;
}



// function for building the map
void LevelLoader::buildMap(int getMaxHealth, int& maxBlocks)
{
   /*
	
    int blockCount = 0;
	int health = 3;
	int ypos = 30;
	int xpos = 25;
	
	while (STATE_PLAYING)
	{
		for (int i = 0; blockCount > 0; i++) {

			if (health == 3)
			{
				gameManager->addEntity(new Brick(gameManager->getWindow(), "redBrick.png", xpos, ypos, health));
			}
			if (health == 2) {
				gameManager->addEntity(new Brick(gameManager->getWindow(), "yellowBrick.png", xpos, ypos, health));
				}
				
				
			if (health == 1) {
				gameManager->addEntity(new Brick(gameManager->getWindow(), "greenBrick.png", xpos, ypos, health));
			}
				

			blockCount++;

		}
		

		ypos += 30;

		if (xpos > gameManager->getWindow()->getWidth() - BLOCK_WIDTH)
			break;

		if (ypos > gameManager->getWindow()->getHeight())
			break;

		maxBlocks = blockCount;

		return;
	}
	*/
}

	
   

//void LevelLoader::openMap(int getMaxHealth, int& maxBlocks)