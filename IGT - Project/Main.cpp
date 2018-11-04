

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <Windows.h>

#include "GameScreenManager.h"
#include "Settings.h"


//Globals------------------------------------------------------------------------------------
SDL_Window*			gWindow = nullptr;
SDL_GLContext		gGLContext = nullptr;
Uint32				gOldTime;
SDL_GameController* gGameControllers[4];

//Function Prototypes------------------------------------------------------------------------
bool InitSDL();
void CloseSDL();

bool Update();
void Render();

bool AnotherInstance();

int main(int argc, char* args[])
{
	if (!AnotherInstance() && InitSDL())
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

		//Set OpenGL attributes---------------------------------------------------------------
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		//Create Window-----------------------------------------------------------------------

		gWindow = SDL_CreateWindow("Delve, Die, Repeat",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight(),
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		gGLContext = SDL_GL_CreateContext(gWindow);

		SDL_SetWindowResizable(gWindow, SDL_TRUE);

		if (gWindow == NULL)
		{
			std::cerr << "Window was not created. Error: " << SDL_GetError();
			return false;
		}

		//Setup OpenGL------------------------------------------------------------------------
		glewExperimental = GL_TRUE;
		GLint GlewInitResult = glewInit();
		if (GLEW_OK != GlewInitResult)
		{
			std::cerr << "Glew Setup failed. ERROR: " << glewGetErrorString(GlewInitResult) << std::endl;
			return false;
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Setup Controllers---------------------------------------------------------------------
		if (SDL_NumJoysticks() < 1)
		{
			std::cerr << "Warning: No Controllers connected!\n";
		}
		else
		{
			//load game controllers
			for (int i = 0; i < SDL_NumJoysticks(); i++)
			{
				gGameControllers[i] = SDL_GameControllerOpen(i);
				if (gGameControllers[i] == NULL)
				{
					std::cerr << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError();
				}
			}
			
		}

		SDL_ShowCursor(SDL_DISABLE);
	}
	glClearColor(0, 0.5, 0.1, 1);
	return true;
}

void CloseSDL()
{
	//Destroy the game screen manager
	delete GameScreenManager::GetInstance();

	//Close game controllers
	for (auto controller : gGameControllers)
	{
		SDL_GameControllerClose(controller);
	}

	//Release the window
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	//Release the renderer
	SDL_GL_DeleteContext(gGLContext);
	gGLContext = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}

bool Update()
{
	//Get the new time.
	Uint32 newTime = SDL_GetTicks();

	//Event Handler.
	SDL_Event e;

	std::vector<SDL_Event>events;

	SDL_PumpEvents();

	//Get the events.
	while (SDL_PollEvent(&e) != 0)
	{
		events.push_back(e);
	}

	GameScreenManager::GetInstance()->Update((float)(newTime - gOldTime) / 1000.0f, events);

	//Handle quiting.
	for (auto e : events)
	{
		if (e.type == SDL_WINDOWEVENT)
		{
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				Settings::GetInstance()->SetResolution(e.window.data1, e.window.data2);
			}
		}
		if (e.type == SDL_QUIT)
			return true;
	}

	//Set the current time to be the old time.
	gOldTime = newTime;

	return false;
}

void Render()
{
	//Clear the Screen
	//glClearColor(0, 0.5, 0.1, 1);

	//Render the game
	GameScreenManager::GetInstance()->Render();

	//Present Back Buffer to screen
	SDL_GL_SwapWindow(gWindow);

	//Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//Checks if another instance of the game is already running
bool AnotherInstance()
{
	HANDLE ourMutex;
	ourMutex = CreateMutex(NULL, true, "Use_a_different_string_here_for_each_program_48161-XYZZY");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return true;

	return false;
}