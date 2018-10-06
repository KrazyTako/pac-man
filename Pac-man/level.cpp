#include "level.h"
#include <SDL_rect.h>

level::level(string fileName)
{
	//Opens level file
	inputFile.open(fileName.c_str());
	if (!inputFile.is_open())
	{
		cout << "could not open level file" << endl;
		SDL_Delay(5000);
	}
	
	//Reads map array size
	inputFile >> mapRows;
	inputFile >> trash;
	inputFile >> mapColumns;
	inputFile >> trash;
	inputFile >> enemyCount;

	//creates array of enemies
	enemyArr = new ghost[enemyCount];

	//allocates memory for 2d array
	mapArr = new int*[mapRows];
	for (int i = 0; i < mapRows; i++)
	{
		mapArr[i] = new int[mapColumns];
	}

	//stores array and counts dots
	for (int i = 0; i < mapRows;i++)
	{
		for (int j = 0; j < mapColumns;j++)
		{
			inputFile >> mapArr[i][j];
			if (mapArr[i][j] == 0)
			{
				totalDots++;
			}
			inputFile >> trash;
		}
	}
	
	//allocate memory for dots and assigns there positions
	dotsArray = new theDots[totalDots];
	dotsCount = totalDots;
	int dotIndex = 0;
	for (int i = 0;i < mapRows;i++)
	{
		for (int j = 0;j < mapColumns;j++)
		{
			if (mapArr[i][j] == 0)
			{
				dotsArray[dotIndex].dotRect = { (32 * j) + 16 - 5, (32 * i) + 16 - 5, 10, 10 };
				dotsArray[dotIndex].rowTile = i;
				dotsArray[dotIndex].colTile = j;
				dotIndex++;
			}
		}
	}

	//loads textures for blocks and dots
	blocks = createTexture("blueBlock.png");
	dots = createTexture("dot.png");
	levelWin = false;
}

level::~level()
{
	delete mapArr;
	delete dotsArray;
}

int** level::getMapArr()
{
	return mapArr;
}

void level::renderMap(SDL_Renderer* gRenderer, player& p1)
{
	//Dots being eaten
	for (int i = 0;i < totalDots;i++)
	{
		if (!dotsArray[i].eaten)
		{
			if (SDL_HasIntersection(&p1.getPlayerRect(), &dotsArray[i].dotRect))
			{
				dotsArray[i].eaten = true;
				dotsCount--;
				p1.playChompSound();
				if (dotsCount <= 0)
					levelWin = true;
			}
		}
	}

	//Draws map layout
	for (int i = 0; i < mapRows; i++)
	{
		for (int j = 0;j < mapColumns; j++)
		{
			if (mapArr[i][j] == 1)
			{
				renderQuad = { 32 * j, 32 * i, 32, 32 };
				SDL_RenderCopy(gRenderer, blocks, NULL, &renderQuad);
			}
		}
	}

	//Draws dots
	for (int i = 0; i < totalDots; i++)
	{
		if (!dotsArray[i].eaten)
			SDL_RenderCopy(gRenderer, dots, NULL, &dotsArray[i].dotRect);
	}

	//render ghosts and check for collision between player
	for (int i = 0;i < enemyCount;i++)
	{
		enemyArr[i].renderGhost(gRenderer, mapArr, p1);
		if (SDL_HasIntersection(&enemyArr[i].getRect(), &p1.getPlayerRect()))
		{
			p1.setToDeath();
			p1.playDeathSound();
			for (int i = 0;i < enemyCount;i++)
			{
				enemyArr[i].resetGhost();
			}
		}
	}
}