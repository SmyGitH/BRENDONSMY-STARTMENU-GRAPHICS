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
    gameBgTexture = window->loadTexture("bgNoTitle.png");
    htpTexture = window->loadTexture("HowToPlay.png");

    currentLevel = 1;
    totalBricksDestroyed = 0;

    powerupTimer = 9999;
    powerUpActive = false;
}

void GameManager::initGame(bool fresh)
{
    paddle = new Entity(window, "paddle.png", 305, 490);
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
    mod = new Mods(window, "PowerUP.png", window->getWidth() / 2, 0 );//makes a new power down object

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

    buildMap();

    if (currentState == STATE_PLAYING)
    {
       
        Log::info("Loaded level " + std::to_string(currentLevel) + " with " + std::to_string(maxBricks) + " blocks.");
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

    //Game States
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

    //
  
    switch (event.type)
    {
        // if user clicks the red X
    case SDL_QUIT:
        _quit = true;
        break;

        //Paddle Movement
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
        case SDLK_a:
            controlAI();
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

   
    
    BrickDamage();
    PowerUp();


    if (levelOver)
    {
        // totalBricksDestroyed += maxBricks;
        currentLevel++;
        initGame(false);
        levelOver = false;
        return;
    }


    if (bricksLeft == 0)
    {
        levelOver = true;
        totalBricksDestroyed += maxBricks;
    }

    if (currentLevel > 3) {
        currentState = STATE_WINNER;
    }

    
    //Game Over Screen
    if (ball->getLives() < 1)
    {

        window->renderCenteredText("GAME OVER", window->getHeight() / 4, { 0,0,0 }, 50, FONT_RENDER_BLENDED, { 255,255,255 });
        window->renderCenteredText("Score: " + std::to_string(calcScore()), window->getHeight() / 2, { 0,0,0 }, 50, FONT_RENDER_BLENDED, { 255,255,255 });
        listenForQuit();
        return;
    }


   

    if (ball->collidedWith(paddle))
    {
        ball->handleCollision(paddle);
    }
    paddle->update();

    ball->update();
    window->renderText("Lives: " + std::to_string(ball->getLives()), 5, 0, { 0, 0, 0 }, 25, FONT_RENDER_BLENDED, { 0, 0, 0 });
    window->renderText("Score: " + std::to_string(calcScore()), window->getWidth() - 100, 0, { 0, 0, 0 }, 25, FONT_RENDER_BLENDED, { 0, 0, 0 });

   
}



//Adding brick into vector
void GameManager::addEntity(Entity* e)
{
	entities.push_back(e);
}

//Game State
void GameManager::setState(int state)
{
    Log::info("Set state to " + std::to_string(state));
    currentState = state;
}

//Print Credits
void GameManager::printCredits()
{
    window->renderCenteredText("Brendon Smy", 100, { 0, 0, 0 }, 45, FONT_RENDER_BLENDED, { 0, 0, 0 });
   
}

//Quitting the game
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

//Optimization 1 Wasn't happy with my original printing of the bricks so I chose to improve upon my early mistakes
void GameManager::buildMap()
{
    int blockCount = 0;
    int health = 0;
    int ypos = 30;
    int xpos = 5;
    maxBricks = 0;

    //setting up level 1
    if (currentLevel == 1) {
        health = 1;

        for (int i = 0; i < 7; i++) {
            addEntity(new Brick(getWindow(), "greenBrick.png", xpos, ypos, health));
            xpos += 90;
            maxBricks++;
        }
    }


    //setting up level 2
    if (currentLevel == 2) {
        health = 2;
        for (int j = 0; j < 2; j++) {
            for (int i = 0; i < 7; i++) {
                addEntity(new Brick(getWindow(), "yellowBrick.png", xpos, ypos, health));
                xpos += 90;
                maxBricks++;
            }
            ypos += 30;
            xpos = 5;
        }

    }

    //setting up level 3
    if (currentLevel == 3) {
        health = 3;
        for (int j = 0; j < 3; j++) {
            for (int i = 0; i < 7; i++) {
                addEntity(new Brick(getWindow(), "redBrick.png", xpos, ypos, health));
                xpos += 90;
                maxBricks++;
            }
            ypos += 30;
            xpos = 5;
        }
    }
}

//Optimization 2 Wanted to make the Game Tick cleaner and more efficient
void GameManager::BrickDamage() {

    //Collision Damage
    bool collidedThisTick = false;
    
    for (Entity* e : entities)
    {

        if (e->isActive())
        {

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
}

//Optimization 3 Chose to clean up the Game Tick even further and is running much smoother after removing power ups and brick damage
void GameManager::PowerUp() {

    powerupTimer++;

    if (randNum == 0 && isPressed == true && !powerUpActive)
    {
        mod->update();
        if (mod->collidedWith(paddle))
        {
            powerupTimer = 0;
            mod->fastPaddle();
            paddle->setMoveRate(10);
            mod->remove();
            powerUpActive = true;
        }
    }

    if (randNum == 1 && isPressed == true && !powerUpActive)
    {
        mod->update();
        if (mod->collidedWith(paddle))
        {
            powerupTimer = 0;
            mod->slowerPaddle();
            paddle->setMoveRate(6);
            mod->remove();
            powerUpActive = true;
        }
    }

    if (randNum == 2 && isPressed == true && !powerUpActive)
    {
        mod->update();
        if (mod->collidedWith(paddle))
        {
            powerupTimer = 0;
            paddle->setTexture("paddle_big.png");
            mod->largePaddle();
            mod->remove();
            powerUpActive = true;
        }
    }

    if (randNum == 3 && isPressed == true && !powerUpActive)
    {
        mod->update();
        if (mod->collidedWith(paddle))
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
}

//AI movement


void GameManager::controlAI()
{

    if (ball->isOnPaddle()) {
        ball->detach();
        isPressed = true;
    }
    
    
   paddle->startMoving(ball->getX());
    
   
   // paddle->startMoving(MOVE_RIGHT);
       
}

//Calculating score
int GameManager::calcScore()
{
    return (totalBricksDestroyed);
}
