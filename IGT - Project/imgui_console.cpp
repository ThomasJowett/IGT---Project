#include "imgui_console.h"
#include "Settings.h"
#include "Collision.h"
#include <locale>
#include "imguiManager.h"

ImGui::Console::Console()
{
	ClearLog();
	memset(InputBuf, 0, sizeof(InputBuf));
	HistoryPos = -1;
	Commands.push_back("HELP");
	Commands.push_back("CLEAR");
	Commands.push_back("HISTORY");
	Commands.push_back("QUIT");
	//Commands.push_back("SHOW_COLLISION");
	//Commands.push_back("SHOW_QUADTREE");
	Commands.push_back("LIST_OBJECTS");
	Commands.push_back("PAUSE");
	Commands.push_back("SETTINGS_VSYNC");
	Commands.push_back("SETTINGS_ZOOM");
	Commands.push_back("LOAD_LEVEL");


	AutoScroll = true;
	ScrollToBottom = true;
}

void ImGui::Console::ClearLog()
{
	for (int i = 0; i < Items.Size; i++)
	{
		free(Items[i]);
	}
	Items.clear();
	ScrollToBottom = true;
}

void ImGui::Console::Draw(bool * p_open)
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y / 3), ImGuiCond_FirstUseEver);

	if (!ImGui::Begin("Console", p_open))
	{
		ImGui::End();
		return;
	}

	//if (ImGui::SmallButton("Add Dummy Error")) { AddLog("[error] something went wrong"); } ImGui::SameLine();
	if (ImGui::SmallButton("Clear")) { ClearLog(); }ImGui::SameLine();
	bool copy_to_clipboard = ImGui::SmallButton("Copy"); ImGui::SameLine();
	if (ImGui::SmallButton("Scroll to Bottom")) ScrollToBottom = true;

	ImGui::Separator();//--------------------------------------------------------------------------------------------

	//Filter
	Filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);

	ImGui::Separator();//--------------------------------------------------------------------------------------------

	//scrolling region
	const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) ClearLog();
		ImGui::EndPopup();
	}

	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
	if (copy_to_clipboard)
		ImGui::LogToClipboard();
	for (int i = 0; i < Items.Size; i++)
	{
		const char* item = Items[i];
		if (!Filter.PassFilter(item))
			continue;

		// Normally you would store more information in your item (e.g. make Items[] an array of structure, store color/type etc.)
		bool pop_color = false;
		if (strstr(item, "[error]")) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); pop_color = true; }
		else if (strncmp(item, "# ", 2) == 0) { ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.6f, 1.0f)); pop_color = true; }
		ImGui::TextUnformatted(item);
		if (pop_color)
			ImGui::PopStyleColor();
	}
	if (copy_to_clipboard)
		ImGui::LogFinish();
	if (ScrollToBottom)
		ImGui::SetScrollHereY(1.0f);
	ScrollToBottom = false;
	ImGui::PopStyleVar();
	ImGui::EndChild();

	ImGui::Separator();//--------------------------------------------------------------------------------------------

	//CommandLine

	const char* title = GameScreenManager::GetInstance()->GetCurrentScreen()->GetScreenName();

	bool reclaim_focus = false;
	if (ImGui::InputText(title, InputBuf, IM_ARRAYSIZE(InputBuf),
		ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion
		| ImGuiInputTextFlags_CallbackHistory, &TextEditCallbackStub, (void*)this))
	{
		char* s = InputBuf;
		Strtrim(s);
		if (s[0])
		{
			ExecCommand(s);
			strcpy_s(s, strlen(s), "");
		}

		reclaim_focus = true;
	}

	//Auto-focus on this window apparition
	ImGui::SetItemDefaultFocus();
	if (reclaim_focus)
		ImGui::SetKeyboardFocusHere(-1);//AutoFocus Previous widget

	ImGui::End();
}

void ImGui::Console::ExecCommand(const char * command_line)
{
	std::locale loc;

	std::string command_line_str = command_line;

	for (std::string::size_type i = 0; i < command_line_str.length(); ++i)
		command_line_str[i] = std::toupper(command_line_str[i], loc);

	command_line = command_line_str.c_str();

	AddLog("# %s\n", command_line);

	//Insert into history
	for (int i = History.Size - 1; i >= 0; i--)
	{
		if (Stricmp(History[i], command_line) == 0)
		{
			free(History[i]);
			History.erase(History.begin() + i);
			break;
		}
	}

	History.push_back(Strdup(command_line));

	if (Stricmp(command_line, "CLEAR") == 0)
	{
		ClearLog();
	}
	else if (Stricmp(command_line, "HELP") == 0)
	{
		AddLog("Commands:");
		for (int i = 0; i < Commands.Size; i++)
		{
			AddLog(" - %s", Commands[i]);
		}
	}
	else if (Stricmp(command_line, "HISTORY") == 0)
	{
		int first = History.Size - 10;
		for (int i = first > 0 ? first : 0; i < History.Size; i++)
		{
			AddLog("%3d: %s\n", i, History[i]);
		}
	}
	else if (Stricmp(command_line, "SHOW_COLLISION") == 0)
	{
		//TODO: implement drawing the quadtree
		ImGui::Manager::GetInstance()->ToggleCollsion();

		AddLog("Command not yet implemented\n");
	}
	else if (Stricmp(command_line, "SHOW_QUADTREE") == 0)
	{
		QuadTree* quadtree = Collision::GetQuadtree();

		//TODO: implement drawing the quadtree
		AddLog("Command not yet implemented\n");
	}
	else if (Stricmp(command_line, "LIST_OBJECTS") == 0)
	{
		for (auto gameObject : GameScreenManager::GetInstance()->GetCurrentScreen()->GetAllGameObjects())
		{
			AddLog("%i  %s \n", gameObject->GetUniqueID(), gameObject->GetName());
		}
	}
	else if (Stricmp(command_line, "PAUSE") == 0)
	{
		GameScreenManager::GetInstance()->TogglePause();
	}
	else if (strstr(command_line, "SETTINGS_VSYNC"))
	{
		command_line_str.erase(command_line_str.begin(), command_line_str.begin() + 14);

		Settings::GetInstance()->SetVsync(atoi(command_line_str.c_str()));
	}
	else if (strstr(command_line, "SETTINGS_ZOOM"))
	{
		command_line_str.erase(command_line_str.begin(), command_line_str.begin() + 12);

		Settings::GetInstance()->SetZoom(atof(command_line_str.c_str()));
	}
	else if (strstr(command_line, "LOAD_LEVEL"))
	{
		command_line_str.erase(command_line_str.begin(), command_line_str.begin() + 11);

		
		if (Stricmp("SCREEN_MENU", command_line_str.c_str()) == 0)
		{
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_MENU);
		}
		else if (Stricmp("SCREEN_LEVEL_1", command_line_str.c_str()) == 0)
		{
			GameScreenManager::GetInstance()->ChangeScreen(SCREEN_LEVEL_1);
		}
		else
		{
			AddLog("[error]: 's%' Level not recognised.");
		}
	}
	else if (Stricmp(command_line, "QUIT") == 0)
	{
		SDL_Event sdlEvent;
		sdlEvent.type = SDL_QUIT;
		SDL_PushEvent(&sdlEvent);
	}
	else
	{
		AddLog("[error]: '%s' Command not recognised. Type 'HELP' for list of commands.\n", command_line);
	}

	ScrollToBottom = true;
}

int ImGui::Console::TextEditCallbackStub(ImGuiInputTextCallbackData * data)
{
	Console* console = (Console*)data->UserData;
	return console->TextEditCallback(data);
}

int ImGui::Console::TextEditCallback(ImGuiInputTextCallbackData * data)
{
	//AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
	switch (data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackCompletion:
	{
		// Example of TEXT COMPLETION

		// Locate beginning of current word
		const char* word_end = data->Buf + data->CursorPos;
		const char* word_start = word_end;
		while (word_start > data->Buf)
		{
			const char c = word_start[-1];
			if (c == ' ' || c == '\t' || c == ',' || c == ';')
				break;
			word_start--;
		}

		// Build a list of candidates
		ImVector<const char*> candidates;
		for (int i = 0; i < Commands.Size; i++)
			if (Strnicmp(Commands[i], word_start, (int)(word_end - word_start)) == 0)
				candidates.push_back(Commands[i]);

		if (candidates.Size == 0)
		{
			// No match
			AddLog("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
		}
		else if (candidates.Size == 1)
		{
			// Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
			data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
			data->InsertChars(data->CursorPos, candidates[0]);
			data->InsertChars(data->CursorPos, " ");
		}
		else
		{
			// Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
			int match_len = (int)(word_end - word_start);
			for (;;)
			{
				int c = 0;
				bool all_candidates_matches = true;
				for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
					if (i == 0)
						c = toupper(candidates[i][match_len]);
					else if (c == 0 || c != toupper(candidates[i][match_len]))
						all_candidates_matches = false;
				if (!all_candidates_matches)
					break;
				match_len++;
			}

			if (match_len > 0)
			{
				data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
				data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
			}

			// List matches
			AddLog("Possible matches:\n");
			for (int i = 0; i < candidates.Size; i++)
				AddLog("- %s\n", candidates[i]);
		}

		break;
	}
	case ImGuiInputTextFlags_CallbackHistory:
	{
		// Example of HISTORY
		const int prev_history_pos = HistoryPos;
		if (data->EventKey == ImGuiKey_UpArrow)
		{
			if (HistoryPos == -1)
				HistoryPos = History.Size - 1;
			else if (HistoryPos > 0)
				HistoryPos--;
		}
		else if (data->EventKey == ImGuiKey_DownArrow)
		{
			if (HistoryPos != -1)
				if (++HistoryPos >= History.Size)
					HistoryPos = -1;
		}

		// A better implementation would preserve the data on the current input line along with cursor position.
		if (prev_history_pos != HistoryPos)
		{
			const char* history_str = (HistoryPos >= 0) ? History[HistoryPos] : "";
			data->DeleteChars(0, data->BufTextLen);
			data->InsertChars(0, history_str);
		}
	}
	}
	return 0;
}
