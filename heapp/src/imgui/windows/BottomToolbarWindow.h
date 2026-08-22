#pragma once

#include "imgui/windows/HEBaseWindow.h"

#include <string>

namespace humongousexplorer::imgui
{
	static constexpr float BOTTOM_TOOLBAR_HEIGHT = 75;

	class BottomToolbarWindow : public HEBaseWindow
	{
	public:
		BottomToolbarWindow();
	private:
		void Update() override;

		// Inherited via HEBaseWindow
		bool OnInitialized() override;
	};
}
