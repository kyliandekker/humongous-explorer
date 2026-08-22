#pragma once

#include "imgui/windows/HEBaseWindow.h"

#include <string>

namespace humongousexplorer::imgui
{
	static constexpr float TOP_TOOLBAR_HEIGHT = 100;

	class TopToolbarWindow : public HEBaseWindow
	{
	public:
		TopToolbarWindow();
	private:
		void Update() override;

		bool OnInitialized() override;
	};
}
