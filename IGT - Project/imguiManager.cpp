#include "imguiManager.h"
#include "imguiManager.h"
#include "Cursor.h"

extern SDL_Window* gWindow;
extern SDL_GLContext gGLContext;

static ImGui::Manager* instance = 0;

ImGui::Manager::Manager()
{
	mShowDockSpace = true;
	mShowConsole = false;
	mShowCollision = false;

	mCursorOverWindow = false;
}

void ImGui::Manager::DockSpace(bool* p_open)
{
	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_PassthruDockspace;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", p_open, window_flags);
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	if (mShowCollision)
		DrawCollision(&mShowCollision);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}
	else
	{
		//ShowDockingDisabledMessage();
	}

	ImGui::End();
}

void ImGui::Manager::DrawCollision(bool * p_open)
{
	ImVec2 window_pos = ImGui::GetWindowPos();
	ImVec2 window_size = ImGui::GetWindowSize();
	ImVec2 window_center = ImVec2(window_pos.x + window_size.x * 0.5f, window_pos.y + window_size.y * 0.5f);

	ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

	GameScreen* currentscreen = GameScreenManager::GetInstance()->GetCurrentScreen();
	Camera* camera = currentscreen->GetCamera();

	for (Collider* collider : currentscreen->GetAllColliders())
	{
		//TODO: get collisions to be rendered ontop of their actual locations
		if (collider->mType == CIRCLE2D)
		{
			Circle2D* circle = dynamic_cast<Circle2D*>(collider);
			draw_list->AddCircle(window_center, circle->GetRadius() * 4, IM_COL32(0, 255, 0, 200), 32, 1.0f);
		}
		else if (collider->mType == BOX2D)
		{
			Box2D* box = dynamic_cast<Box2D*>(collider);
			float width = box->GetWidth() *4;
			float height = box->GetHeight()*4;
			draw_list->AddRect(window_center, ImVec2(window_center.x + width, window_center.y + height), IM_COL32(255, 0, 255, 200), 0, ImDrawCornerFlags_All, 1.0f);
		}
	}
}


ImGui::Manager::~Manager()
{
}

ImGui::Manager * ImGui::Manager::GetInstance()
{
	if (instance == 0)
	{
		instance = new ImGui::Manager();
	}
	return instance;
}

bool ImGui::Manager::Update(std::vector<SDL_Event> &events)
{
	for (SDL_Event e : events)
	{
		if (e.type == SDL_KEYDOWN)
		{
			switch (e.key.keysym.sym)
			{
			case SDLK_SEMICOLON:
				mShowConsole = !mShowConsole;
				break;
			default:
				break;
			}
		}
	
		if (mShowConsole)
		{
			ImGui_ImplSDL2_ProcessEvent(&e);
		}
	}
	return !mShowConsole;
}

void ImGui::Manager::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(gWindow);
	ImGui::NewFrame();

	DockSpace(&mShowDockSpace);

	//ImGui::ShowDemoWindow();

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGuiIO& io = ImGui::GetIO();

	//FPS Counter-----------------------------------------------------------------------
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

	if (mShowConsole)
	{
		mConsole.Draw(&mShowConsole);
	}

	if (ImGui::IsMouseHoveringAnyWindow())
	{
		if (!mCursorOverWindow)
		{
			mCursorOverWindow = true;
		}
		Cursor::SystemCursorType(SDL_SYSTEM_CURSOR_ARROW);
		SDL_ShowCursor(true);
	}
	else if(mCursorOverWindow)
	{
		mCursorOverWindow = false;
		Cursor::CustomCursorType("Cursor_Arrow");
	}

	ImGui::Render();

	io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGui::Manager::Initialise()
{
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

	//setup the windows style
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.3f;
		style.Colors[ImGuiCol_ChildWindowBg].w = 0.0f;
		style.Colors[ImGuiCol_ChildBg].w = 0.0f;
	}

	ImGui_ImplSDL2_InitForOpenGL(gWindow, gGLContext);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGui::Manager::ShutDown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ImGui::Manager::ToggleCollsion()
{
	mShowCollision = !mShowCollision;
}
