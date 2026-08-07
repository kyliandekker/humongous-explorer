#pragma once

#include <vector>
#include <string>
#include <functional>
#include <imgui/imgui.h>

namespace humongousexplorer::imgui
{
	enum class RowInfoTextAlignment
	{
		Left,
		Right
	};

	struct RowInfo
	{
		RowInfo(const std::string& a_sName, ImColor a_Color, float a_fExtraOffset, RowInfoTextAlignment a_RowInfoTextAlignment = RowInfoTextAlignment::Left) :
			m_sName(a_sName),
			m_Color(a_Color),
			m_fExtraOffset(a_fExtraOffset),
			m_RowInfoTextAlignment(a_RowInfoTextAlignment)
		{
		}

		std::string m_sName;
		ImColor m_Color;
		float m_fExtraOffset;
		RowInfoTextAlignment m_RowInfoTextAlignment;
	};

	enum class FileEntryInteractionType
	{
		None,
		LeftClicked,
		RightClicked,
		DoubleClicked
	};

	struct FileEntryView
	{
		FileEntryView(const std::string& a_sIcon, const std::vector<RowInfo>& a_aRows) : 
			m_sIcon(a_sIcon),
			m_aRows(a_aRows)
		{}

		std::string m_sIcon;
		std::vector<RowInfo> m_aRows;

		virtual FileEntryInteractionType Render(std::function<bool()> a_fnSelected);
	};

	struct TreeFileEntryView : public FileEntryView
	{
		std::vector<TreeFileEntryView> m_aChildren;

		FileEntryInteractionType Render(std::function<bool()> a_fnSelected) override;
	};
}