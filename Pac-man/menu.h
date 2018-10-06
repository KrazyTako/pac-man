#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <SDL_ttf.h>
using namespace std;

class menu
{
public:
	menu(SDL_Renderer*);
	~menu();

	struct textTexture
	{
		SDL_Texture* texture;
		string message;
		SDL_Rect renderRect;
	};

	void createFont(TTF_Font*, SDL_Renderer*, textTexture&);
	void renderMenu(SDL_Renderer*);

private:
	TTF_Font* gameFont;
	textTexture startGameText;
	textTexture resumeText;
	textTexture placeHolder;
};

