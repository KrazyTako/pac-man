#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;

const int windowWidth = 640, windowHeight = 480;

SDL_Texture* createTexture(string, SDL_Renderer*);
void displayError(string);
void init(SDL_Window* &window, SDL_Renderer* &renderer)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		displayError("SDL_INIT_VIDEO");

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		cout << "SDL_image error" << IMG_GetError() << endl;
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
	SDL_Renderer *gRenderer = NULL;
	SDL_Window *gWindow = NULL;
	SDL_Texture *hello = NULL;

	init(gWindow, gRenderer);              //inits SDL and creates window and renderer.
	
	hello = createTexture("hello.bmp", gRenderer);

	for (int i = 0;i < 3;i++)
	{
		SDL_RenderClear(gRenderer);
		SDL_RenderCopy(gRenderer, hello, NULL, NULL);
		SDL_RenderPresent(gRenderer);
		SDL_Delay(1000);
	}


	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();

	return 0;
}

SDL_Texture* createTexture(string fileName, SDL_Renderer* renderer)
{
	SDL_Texture *finalTexture = NULL;
	SDL_Surface *tempSurface = NULL;
	tempSurface = IMG_Load(fileName.c_str());
	if (tempSurface == NULL)
		displayError("IMG_Load");

	finalTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
	if (finalTexture == NULL)
		displayError("SDL_Create TextureFromSurface");

	SDL_FreeSurface(tempSurface);

	return finalTexture;
}
void displayError(string msg)
{
	cout << msg << " Error: " << SDL_GetError() << endl;
	SDL_Delay(5000);
	SDL_Quit();
}