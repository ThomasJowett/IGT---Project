#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "imgui_console.h"

#include <SDL.h>

namespace ImGui
{
	class Manager
	{
	public:
		~Manager();

		static Manager* GetInstance();

		bool Update(std::vector<SDL_Event>&eventQueue);
		void Render();

		void Initialise();

		void ShutDown();

		void ToggleCollsion();

	private:
		Manager();

		void DockSpace(bool* p_open);
		void DrawCollision(bool* p_open);

		bool mShowDockSpace;
		bool mShowConsole;
		bool mShowCollision;

		ImGui::Console mConsole;

		bool mCursorOverWindow;
	};

}