#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <SDL_ttf.h>
#include "player.h"
#include "ghost.h"
#include "level.h"
#include "cutscene.h"
#include "menu.h"
using namespace std;

const int windowWidth = 640, windowHeight = 704;
SDL_Renderer *gRenderer = NULL;

enum gameState
{
	GAME_RUNNING,
	GAME_PAUSE,
	GAME_CUTSCENE,
	GAME_MENU
};

SDL_Texture* createTexture(string);
void displayError(string);
void init(SDL_Window* &window, SDL_Renderer* &renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		displayError("SDL_INIT_VIDEO");

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		cout << "SDL_image error" << IMG_GetError() << endl;
	}
	
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "SDL_Mixer error" << Mix_GetError() << endl;
	}
	
	if (TTF_Init() == -1)
	{
		cout << "TTF_Init error " << TTF_GetError() << endl;
	}

	window = SDL_CreateWindow("Pac-man!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
	if (window == NULL)
		displayError("SDL_CreateWindow");

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == NULL)
		displayError("SDL_CreateRenderer");
}

int main(int argc, char* args[])
{
	//window, renderer, and other variables
	SDL_Window *gWindow = NULL;
	SDL_Event ev;
	bool quit = false;
	string levelName[5] = { "level1.txt", "level2.txt", "level3.txt", "level4.txt" };
	string cutSceneName[5] = { "cutScene1.txt", "cutScene2.txt", "cutScene3.txt", "cutScene4.txt" };
	int thisLevel = 0;
	gameState currentGameState = GAME_RUNNING;

	//initializes SDL and sets up a window and it's renderer.
	init(gWindow, gRenderer);

	//map and player init
	Mix_Chunk* beginningSound = Mix_LoadWAV("pacman_beginning.wav");
	if (beginningSound == NULL)
	{
		cout << "didnt load beginning sound: " << Mix_GetError() << endl;
	}
	player player1;
	level* currentLevel = new level(levelName[thisLevel]);
	cutscene* currentCutscene = new cutscene(cutSceneName[thisLevel]);
	menu* currentMenu = new menu(gRenderer);

	Mix_PlayChannel(-1, beginningSound, 0);
	while (!quit)
	{
		//handles input from the player
		while (SDL_PollEvent(&ev) != 0)
		{
			if (ev.type == SDL_QUIT)
				quit = true;
			if (!player1.playerHit())
			{
				player1.playerInput(ev);
			}
			//pauses game when 'p' is pressed
			if (ev.type == SDL_KEYDOWN)
			{
				if (ev.key.keysym.sym == SDLK_p)
					if (currentGameState == GAME_PAUSE)
					{
						currentGameState = GAME_RUNNING;
					}
					else
					{
						currentGameState = GAME_PAUSE;
					}
			}
		}

		switch (currentGameState)
		{
		case GAME_RUNNING:
			//functions that render the current level to the scrren
			if (!player1.playerHit())
			{
				//if player is alive, render the map and player
				SDL_RenderClear(gRenderer);
				currentLevel->renderMap(gRenderer, player1);
				player1.renderPlayer(gRenderer, currentLevel->getMapArr());
				SDL_RenderPresent(gRenderer);
				if (currentLevel->finishLevel())
				{
					delete currentLevel;
					thisLevel++;
					currentLevel = new level(levelName[thisLevel]);
					currentCutscene = new cutscene(cutSceneName[thisLevel]);
					player1.resetPosition();
					SDL_Delay(2000);
					currentGameState = GAME_CUTSCENE;
				}
			}
			else
			{
				//if player is dead, render the death animations
				player1.playerDeath(gRenderer);
				SDL_RenderPresent(gRenderer);
			}
			break;

		case GAME_CUTSCENE:
			if (!currentCutscene->sceneComplete)
			{
				SDL_RenderClear(gRenderer);
				currentCutscene->renderScene(gRenderer);
				SDL_RenderPresent(gRenderer);
			}
			else
			{
				delete currentCutscene;
				currentGameState = GAME_RUNNING;
			}
			break;

		case GAME_MENU:
			SDL_RenderClear(gRenderer);
			currentMenu->renderMenu(gRenderer);
			SDL_RenderPresent(gRenderer);
			break;

		case GAME_PAUSE:
			//pauses game when 'p' is pressed
			break;
		}
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	return 0;
}

SDL_Texture* createTexture(string fileName)
{
	SDL_Texture *finalTexture = NULL;
	SDL_Surface *tempSurface = NULL;
	tempSurface = IMG_Load(fileName.c_str());
	if (tempSurface == NULL)
		displayError("IMG_Load");

	SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0, 0));

	finalTexture = SDL_CreateTextureFromSurface(gRenderer, tempSurface);
	if (finalTexture == NULL)
		displayError("SDL_CreateTextureFromSurface");

	SDL_FreeSurface(tempSurface);

	return finalTexture;
}
void displayError(string msg)
{
	cout << msg << " Error: " << SDL_GetError() << endl;
	SDL_Delay(5000);
	SDL_Quit();
}