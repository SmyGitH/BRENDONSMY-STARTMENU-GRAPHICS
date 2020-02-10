#include <iostream>
#include <time.h>
#include <array>
#include "Brick.h"
#include "GameManager.h"
#include "Log.h"
#include "Menu.h"
#include "Timer.h"
#include "levelLoader.h"

GameManager::GameManager(Window* window):
    window(window)
{
    currentState = STATE_MENU;
    _quit = false;
    srand(time(NULL));

    bgTexture = window->loadTexture("bg.png");
    //gameBgTexture = window->loadTexture("bgNoTitle.png");
    htpTexture = window->loadTexture("HowToPlay.png");

    paddle = new Entity(window, "paddle.png", 305, 490);
	

    currentLevel = 1;
    bricksLeft = 0;
    maxBricks = 0;
    totalBricksDestroyed = 0;
	//printBrickX[0] = {30,120,210,300,390};
	//printBrickY[0] = {30,60,90};

    powerupTimer = 3;
    powerUpActive = false;
}

void GameManager::initGame(bool fresh)
{
    paddle->setMoveRate(8);
    paddle->setTexture("paddle.png");
    paddle->setX(305);
    paddle->setY(490);
    paddle->stopMoving(MOVE_LEFT);
    paddle->stopMoving(MOVE_RIGHT);
    ball = new Ball(window, "ball.png", window->getWidth() / 2, window->getHeight() / 2, paddle);
    ball->setOnPaddle(true);
	

    //used for random powerup spwaning
    randNum = rand() % 4;
    mod = new Mods(window, "PowerUP.png", window->getWidth() / 2, 0 );

    isPressed = false;

    powerUpActive = false;

    upNum = rand() % 2;
    downNum = rand() % 2;

    entities = std::vector<Entity*>();

    if (fresh)
    {
        currentLevel = 1;
        totalBricksDestroyed = 0;
    } 

    //LevelLoader* loader = new LevelLoader(this);

    if (currentState == STATE_PLAYING)
    {
        Log::info("Loaded level " + std::to_string(currentLevel) + " with " + std::to_string(maxBricks) + " blocks.");
		addEntity(brick = new Brick(window, "redBrick.png", 30, 100, 3));
		addEntity(brick = new Brick(window, "redBrick.png", 120, 100, 3));
		addEntity(brick = new Brick(window, "redBrick.png", 210, 100, 3));
		addEntity(brick = new Brick(window, "redBrick.png", 300, 100, 3));
		addEntity(brick = new Brick(window, "redBrick.png", 390, 100, 3));
		addEntity(brick = new Brick(window, "redBrick.png", 480, 100, 3));
        bricksLeft = maxBricks;
        levelOver = false;
    }
}

void GameManager::runGame()
{
    Menu mainMenu(this);
    mainMenu.addEntry("Play");
    mainMenu.addEntry("How to Play");
    mainMenu.addEntry("Credits");
    mainMenu.addEntry("Exit");

    Timer fpsTimer;
    Timer capTimer;

    uint32_t frameCount = 0;
    fpsTimer.start();

    while (!_quit)
    {
        window->clear();

        capTimer.start();

        switch (currentState)
        {
        case STATE_MENU:
        {
            window->renderTexture(bgTexture, 0, 0);
            mainMenu.tick();
            break;
        }
        case STATE_HOWTOPLAY:
        {
            window->renderTexture(htpTexture, 0, 0);
            listenForQuit();
            break;
        }
        case STATE_CREDITS:
        {
            window->renderTexture(bgTexture, 0, 0);
            printCredits();
            listenForQuit();
            break;
        }
        case STATE_PLAYING:
            window->renderTexture(gameBgTexture, 0, 0);
            gameTick();
            break;
        case STATE_WINNER:
        {
            window->renderTexture(bgTexture, 0, 0);
            window->renderCenteredText("YOU WON!", 100, { 0, 0, 0 }, 75, FONT_RENDER_BLENDED, { 0, 0, 0 });
            window->renderCenteredText("Final Score: " + std::to_string(calcScore()), 180, { 0, 0, 0 }, 75, FONT_RENDER_BLENDED, { 0, 0, 0 });
            listenForQuit();
            break;
        }

        default:
            Log::warn("Recieved unhandled gamestate: " + std::to_string(currentState));
            break;
        }

        window->render();

        frameCount++;

        // if our fps it too high, wait here until we're back to ~60fps
        if (capTimer.getTicks() < (1000 / window->getMaxFps()))
        {
            int waitTime = (1000 / window->getMaxFps()) - capTimer.getTicks();
            SDL_Delay(waitTime);
        }
    }
}

void GameManager::gameTick()
{
    bool repeatKey = SDL_PollEvent(&event) == 1;
	

    if(ball->getLives() < 1)
    {
        //totalBricksDestroyed += maxBricks - bricksLeft;
        
        window->renderCenteredText("GAME OVER", window->getHeight()/4, {0,0,0}, 50, FONT_RENDER_BLENDED, {255,255,255});
        window->renderCenteredText("Score: " + std::to_string(calcScore()), window->getHeight()/2, {0,0,0}, 50, FONT_RENDER_BLENDED, {255,255,255});
        listenForQuit();
        return;
    }

    switch (event.type)
    {
    // if user clicks the red X
    case SDL_QUIT:
        _quit = true;
        break;

    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            paddle->startMoving(MOVE_LEFT);
            break;
        case SDLK_RIGHT:
            paddle->startMoving(MOVE_RIGHT);
            break;
        case SDLK_SPACE:
            if (ball->isOnPaddle())
                ball->detach();
            isPressed = true;
            break;
        case SDLK_ESCAPE:
            if (repeatKey)
            {
                setState(STATE_MENU);
                return;
            }
        }
        break;

    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_LEFT:
            paddle->stopMoving(MOVE_LEFT);
            break;
        case SDLK_RIGHT:
            paddle->stopMoving(MOVE_RIGHT);
            break;
        }
        break;
    }

	bool collidedThisTick = false;
    for (Entity* e : entities)
    {
      
        if (e->isActive())
        {
            // brick damage
            if ((ball->collidedWith(e)) && (e->isActive()) && !collidedThisTick)
            {
                collidedThisTick = true;
                ball->handleCollision(e);
                if (e->getTypeId() == TYPEID_BRICK)
                {
                    if (!((Brick*)e)->dealDamage(1))
                    {
                        bricksLeft--;
                        totalBricksDestroyed++;
                       
                    
                    }
                    Log::info(std::to_string(bricksLeft) + " / " + std::to_string(maxBricks) + " bricks remaining");
                }

              
            }
            e->update();
        }
    }

	//Power ups
    powerupTimer++;
    
    if(randNum == 0 && isPressed == true && !powerUpActive)
    {
        mod->update();
        if(mod->collidedWith(paddle))
        {
            powerupTimer = 0;
            mod->fastPaddle();
            paddle->setMoveRate(16);
            mod->remove();
            powerUpActive = true;
        }
    }

    if(randNum == 1 && isPressed == true && !powerUpActive)
    {
		{
			mod->update();
			if (mod->collidedWith(paddle))
			{
				powerupTimer = 0;
				mod->slowerPaddle();
				paddle->setMoveRate(4);
				mod->remove();
				powerUpActive = true;
			}
		}
    }

    if(randNum == 2 && isPressed == true && !powerUpActive)
    {
        mod->update();
        if(mod->collidedWith(paddle))
        {
            powerupTimer = 0;
            paddle->setTexture("paddle_big.png");
            mod->largePaddle();
            mod->remove();
            powerUpActive = true;
        }
    }

    if(randNum == 3 && isPressed == true && !powerUpActive)
    {
        mod->update();
        if(mod->collidedWith(paddle))
        {
            powerupTimer = 0;
            paddle->setTexture("paddle_small.png");
            mod->smallPaddle();
            mod->remove();
            powerUpActive = true;
        }
    }
    uint8_t c = powerupTimer;
    if (randNum == 0 && powerupTimer < 254)
    {
        window->renderCenteredText("PADDLE SPEED INCREASED!", 300, { c,c,c }, 30, FONT_RENDER_BLENDED, { 100,100,100 });
    }
    if (randNum == 1 && powerupTimer < 254)
    {
        window->renderCenteredText("PADDLE SPEED DECREASED!", 300, { c,c,c }, 30, FONT_RENDER_BLENDED, { 100,100,100 });
    }
    if (randNum == 2 && powerupTimer < 254)
    {
        window->renderCenteredText("PADDLE SIZE INCREASED!", 300, { c,c,c }, 30, FONT_RENDER_BLENDED, { 100,100,100 });
    }
    if (randNum == 3 && powerupTimer < 254)
    {
        window->renderCenteredText("PADDLE SIZE DECREASED!", 300, { c,c,c }, 30, FONT_RENDER_BLENDED, { 100,100,100 });
    }

    if (ball->collidedWith(paddle))
    {
        ball->handleCollision(paddle);
    }
    paddle->update();

    ball->update();
    window->renderText("Lives: " + std::to_string(ball->getLives()), 5, 0, { 0, 0, 0 }, 25, FONT_RENDER_BLENDED, { 0, 0, 0 });
    window->renderText("Score: " + std::to_string(calcScore()), window->getWidth() - 100, 0, { 0, 0, 0 }, 25, FONT_RENDER_BLENDED, { 0, 0, 0 });

    if (bricksLeft <= 0)
    {
        levelOver = true;
       // totalBricksDestroyed += maxBricks;
    }
}

void GameManager::addEntity(Entity* e)
{
	entities.push_back(e);
}

void GameManager::setState(int state)
{
    Log::info("Set state to " + std::to_string(state));
    currentState = state;
}

void GameManager::printCredits()
{
    window->renderCenteredText("Brendon Smy", 100, { 0, 0, 0 }, 45, FONT_RENDER_BLENDED, { 0, 0, 0 });
   
}

void GameManager::listenForQuit()
{
    SDL_Event currEvent;
    bool repeatKey = SDL_PollEvent(&currEvent) == 1;
    
    switch (currEvent.type)
    {
    // if user clicks the red X
    case SDL_QUIT:
        _quit = true;
        return;
    case SDL_KEYDOWN:
        if (repeatKey)
        {
            switch (currEvent.key.keysym.sym)
            {
            case SDLK_SPACE:
            case SDLK_RETURN:
            case SDLK_ESCAPE:
                setState(STATE_MENU);
            }
        }
    }
}

int GameManager::calcScore()
{
    return (totalBricksDestroyed);
}
