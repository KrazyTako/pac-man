#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "player.h"
using namespace std;

class ghost
{
public:
	ghost();
	~ghost();

	void ghostMove(int** map);
	void renderGhost(SDL_Renderer*, int** map, player p1);
	void resetGhost();
	SDL_Rect getRect() { return renderQuad; }

	friend SDL_Texture* createTexture(string);

private:
	int xPos, yPos;
	int speed;
	int frame, maxMoveFrames;
	int randNum;
	int ghostTileRow, ghostTileCol, tempX, tempY;
	bool moveLeft, moveRight, moveUp, moveDown, inMotion;
	SDL_Rect currentClip;
	SDL_Rect renderQuad;
	SDL_Texture* ghostImage;

};

