#pragma once

#include <string>

#include "imgui/views/ImGuiUIView.h"

namespace humongousexplorer::imgui
{
	class BottomToolbar : public ImGuiUIView
	{
	public:
		void Initialize();

		void Render() override;
		float GetSize() const;
	private:
		std::string m_sMessage;
		std::string m_sIcon;
		float m_fSize = 75;
	};
}