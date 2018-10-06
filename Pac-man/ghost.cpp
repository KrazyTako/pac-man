#include "ghost.h"

ghost::ghost()
{
	ghostImage = createTexture("redGhost.png");
	srand(time(NULL));
	randNum = rand() % 5;
	xPos = 320;
	yPos = 270;
	speed = 3;
	frame = 0;
	maxMoveFrames = 0;
	moveLeft = false, moveRight = false, moveLeft = false, moveDown = false, inMotion = false;
}


ghost::~ghost()
{
}

void ghost::resetGhost()
{
	xPos = 320;
	yPos = 270;
}

void ghost::ghostMove(int** map)
{
	if(!inMotion)
	{ 
		inMotion = true;
		randNum = rand() % 4;
		maxMoveFrames = rand() % 25 + 20;
		if (randNum == 0)
		{
			moveLeft = true;
		}
			else if (randNum == 1)
			{
				moveRight = true;
			}
				else if (randNum == 2)
				{
					moveUp = true;
				}
				else if (randNum == 3)
				{
					moveDown = true;
				}
	}

	if (inMotion)
	{
		ghostTileRow = yPos / 32;
		ghostTileCol = xPos / 32;
		if (moveLeft)
		{
			if (frame >= maxMoveFrames)
			{
				moveLeft = false;
				inMotion = false;
				frame = 0;
			}
			else
			{
				tempX = (xPos - speed) / 32;
				tempY = (yPos + 25) / 32;
				if (map[ghostTileRow][tempX] == 1 || map[tempY][tempX])
				{
					moveLeft = false;
					inMotion = false;
					frame = 0;
				}
				else
				{
					xPos -= speed;
				}
			}
		}
			else if (moveRight)
			{
				if (frame >= maxMoveFrames)
				{
					moveRight = false;
					inMotion = false;
					frame = 0;
				}
				else
				{
					tempX = (xPos + speed + 25) / 32;
					tempY = (yPos + 25) / 32;
					if (map[ghostTileRow][tempX] == 1 || map[tempY][tempX] == 1)
					{
						moveRight = false;
						inMotion = false;
						frame = 0;
					}
					else
					{
						xPos += speed;
					}
				}
			}
				else if (moveUp)
				{
					if (frame >= maxMoveFrames)
					{
						moveUp = false;
						inMotion = false;
						frame = 0;
					}
					else
					{
						tempY = (yPos - speed) / 32;
						tempX = (xPos + 25) / 32;
						if (map[tempY][ghostTileCol] == 1 || map[tempY][tempX])
						{
							moveUp = false;
							inMotion = false;
							frame = 0;
						}
						else
						{
							yPos -= speed;
						}
					}
				}
					else if (moveDown)
					{
						if (frame >= maxMoveFrames)
						{
							moveDown = false;
							inMotion = false;
							frame = 0;
						}
						else
						{
							tempY = (yPos + speed + 25) / 32;
							tempX = (xPos + 25) / 32;
							if (map[tempY][ghostTileCol] == 1 || map[tempY][tempX] == 1)
							{
								moveDown = false;
								inMotion = false;
								frame = 0;
							}
							else
							{
								yPos += speed;
							}
						}
					}
	}
	if (xPos > 710)
	{
		xPos = 400;
	}
}

void ghost::renderGhost(SDL_Renderer* gRenderer, int** map, player p1)
{
	ghostMove(map);
	frame++;
	renderQuad = { xPos, yPos, 25, 25 };
	SDL_RenderCopy(gRenderer, ghostImage, NULL, &renderQuad);

}