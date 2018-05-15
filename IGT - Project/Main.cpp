#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

#include "Constants.h"


//Globals------------------------------------------------------------------------------------
SDL_Window*			gWindow = NULL;
SDL_Renderer*		gRenderer = NULL;
Uint32				gOldTime;
SDL_GameController* gGameControllers[4];

//Function Prototypes------------------------------------------------------------------------
bool InitSDL();
void CloseSDL();

bool Update();
void Render();

int main(int argc, char* args[])
{
	if (InitSDL())
	{
		bool quit = false;
		gOldTime = SDL_GetTicks();

		while (!quit)
		{
			quit = Update();
			Render();
		}

	}

	CloseSDL();

	return 0;
}

bool InitSDL()
{
	//Setup SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		std::cerr << "SDL did not initialise. ERROR: " << SDL_GetError();
		return false;
	}
	else
	{
		//Attempt to set texture filtering to linear.
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			std::cerr << "Warning: Linear texture filtering not available";
		}

		//Create Window-----------------------------------------------------------------------

		gWindow = SDL_CreateWindow("Dungeon Game",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

		if (gWindow == NULL)
		{
			std::cerr << "Window was not created. Error: " << SDL_GetError();
			return false;
		}

		//Create Renderer---------------------------------------------------------------------
		
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer != NULL)
		{
			//Initialise PNG loading
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags)&imageFlags))
			{
				std::cerr << "SDL_Image could not initialise. ERROR: " << IMG_GetError();
				return false;
			}
		}
		else
		{
			std::cerr << "Renderer could not initialise. Error: " << SDL_GetError();
			return false;
		}
	}
	return true;
}

void CloseSDL()
{
	//Destroy the game screen manager
	//delete gGameScreenManager;
	//gGameScreenManager = NULL;

	//Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Release the renderer
	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool Update()
{
	//Get the new time.
	Uint32 newTime = SDL_GetTicks();

	//Event Handler.
	SDL_Event e;

	std::vector<SDL_Event>events;

	//Get the events.
	while (SDL_PollEvent(&e) != 0)
	{
		events.push_back(e);
	}

	//GameScreenManager::GetInstance()->Update((float)(newTime - gOldTime) / 1000.0f, events);

	//Handle quiting.
	for (auto e : events)
	{
		if (e.type == SDL_QUIT)
			return true;
	}


	//Set the current time to be the old time.
	gOldTime = newTime;

	return false;
}

void Render()
{
	//clear the Screen
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(gRenderer);

	//GameScreenManager::GetInstance()->Render();

	//Present Back Buffer to screen
	SDL_RenderPresent(gRenderer);
}


