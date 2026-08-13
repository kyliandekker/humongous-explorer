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

		void OnLoadArchiveSuccess(const std::string& a_sName);
		void OnLoadArchiveFailed(const std::string& a_sName, const std::string& a_sReason);
		void OnLoadArchiveProgressed(float a_fProgress);
	private:
		std::string m_sMessage;
		std::string m_sIcon;
		float m_fSize = 75;
		float m_fRealPercentage = 0.0f;
		float m_fPercentage = 0.0f;
	};
}