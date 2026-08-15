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
		void OnStatusMessageUpdated(bool a_bSuccess, const std::string& a_sMessage);

		bool m_bInitialized = false;

		std::string m_sMessage;
		std::string m_sIcon;
		float m_fSize = 75;
	};
}