#include <cmath>

#include "Ball.h"
#include "Brick.h"
#include "Log.h"


Ball::Ball(Window* window, const std::string& textureName, int xPos, int yPos, Entity* linkedPaddle) :
    Entity(window, textureName, xPos, yPos)
{
    this->typeId = TYPEID_BALL;
    this->linkedPaddle = linkedPaddle;
    lives = 3;
    yVelocity = 0;
    xVelocity = 0;
}

void Ball::update()
{
    // don't render the ball if there are no lives left
    if (lives <= 0)
        return;

    // if the ball is still on the paddle, make it move in sync with the paddle
    if (onPaddle)
    {
        xPos = linkedPaddle->getX() + (linkedPaddle->getWidth()/2) - getWidth()/2;
        yPos = linkedPaddle->getY() - getHeight();
        window->renderTexture(texture, xPos, yPos);
        return;
    }

    // first move the ball according to its current velocity
    xPos += xVelocity;
    yPos += yVelocity;

    // then we check to see if the ball is somewhere that it shouldn't be
    // if it is, correct that

    // if the ball goes off the left side of the screen
    if (xPos < 0)
    {
        xPos = 0;
        xVelocity = -xVelocity;
    }

    // if the ball goes off the right side of the screen
	if(xPos > window->getWidth() - getWidth())
	{
		xPos = window->getWidth() - getWidth();
		xVelocity = -xVelocity;
	}

    // if the ball goes off the top of the screen
    if (yPos < 0)
    {
        yPos = 0;
        yVelocity = -yVelocity;
    }

    // if the ball goes off the bottom of the screen
    if (yPos > window->getHeight() - height)
    {
        lives--;
        Log::info("Lives left: " + std::to_string(lives));
        setOnPaddle(true);
    }

    // actually render the ball at its new position to the screen
    window->renderTexture(texture, xPos, yPos);
}

void Ball::handleCollision(Entity* entity)
{
    if (!entity->isActive())
        return;

    Log::info("Ball hit something!");

	double totalSpeed = sqrt(xVelocity*xVelocity + yVelocity*yVelocity);

    double ballCenter = xPos + (width / 2);
    double entityCenter = entity->getX() + (entity->getWidth() / 2);

	if (entity->getTypeId() == TYPEID_BRICK)
	{
		double checkRight = std::abs(entity->getX() - (xPos + width));
		double checkLeft = std::abs((entity->getX() + entity->getWidth()) - xPos );
		double checkHorizontal;
		if (checkLeft < checkRight)
		{
			checkHorizontal = checkLeft;
		}
		else
		{
			checkHorizontal = checkRight;
		}

		double checkUp = std::abs((entity->getY() + entity->getHeight()) - yPos);
		double checkDown = std::abs(entity->getY() - (yPos + height));
		double checkVertical;
		if (checkUp < checkDown)
		{
			checkVertical = checkUp;
		}
		else
		{
			checkVertical = checkDown;
		}

		if (checkHorizontal > checkVertical)
			yVelocity = -yVelocity;
		else if (std::abs(checkHorizontal - checkVertical)<1.2)
		{
			yVelocity = -yVelocity;
			xVelocity = -xVelocity;
		}
		else
			xVelocity = -xVelocity;

	}

	else
	{
		double horizontalDifference = ballCenter - entityCenter;
		totalSpeed += 1;
		if (totalSpeed > 10)
			totalSpeed = 10;
		double w = entity->getWidth()/2.0;
		double angle = horizontalDifference / w;
		xVelocity = angle*totalSpeed*.8;
		yVelocity = 0 - sqrt(totalSpeed*totalSpeed - xVelocity*xVelocity);
		if (std::abs(yVelocity) < .8)
			yVelocity = -1;
	}
}

void Ball::setOnPaddle(bool apply)
{
    if (apply)
        Log::info("Placed ball on paddle.");
    else
        Log::info("Freed ball from paddle.");

    onPaddle = apply;
}

void Ball::setLives(int count)
{
    lives = count;
    Log::info("Set new life count to " + std::to_string(count));
}

void Ball::detach()
{
    Log::info("Detached ball from paddle.");
    onPaddle = false;

    // launch the ball in the direction the paddle is moving
    xVelocity = linkedPaddle->isMoving(MOVE_LEFT) ? -5 : 5;
    yVelocity = -5;
}
