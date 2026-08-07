#pragma once

#include "imgui/windows/MainWindowDock.h"

namespace humongousexplorer::imgui
{
	class EditorWindowDock : public MainWindowDock
	{
	public:
		void Render() override;
	};
}