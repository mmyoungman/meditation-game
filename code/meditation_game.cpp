#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "mmy.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

SDL_Window *window;
SDL_Renderer *renderer = NULL;

// Game State
enum gs
{
    PERSON_FRONT,
    PERSON_SITTING_A,
    PERSON_SITTING_B,
    TITLE_SCREEN,
    GAME_FRONT,
    GAME_TURNING,
    GAME_WALKING
};

// Title Message
enum titlem
{
    HOLD_SPACE,
    TOO_MUCH_SPACE,
    TOO_LITTLE_SPACE
};

struct sprite_sheet {
    int numFrames;
    int currentFrame;
    int w;
    int h;
    SDL_Texture* texture;
};

sprite_sheet loadSpriteSheet(const char* pathToImg, int numFrames, int width, int height)
{
    sprite_sheet result;
    result.numFrames = numFrames;
    result.currentFrame = 0;

    result.w = width;
    result.h = height;

    SDL_Surface* temp = IMG_Load(pathToImg);
    result.texture = SDL_CreateTextureFromSurface(renderer, temp);

    return result;
}

void drawSpriteClip(sprite_sheet* sp, int posX, int posY)
{
    SDL_Rect clip = { 0, sp->currentFrame * sp->h, sp->w, sp->h };
    SDL_Rect rend = { posX, posY, clip.w, clip.h };
    SDL_RenderCopy(renderer, sp->texture, &clip, &rend);

    sp->currentFrame++;
    if(sp->currentFrame >= sp->numFrames)
        sp->currentFrame = 0;
}

void drawSpriteClip(sprite_sheet* sp, int posX, int posY, int alpha)
{
    SDL_Rect clip = { 0, sp->currentFrame * sp->h, sp->w, sp->h };
    SDL_Rect rend = { posX, posY, clip.w, clip.h };
    SDL_SetTextureAlphaMod(sp->texture, alpha);
    SDL_RenderCopy(renderer, sp->texture, &clip, &rend);

    sp->currentFrame++;
    if(sp->currentFrame >= sp->numFrames)
        sp->currentFrame = 0;
}

int main(int argc, char* argv[])
{

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
        return 1;
    else
    {
        window = SDL_CreateWindow("Meditation by Mark Milan", 
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
        if(window == NULL)
            return 1;
        else 
        {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if(renderer == NULL)
                return 1;
            else 
            {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
                    return 1;
            }
        }
    }

    SDL_Surface* tempSurface = NULL;

    sprite_sheet background = loadSpriteSheet("../data/xbackground.png", 4, 1280, 50);
    sprite_sheet personFront = loadSpriteSheet("../data/xpersonfront.png", 12, 419, 377);
    sprite_sheet personSittingA = loadSpriteSheet("../data/xpersonsittinga.png", 21, 419, 377);
    sprite_sheet personSittingB = loadSpriteSheet("../data/xpersonsittingb.png", 21, 419, 377);
    sprite_sheet titleScreen = loadSpriteSheet("../data/xtitlescreen.png", 5, 419, 377);
    sprite_sheet title = loadSpriteSheet("../data/xtitle.png", 5, 419, 171);
    sprite_sheet titleesctoquit = loadSpriteSheet("../data/xtitleesctoquit.png", 5, 419, 98);
    sprite_sheet titlestriving = loadSpriteSheet("../data/xtitlestriving.png", 5, 419, 171);
    sprite_sheet titledistraction = loadSpriteSheet("../data/xtitledistraction.png", 5, 419, 171);
    sprite_sheet titletoomuchspace = loadSpriteSheet("../data/xtitletoomuchspace.png", 5, 419, 98);
    sprite_sheet titletoolittlespace = loadSpriteSheet("../data/xtitletoolittlespace.png", 5, 419, 98);
    sprite_sheet personTurning = loadSpriteSheet("../data/xpersonturning.png", 19, 419, 377);
    sprite_sheet personWalking = loadSpriteSheet("../data/xpersonwalking.png", 21, 419, 377);

    enum gm 
    {
        ICANTMEDITATE,
        IMUSTSTOP,
        ALMONDMILK,
        IMSOSPIRITUAL,
        IMBORED,
        MYSTICALEXPERIENCE,
        UNPLEASANTMEMORY,
        SEXUALIMAGERY,
        THISISWEIRD,
        LAUGHTERTHOUGHT,
        LEFTFOOT,
        SUN,
        CLOUD,
        GREATSADNESS,
        ACTORSNAME,
        DISTRACTED,
        ITCH,
        WRITEDOWN,
        JHANA,
        PROGRESS,
        BLISS,
        WATERPLANTS,
        OVERWHELMING,
        NUM_OF_GAME_MSGS
    };

    sprite_sheet game_msgs[NUM_OF_GAME_MSGS];

    game_msgs[ICANTMEDITATE] = loadSpriteSheet("../data/xicantmeditate.png", 5, 300, 146);
    game_msgs[IMUSTSTOP] = loadSpriteSheet("../data/ximuststop.png", 4, 358, 260);
    game_msgs[ALMONDMILK] = loadSpriteSheet("../data/xalmondmilk.png", 4, 302, 177);
    game_msgs[IMSOSPIRITUAL] = loadSpriteSheet("../data/ximsospiritual.png", 4, 378, 165);
    game_msgs[IMBORED] = loadSpriteSheet("../data/ximbored.png", 4, 340, 130);
    game_msgs[MYSTICALEXPERIENCE] = loadSpriteSheet("../data/xmysticalexperience.png", 4, 291, 177);
    game_msgs[UNPLEASANTMEMORY] = loadSpriteSheet("../data/xunpleasantmemory.png", 4, 321, 156);
    game_msgs[SEXUALIMAGERY] = loadSpriteSheet("../data/xsexualimagery.png", 4, 262, 168);
    game_msgs[THISISWEIRD] = loadSpriteSheet("../data/xthisisweird.png", 4, 298, 177);
    game_msgs[LAUGHTERTHOUGHT] = loadSpriteSheet("../data/xlaughterthought.png", 4, 252, 192);
    game_msgs[LEFTFOOT] = loadSpriteSheet("../data/xleftfoot.png", 4, 424, 164);
    game_msgs[SUN] = loadSpriteSheet("../data/xsun.png", 4, 282, 222);
    game_msgs[CLOUD] = loadSpriteSheet("../data/xcloud.png", 4, 207, 110);
    game_msgs[GREATSADNESS] = loadSpriteSheet("../data/xgreatsadness.png", 4, 322, 156);
    game_msgs[ACTORSNAME] = loadSpriteSheet("../data/xactorsname.png", 4, 270, 207);
    game_msgs[DISTRACTED] = loadSpriteSheet("../data/xdistracted.png", 4, 327, 144);
    game_msgs[ITCH] = loadSpriteSheet("../data/xitch.png", 4, 278, 144);
    game_msgs[WRITEDOWN] = loadSpriteSheet("../data/xwritedown.png", 4, 388, 182);
    game_msgs[JHANA] = loadSpriteSheet("../data/xjhana.png", 4, 408, 140);
    game_msgs[PROGRESS] = loadSpriteSheet("../data/xprogress.png", 4, 290, 136);
    game_msgs[BLISS] = loadSpriteSheet("../data/xbliss.png", 4, 222,124);
    game_msgs[WATERPLANTS] = loadSpriteSheet("../data/xwaterplants.png", 4, 274, 177);
    game_msgs[OVERWHELMING] = loadSpriteSheet("../data/xoverwhelming.png", 4, 314, 147);

    int gameMsgPos = WINDOW_WIDTH;
    int gameMsgPosInc = 5;
    int gameMsgAlpha = 255;
    int game_msg = stb_rand() % NUM_OF_GAME_MSGS; 
    int prevMsg = game_msg;

    gs game_state = PERSON_FRONT;
    titlem title_message = HOLD_SPACE;

    bool gameRunning = true;

    Uint8 gameAlpha = 255;
    int alphaInc = 8;

    bool spaceIsDown;

    SDL_Event e;
    while(gameRunning) 
    {
        while(SDL_PollEvent(&e) != 0) 
        {
            if(e.type == SDL_QUIT) 
            {
                SDL_SetWindowFullscreen(window, 0);
                gameRunning = false;
            }
        }

        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
        if(keyboardState[SDL_SCANCODE_SPACE])
            spaceIsDown = true;
        else
            spaceIsDown = false;

        if(keyboardState[SDL_SCANCODE_ESCAPE] || keyboardState[SDL_SCANCODE_Q])
        {
            SDL_SetWindowFullscreen(window, 0);
            gameRunning = false;
        }

        SDL_RenderClear(renderer);

        drawSpriteClip(&background, 0, 450);

        if(game_state == PERSON_FRONT)
        {
            drawSpriteClip(&personFront, 431, 171);
            if(personFront.currentFrame == 0)
                game_state = PERSON_SITTING_A;
        }

        if(game_state == PERSON_SITTING_A)
        {
            drawSpriteClip(&personSittingA, 431, 171);
            if(personSittingA.currentFrame == 0)
                game_state = PERSON_SITTING_B;
        }

        if(game_state == PERSON_SITTING_B)
        {
            drawSpriteClip(&personSittingB, 431, 171);
            if(personSittingB.currentFrame == 0)
                game_state = TITLE_SCREEN;
        }

        if(game_state == TITLE_SCREEN)
        {
            drawSpriteClip(&titleScreen, 431, 171, gameAlpha);

            if(title_message == HOLD_SPACE)
            {
                drawSpriteClip(&title, 431, 171, gameAlpha);
                drawSpriteClip(&titleesctoquit, 431, 550, gameAlpha);
            }
            else if(title_message == TOO_MUCH_SPACE)
            {
                drawSpriteClip(&titlestriving, 431, 171, gameAlpha);
                drawSpriteClip(&titletoomuchspace, 431, 550, gameAlpha);
            }
            else if(title_message == TOO_LITTLE_SPACE)
            {
                drawSpriteClip(&titledistraction, 431, 171, gameAlpha);
                drawSpriteClip(&titletoolittlespace, 431, 550, gameAlpha);
            }

            if(spaceIsDown && gameAlpha <= alphaInc)
            {
                gameAlpha = 20;
                game_state = GAME_FRONT;
            }
            else if(spaceIsDown && gameAlpha >= alphaInc)
                gameAlpha -= alphaInc;
            else if(!spaceIsDown && gameAlpha <= (255 - alphaInc))
                gameAlpha += alphaInc; 
        }

        if(game_state == GAME_FRONT)
        {
            if(gameAlpha <= alphaInc)
            {
                game_state = TITLE_SCREEN;
                gameAlpha = 255;
            }
            else if(!spaceIsDown && gameAlpha >= alphaInc)
            {
                gameAlpha -= alphaInc;
            }
            else if(spaceIsDown && gameAlpha <= (255 - alphaInc))
                gameAlpha += alphaInc; 
            else if(spaceIsDown && gameAlpha >= (255 - alphaInc))
                game_state = TITLE_SCREEN;

            drawSpriteClip(&personFront, 431, 171, gameAlpha);

            if(personFront.currentFrame == 0) 
            {
                game_state= GAME_TURNING;
                personTurning.currentFrame = 0;
            }
        }

        if(game_state == GAME_TURNING)
        {
            if(gameAlpha <= alphaInc)
            {
                title_message = TOO_LITTLE_SPACE;
                game_state = TITLE_SCREEN;
                gameAlpha = 255;
            }
            else if(!spaceIsDown && gameAlpha >= alphaInc)
            {
                gameAlpha -= alphaInc;
            }
            else if(spaceIsDown && gameAlpha <= (255 - alphaInc))
                gameAlpha += alphaInc; 
            else if(spaceIsDown && gameAlpha >= (255 - alphaInc))
            {
                title_message = TOO_MUCH_SPACE;
                game_state = TITLE_SCREEN;
            }

            drawSpriteClip(&personTurning, 431, 171, gameAlpha);

            if(personTurning.currentFrame == 0)
            {
                game_state = GAME_WALKING;
                personWalking.currentFrame = 0;
            }
        }

        if(game_state == GAME_WALKING)
        {
            if(gameMsgAlpha > 0)
            {
                drawSpriteClip(&game_msgs[game_msg], gameMsgPos, 30, gameMsgAlpha);
                gameMsgAlpha -= 1;
                gameMsgPos -= gameMsgPosInc;
            }
            else
            {
                gameMsgPos = WINDOW_WIDTH;
                gameMsgAlpha = 255;

                prevMsg = game_msg;
                do { game_msg = stb_rand() % NUM_OF_GAME_MSGS; 
                } while(game_msg == prevMsg);
            }

            drawSpriteClip(&personWalking, 431, 171, gameAlpha);

            if(gameAlpha <= alphaInc)
            {
                game_state = TITLE_SCREEN;
                title_message = TOO_LITTLE_SPACE;

                prevMsg = game_msg;
                do { game_msg = stb_rand() % NUM_OF_GAME_MSGS; 
                } while(game_msg == prevMsg);

                gameMsgPos = WINDOW_WIDTH;
                gameMsgAlpha = 255;
                gameAlpha = 255;
            }
            else if(!spaceIsDown && gameAlpha >= alphaInc)
            {
                gameAlpha -= alphaInc;
            }
            else if(spaceIsDown && gameAlpha <= (255 - alphaInc))
                gameAlpha += alphaInc; 
            else if(spaceIsDown && gameAlpha >= (255 - alphaInc))
            {
                game_state = TITLE_SCREEN;
                title_message = TOO_MUCH_SPACE;

                prevMsg = game_msg;
                do { game_msg = stb_rand() % NUM_OF_GAME_MSGS; 
                } while(game_msg == prevMsg);
                gameMsgPos = WINDOW_WIDTH;
                gameMsgAlpha = 255;
            }

        }

        SDL_RenderPresent(renderer);
        //TODO(mark): Make constant i.e. delay = some time - time for tick 
        SDL_Delay(100);
    }

    return 0;
}
