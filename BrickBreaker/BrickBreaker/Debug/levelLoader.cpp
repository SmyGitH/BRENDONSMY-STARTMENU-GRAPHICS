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

#define BLOCK_WIDTH 90

// function for openeing and reading the file containing the map
void LevelLoader::openMap(const std::string& mapName, int& maxBlocks)
{
   

    int blockCount = 10;
	int health = 3;
	int ypos = 30;
	int xpos = 25;
	
	while (health > 0)
	{
		if (health == 3)
			gameManager->addEntity(new Brick(gameManager->getWindow(), "redBrick.png", xpos, ypos, health));
		if (health == 2)
			gameManager->addEntity(new Brick(gameManager->getWindow(), "yellowBrick.png", xpos, ypos, health));
		if (health == 1)
			gameManager->addEntity(new Brick(gameManager->getWindow(), "greenBrick.png", xpos, ypos, health));

		blockCount++;

		ypos += 30;

		if (xpos > gameManager->getWindow()->getWidth() - BLOCK_WIDTH)
			break;

		if (ypos > gameManager->getWindow()->getHeight())
			break;

		maxBlocks = blockCount;

		return;
	}

}

	
   
	


//void LevelLoader::openMap(const std::string& mapName, int& maxBlocks)
//void LevelLoader::openMap(int getMaxHealth, int& maxBlocks)