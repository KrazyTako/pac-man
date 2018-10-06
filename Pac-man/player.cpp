#include "player.h"

player::player()
{
	playerImage = createTexture("wakaSheet.png");
	playerDyingImage = createTexture("dyingWaka.png");
	health = 5;
	speed = 3;
	xVel = 0;
	yVel = 0;
	xPos = 400;
	yPos = 515;
	rotationAngle = 0.0;
	frame = 0;
	deathFrame = 0;
	flip = SDL_FLIP_NONE;
	keyUp = false, keyDown = false, keyRight = false, keyLeft = false;
	isHit = false;
	spriteClips[0] = { 0, 0, 25,25 };
	spriteClips[1] = { 25, 0, 25, 25 };
	spriteClips[2] = { 50, 0, 25, 25 };
	spriteClips[3] = { 0, 25, 25, 25 };
	spriteClips[4] = { 25, 25, 25, 25 };
	spriteClips[5] = { 50, 25, 25, 25 };
	renderingQuad = { xPos, yPos, 25, 25 };

	chompSound = Mix_LoadWAV("pacman_chompNEW.wav");
	deathSound = Mix_LoadWAV("pacman_death.wav");
}

player::~player()
{
}

void player::updatePosition(int** map)
{
	int tempX, tempY;
	xVel = 0;
	yVel = 0;
	rowPlayerTile = yPos / 32;
	colPlayerTile = xPos / 32;

	//checks for collision on the walls of the map
	if (keyDown)						
	{							
		yVel += speed;
		tempY = (yPos + speed + 25)/32;
		tempX = (xPos + 25)/32;
		if (map[tempY][colPlayerTile] == 1 || map[tempY][tempX] == 1)
		{ 
			yVel = 0;
		}
	}
	if (keyUp)
	{
		yVel -= speed;
		tempY = (yPos + yVel) / 32;
		tempX = (xPos + 25) / 32;
		if (map[tempY][colPlayerTile] == 1 || map[tempY][tempX])
		{
			yVel = 0;
		}

	}
	if (keyRight)
	{
		xVel += speed;
		tempX = (xPos + xVel + 25) / 32;								
		tempY = (yPos + 25) / 32;
		if (map[rowPlayerTile][tempX] == 1 || map[tempY][tempX] == 1)
		{
			if (rowPlayerTile == 8 && tempX == 20)	
			{
				//compensates for the weird going through right wall thing
			}
			else
				xVel = 0;
		}
	}
	if (keyLeft)
	{
		xVel -= speed;
		tempX = (xPos + xVel)/32;
		tempY = (yPos + 25) / 32;
		if (map[rowPlayerTile][tempX] == 1 || map[tempY][tempX])
			xVel = 0;
	}
	xPos += xVel;
	yPos += yVel;

	//Allows player to go through portal to switch sides
	if ((xPos + 25) < 0)
	{											
		xPos = 640;									
	}												
	if (xPos > 640)									
	{												
		xPos = -25;									
	}
}
void player::playerInput(SDL_Event ev)
{
	if (ev.type == SDL_KEYDOWN)
	{
		switch (ev.key.keysym.sym)
		{
		case SDLK_UP:
			keyUp = true;
			rotationAngle = 90.0;
			flip = SDL_FLIP_NONE;
			break;
		case SDLK_DOWN:
			keyDown = true;
			rotationAngle = -90.0;
			flip = SDL_FLIP_NONE;
			break;
		case SDLK_RIGHT:
			keyRight = true;
			flip = SDL_FLIP_HORIZONTAL;
			rotationAngle = 0.0;
			break;
		case SDLK_LEFT:
			keyLeft = true;
			flip = SDL_FLIP_NONE;
			rotationAngle = 0.0;
			break;
		}
	}
	if (ev.type == SDL_KEYUP)
	{
		switch (ev.key.keysym.sym)
		{
		case SDLK_UP:
			keyUp = false;
			break;
		case SDLK_DOWN:
			keyDown = false;
			break;
		case SDLK_LEFT:
			keyLeft = false;
			break;
		case SDLK_RIGHT:
			keyRight = false;
			break;
		}
	}
}
void player::playerDeath(SDL_Renderer*  gRenderer)
{
	currentClip = spriteClips[frame / 10];
	frame++;
	deathFrame++;
	if (deathFrame >= 150)
	{
		deathFrame = 0;
		isHit = false;
		resetPosition();
		renderingQuad = { xPos, yPos, 25, 25 };
		return;
	}
	if (frame >= 60)
		frame = 0;
	SDL_RenderCopyEx(gRenderer, playerDyingImage, &currentClip, &renderingQuad, rotationAngle, NULL, flip);
}

void player::setToDeath()
{
	cout << --health << endl;
	isHit = true;
}
void player::resetPosition()
{
	xPos = 400;
	yPos = 515;
	keyUp = false, keyDown = false, keyRight = false, keyLeft = false;
}
void player::renderPlayer(SDL_Renderer* gRenderer, int** map)
{
	if (!isHit)
	{
		if (chompCarry)
			playChompSound();
		updatePosition(map);
		renderingQuad = { xPos, yPos, 25, 25 };
		currentClip = spriteClips[frame / 4];
		frame++;
		if (frame >= 24)
			frame = 0;
		SDL_RenderCopyEx(gRenderer, playerImage, &currentClip, &renderingQuad, rotationAngle, NULL, flip);
	}
}
void player::playChompSound()
{
	if (!Mix_Playing(1))
	{
		Mix_PlayChannelTimed(1, chompSound, 0, 150);
		chompCarry = false;
	}
	else
	{
		chompCarry = true;
	}
}
void player::playDeathSound()
{
	Mix_PlayChannel(1, deathSound, 0);
}