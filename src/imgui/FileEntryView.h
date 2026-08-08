#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <imgui/imgui.h>
#include <imgui_internal.h>

struct ID3D11ShaderResourceView;

namespace humongousexplorer::imgui
{
	//---------------------------------------------------------------------
	enum class RowInfoTextAlignment
	{
		Left,
		Right
	};

	//---------------------------------------------------------------------
	struct RowInfo
	{
		RowInfo(float a_fExtraOffset, RowInfoTextAlignment a_RowInfoTextAlignment = RowInfoTextAlignment::Left) :
			m_fExtraOffset(a_fExtraOffset),
			m_RowInfoTextAlignment(a_RowInfoTextAlignment)
		{
		}

		float m_fExtraOffset;
		RowInfoTextAlignment m_RowInfoTextAlignment;
	};

	//---------------------------------------------------------------------
	struct RowEntry
	{
		RowEntry(RowInfo a_RowInfo) :
			m_RowInfo(a_RowInfo)
		{}

		virtual ~RowEntry() = default;

		RowInfo m_RowInfo;

		virtual void Render(const ImVec2& a_vPos) = 0;
		virtual ImVec2 GetSize() = 0;
	};

	//---------------------------------------------------------------------
	struct TextRowEntry : public RowEntry
	{
		TextRowEntry(RowInfo a_RowInfo, const std::string& a_sText, ImColor a_Color) : RowEntry(a_RowInfo),
			m_sText(a_sText),
			m_Color(a_Color)
		{}

		std::string m_sText;
		ImColor m_Color;

		void Render(const ImVec2& a_vPos) override;
		ImVec2 GetSize() override;
	};

	//---------------------------------------------------------------------
	struct IconRowEntry : public RowEntry
	{
		IconRowEntry(RowInfo a_RowInfo, const std::string& a_sIconName, float a_fSize = 0) : RowEntry(a_RowInfo),
			m_sIconName(a_sIconName),
			m_fSize(a_fSize)
		{}

		float m_fSize = 0;
		std::string m_sIconName;
		ID3D11ShaderResourceView* m_pTexture;

		void Render(const ImVec2& a_vPos) override;
		ImVec2 GetSize() override;
	};

	//---------------------------------------------------------------------
	enum class FileEntryInteractionType
	{
		None,
		LeftClicked,
		RightClicked,
		DoubleClicked
	};

	//---------------------------------------------------------------------
	struct FileEntryView
	{
		FileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows) :
			m_aRows(std::move(a_aRows))
		{}

		std::vector<std::unique_ptr<RowEntry>> m_aRows;

		virtual FileEntryInteractionType Render(std::function<bool()> a_fnSelected);
	};

	//---------------------------------------------------------------------
	struct TreeFileEntryView : public FileEntryView
	{
		TreeFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows, std::vector<std::unique_ptr<TreeFileEntryView>> a_aChildren = {}) : FileEntryView(std::move(a_aRows)),
			m_aChildren(std::move(a_aChildren))
		{}

		std::vector<std::unique_ptr<TreeFileEntryView>> m_aChildren;
		FileEntryInteractionType Render(std::function<bool()> a_fnSelected) override;
	};

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeTextRow(const RowInfo& a_RowInfo, const std::string& a_sLabel)
	{
		return std::make_unique<TextRowEntry>(RowInfo(50), a_sLabel, IM_COL32(236, 239, 244, 255));
	}

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeNameRow(const std::string& a_sLabel)
	{
		return std::make_unique<TextRowEntry>(RowInfo(5), a_sLabel, IM_COL32(236, 239, 244, 255));
	}

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeCountRow(const std::string& a_sLabel)
	{
		return std::make_unique<TextRowEntry>(RowInfo(50, RowInfoTextAlignment::Right), a_sLabel, IM_COL32(236, 239, 244, 255));
	}

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeIconRow(const std::string& a_sLabel)
	{
		return std::make_unique<IconRowEntry>(RowInfo(4), a_sLabel, ImGui::GetFontSize() + 32.0f);
	}

	template<typename... Args>
	inline std::vector<std::unique_ptr<RowEntry>> MakeRows(Args... args)
	{
		std::vector<std::unique_ptr<RowEntry>> rows;
		(rows.push_back(std::move(args)), ...);
		return rows;
	}
}