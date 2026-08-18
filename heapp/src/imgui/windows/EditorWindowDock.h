#pragma once

#include "imgui/windows/MainWindowDock.h"

namespace humongousexplorer::imgui
{
	class EditorWindowDock : public MainWindowDock
	{
	private:
		void Update() override;

		bool OnInitialized() override
		{
			return true;
		}
	};
}