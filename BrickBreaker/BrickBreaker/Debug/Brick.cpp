#include "Brick.h"


Brick::Brick(Window* window, const std::string& textureName, int xPos, int yPos, int maxHealth) :
Entity(window, textureName, xPos, yPos)
{
    this->typeId = TYPEID_BRICK;

	this->maxHealth = maxHealth;
    health = maxHealth;
	
}
void Brick::update() 
{
	
	window->renderTexture(texture, xPos, yPos);
}



bool Brick::dealDamage(int dmg)
{
    health -= dmg;


	if (health == 2) {
		setTexture("yellowBrick.png");
	}
		
	if (health == 1) {
		setTexture("greenBrick.png");
	}

    if (health <= 0)
    {
        remove();
        return false;
    }

    return true;
}
