#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <fstream>
#include "ghost.h"
#include "player.h"
using namespace std;

class level
{
public:
	level(string);
	~level();

	struct theDots
	{
		bool eaten = false;
		SDL_Rect dotRect;
		int rowTile, colTile;
	};

	void renderMap(SDL_Renderer*, player&);
	int** getMapArr();
	bool finishLevel() { return levelWin; }

	friend SDL_Texture* createTexture(string);

private:
	ifstream inputFile;
	int mapRows, mapColumns;
	int **mapArr;
	char trash;
	int totalDots;
	int dotsCount;
	theDots* dotsArray;
	bool levelWin;
	ghost* enemyArr;
	int enemyCount;

	SDL_Texture* blocks;
	SDL_Texture* dots;
	SDL_Rect	renderQuad;
};

