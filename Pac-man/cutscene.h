#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <fstream>
using namespace std;

class cutscene
{
public:
	cutscene(string);
	~cutscene();

	struct spriteStruct
	{
		SDL_Rect spriteRect;
		char* direction;
		int* frameCount;
		int index, instructCount;
		bool finished;
	};
	bool sceneComplete;
	bool allSpritesDone(spriteStruct[], int);
	void updatePositions(spriteStruct[], int);
	void renderScene(SDL_Renderer*);

	friend SDL_Texture* createTexture(string);

private:
	int ghostCount, playerCount;
	int speed;

	SDL_Texture* playerImage;
	SDL_Texture* ghostImage;
	spriteStruct* ghostArr;
	spriteStruct* player;

	ifstream cutSceneFile;

};