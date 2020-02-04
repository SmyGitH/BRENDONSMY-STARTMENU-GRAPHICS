#ifndef _MODS_H
#define _MODS_H

#include <iostream>

#include "Entity.h"
#include "Log.h"


class Mods : public Entity
{
    public:
        Mods(Window* window, const std::string& textureName, int xPos, int yPos);

        
        virtual void update();
        void fastPaddle();
        void slowerPaddle();
		void largePaddle();
		void smallPaddle();

    private:
        int xVelocity;  //left and right velocity
        int yVelocity;  //up and down velocity
};

#endif
