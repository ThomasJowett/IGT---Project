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

		void Update(std::vector<SDL_Event> events);
		void Render();

		void Initialise();

		void ShutDown();

	private:
		Manager();

		void DockSpace(bool* p_open);

		bool mShowDockSpace = true;
		bool mShowConsole = false;

		ImGui::Console mConsole;
	};

}