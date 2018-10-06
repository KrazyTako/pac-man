#include "menu.h"

menu::menu(SDL_Renderer* gRenderer)
{
	gameFont = TTF_OpenFont("lazy.ttf", 64);

	startGameText.message = "Start";
	createFont(gameFont, gRenderer, startGameText);
	startGameText.renderRect.x = 320-(startGameText.renderRect.w/2);
	startGameText.renderRect.y = 200;

	resumeText.message = "Resume";
	createFont(gameFont, gRenderer, resumeText);
	resumeText.renderRect.x = 320-(resumeText.renderRect.w/2);
	resumeText.renderRect.y = 300;

	placeHolder.message = "Place Holder";
	createFont(gameFont, gRenderer, placeHolder);
	placeHolder.renderRect.x = 320 - (placeHolder.renderRect.w / 2);
	placeHolder.renderRect.y = 400;
}


menu::~menu()
{
}

void menu::renderMenu(SDL_Renderer* gRenderer)
{
	SDL_RenderCopy(gRenderer, startGameText.texture, NULL, &startGameText.renderRect);
	SDL_RenderCopy(gRenderer, resumeText.texture, NULL, &resumeText.renderRect);
	SDL_RenderCopy(gRenderer, placeHolder.texture, NULL, &placeHolder.renderRect);
}

void menu::createFont(TTF_Font* font, SDL_Renderer* gRenderer, textTexture& target)
{
	SDL_Color txtColor = { 255, 0, 160 };
	SDL_Surface* textSurface;
	textSurface = TTF_RenderText_Solid(font, target.message.c_str(), txtColor);
	if (textSurface == NULL)
	{
		cout << "unable to render text surface!" << TTF_GetError() << endl;
	}
	else
	{
		target.texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (target.texture == NULL)
		{
			cout << "unable to create texture from surface!" << TTF_GetError() << endl;
		}
		else
		{
			target.renderRect.h = textSurface->h;
			target.renderRect.w = textSurface->w;
		}
		SDL_FreeSurface(textSurface);
	}
}
