
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <Windows.h>

#include "GameScreenManager.h"
#include "Settings.h"
#include "Debug.h"
#include "Cursor.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

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
		DBG_OUTPUT("SDL did not initialise. ERROR: ", SDL_GetError(), "\n");
		std::cerr << "SDL did not initialise. ERROR: " << SDL_GetError() << std::endl;
		return false;
	}
	else
	{
		//Attempt to set texture filtering to linear.
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			DBG_OUTPUT("Warning: Linear texture filtering not available \n");
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

		SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
		gWindow = SDL_CreateWindow("Delve, Die, Repeat",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			Settings::GetInstance()->GetScreenWidth(), Settings::GetInstance()->GetScreenHeight(),
			SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		gGLContext = SDL_GL_CreateContext(gWindow);
		SDL_GL_MakeCurrent(gWindow, gGLContext);

		SDL_SetWindowResizable(gWindow, SDL_TRUE);

		if (gWindow == NULL)
		{
			DBG_OUTPUT("Window was not created. Error: ", SDL_GetError(), "\n");
			std::cerr << "Window was not created. Error: " << SDL_GetError() << std::endl;
			return false;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		//Setup ImGui-------------------------------------------------------------------------
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImGui_ImplSDL2_InitForOpenGL(gWindow, gGLContext);
		ImGui_ImplOpenGL3_Init("#version 130");

		//Setup OpenGL------------------------------------------------------------------------
		glewExperimental = GL_TRUE;
		GLint GlewInitResult = glewInit();
		if (GLEW_OK != GlewInitResult)
		{
			DBG_OUTPUT("Glew Setup failed. ERROR: ", glewGetErrorString(GlewInitResult), "\n");
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
			DBG_OUTPUT("Warning: No controllers Connected!\n");
		}
		else
		{
			//load game controllers
			for (int i = 0; i < SDL_NumJoysticks(); i++)
			{
				gGameControllers[i] = SDL_GameControllerOpen(i);
				if (gGameControllers[i] == NULL)
				{
					DBG_OUTPUT("Warning: Unable to open game controller! SDL Error: ", SDL_GetError(), "\n");
					std::cerr << "Warning: Unable to open game controller! SDL Error: " << SDL_GetError() << std::endl;
				}
			}
			
		}

		//Setup Custom Cursors-----------------------------------------------------------------
		//Cursor::CreateCustomCursor(SDL_SYSTEM_CURSOR_ARROW, "Images/Cursor_Arrow.png", 0, 0);
		//Cursor::CreateCustomCursor(SDL_SYSTEM_CURSOR_CROSSHAIR, "Images/Cursor_Crosshairs.png", 32, 32);
		//Cursor::CreateCustomCursor(SDL_SYSTEM_CURSOR_HAND, "Images/Cursor_Pointer.png", 0, 0);

		Cursor::LoadCursors();

		Cursor::CustomCursorType("Cursor_Arrow");
	}
	glClearColor(0.0f, 0.5f, 0.1f, 1.0f);

	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
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

	//release cursors
	Cursor::ReleaseCursors();

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();

	//Shutdown ImGui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
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

	//GameScreenManager::GetInstance()->PauseGame();

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
		else
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
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

	//Clear the buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Render the game
	GameScreenManager::GetInstance()->Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(gWindow);
	ImGui::NewFrame();

	//ImGui::ShowDemoWindow();

	//FPS Counter
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 window_pos = ImVec2(viewport->Pos.x + 10, viewport->Pos.y + 10);
	//ImVec2 window_pos_pivot;
	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0, 0));

	bool p_open = true;

	ImGui::Begin("FPS", &p_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar 
		| ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize 
		| ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing 
		| ImGuiWindowFlags_NoNav);

	ImGui::Text("%.1f", ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//Present Back Buffer to screen
	SDL_GL_SwapWindow(gWindow);

	
}

//Checks if another instance of the game is already running
bool AnotherInstance()
{
	HANDLE ourMutex;
	ourMutex = CreateMutex(NULL, true, L"Use_a_different_string_here_for_each_program_48161-XYZZY");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		DBG_OUTPUT("Error: Application Already Open\n");
		return true;
	}

	return false;
}