#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
using namespace std;

class player
{
public:
	player();
	~player();

	void renderPlayer(SDL_Renderer* gRenderer, int**);
	void playerInput(SDL_Event ev);
	void updatePosition(int**);
	SDL_Rect getPlayerRect() { return renderingQuad; }
	friend SDL_Texture* createTexture(string);
	bool playerHit() {return isHit;}
	bool chompCarry;

	void playChompSound();
	void playDeathSound();

	void resetPosition();
	void setToDeath();
	void playerDeath(SDL_Renderer*);

private:
	int health;
	int speed;
	int xPos, yPos;
	int xVel, yVel;
	int frame, deathFrame;
	double rotationAngle;
	int rowPlayerTile, colPlayerTile;
	bool keyUp, keyDown, keyLeft, keyRight;
	bool isHit;
	Mix_Chunk* chompSound;
	Mix_Chunk* deathSound;

	SDL_RendererFlip flip;
	SDL_Texture* playerImage;
	SDL_Texture* playerDyingImage;
	SDL_Rect spriteClips[6];
	SDL_Rect currentClip;
	SDL_Rect renderingQuad;

};

