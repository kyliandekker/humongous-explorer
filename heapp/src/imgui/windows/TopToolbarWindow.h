#pragma once

#include "imgui/windows/HEBaseWindow.h"

#include <string>

namespace humongousexplorer::imgui
{
	inline float TOP_TOOLBAR_HEIGHT = 120;

	class TopToolbarWindow : public HEBaseWindow
	{
	public:
		TopToolbarWindow();
	private:
		void Update() override;

		bool OnInitialized() override;
	};
}
