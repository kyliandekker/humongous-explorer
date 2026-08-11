#pragma once

#include "imgui/views/ImGuiUIView.h"

namespace humongousexplorer::imgui
{
	class BottomToolbar : public ImGuiUIView
	{
	public:
		void Render() override;
		float GetSize() const;
	private:
		float m_fSize = 75;
	};
}