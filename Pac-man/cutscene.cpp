#include "cutscene.h"

cutscene::cutscene(string fileName)
{
	//opens cutscene file and loads images
	cutSceneFile.open(fileName.c_str());
	if (!cutSceneFile.is_open())
	{
		cout << "cutscene file could not be opened" << endl;
	}
	playerImage = createTexture("waka.png");
	ghostImage = createTexture("redGhost.png");
	speed = 3;
	sceneComplete = false;

	//begins reading in ghostcount and playercount. Then reads in
	//the player(s) and ghosts initial position and defauls values.
	cutSceneFile >> playerCount;
	player = new spriteStruct[playerCount];
	cutSceneFile >> ghostCount;
	ghostArr = new spriteStruct[ghostCount];

	for (int i = 0; i < playerCount; i++)
	{
		cutSceneFile >> player[i].spriteRect.x;
		cutSceneFile >> player[i].spriteRect.y;
		player[i].spriteRect.w = 25;
		player[i].spriteRect.h = 25;
		player[i].finished = false;
	}
	for (int i = 0;i < ghostCount;i++)
	{
		cutSceneFile >> ghostArr[i].spriteRect.x;
		cutSceneFile >> ghostArr[i].spriteRect.y;
		ghostArr[i].spriteRect.w = 25;
		ghostArr[i].spriteRect.h = 25;
		ghostArr[i].finished = false;
	}

	//stores all movement instructions for the waka
	for (int i = 0; i < playerCount;i++)
	{
		player[i].index = 0;
		cutSceneFile >> player[i].instructCount;
		player[i].direction = new char[player[i].instructCount];
		player[i].frameCount = new int[player[i].instructCount];
		for (int j = 0; j < player[i].instructCount; j++)
		{
			cutSceneFile >> player[i].direction[j];
			cutSceneFile >> player[i].frameCount[j];
		}
	}

	//stores all the movement instructions for the ghosts
	for (int i = 0; i < ghostCount; i++)
	{
		ghostArr[i].index = 0;
		cutSceneFile >> ghostArr[i].instructCount;
		ghostArr[i].direction = new char[ghostArr[i].instructCount];
		ghostArr[i].frameCount = new int[ghostArr[i].instructCount];
		for (int j = 0; j < ghostArr[i].instructCount ; j++)
		{
			cutSceneFile >> ghostArr[i].direction[j];
			cutSceneFile >> ghostArr[i].frameCount[j];
		}
	}
}

cutscene::~cutscene()
{
	delete player;
	delete ghostArr;
}

void cutscene::updatePositions(spriteStruct target[], int count)
{
	//sprite movement
	for (int i = 0; i < count; i++)
	{
		if (!target[i].finished)
		{
			switch (target[i].direction[target[i].index])
			{
			case 'l':
				target[i].spriteRect.x = target[i].spriteRect.x - speed;
				break;

			case 'r':
				target[i].spriteRect.x = target[i].spriteRect.x + speed;
				break;

			case 'u':
				target[i].spriteRect.y = target[i].spriteRect.y - speed;
				break;

			case 'd':
				target[i].spriteRect.y = target[i].spriteRect.y + speed;
				break;

			default:
				break;
			}
			target[i].frameCount[target[i].index]--;
			if (target[i].frameCount[target[i].index] == 0)
			{
				target[i].index++;
			}
			if (target[i].index == target[i].instructCount)
			{
				target[i].finished = true;
			}
		}
	}
}

void cutscene::renderScene(SDL_Renderer* gRenderer)
{
	updatePositions(ghostArr, ghostCount);
	updatePositions(player, playerCount);
	for (int i = 0; i < playerCount; i++)
	{
		SDL_RenderCopy(gRenderer, playerImage, NULL, &player[i].spriteRect);
	}
	for (int i = 0;i < ghostCount;i++)
	{
		SDL_RenderCopy(gRenderer, ghostImage, NULL, &ghostArr[i].spriteRect);
	}

	if (allSpritesDone(ghostArr, ghostCount) && allSpritesDone(player, playerCount))
	{
		sceneComplete = true;
	}
}

bool cutscene::allSpritesDone(spriteStruct target[], int count)
{
	for (int i = 0; i < count; i++)
	{
		if (!target[i].finished)
		{
			return false;
		}
	}
	return true;
}