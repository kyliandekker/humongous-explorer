#pragma once

#include <vector>
#include <string>
#include <functional>
#include <memory>
#include <imgui/imgui.h>
#include <imgui_internal.h>

namespace humongousexplorer::parsing
{
	struct Chunk;
}

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
		{
		}

		virtual ~RowEntry() = default;

		RowInfo m_RowInfo;

		virtual void Render(const ImVec2& a_vPos) = 0;
		virtual ImVec2 GetSize() const = 0;

		virtual bool Find(const std::string& a_sObjective) const = 0;
	};

	//---------------------------------------------------------------------
	struct TextRowEntry : public RowEntry
	{
		TextRowEntry(RowInfo a_RowInfo, const std::string& a_sText, ImColor a_Color) : RowEntry(a_RowInfo),
			m_sText(a_sText),
			m_Color(a_Color)
		{
		}

		std::string m_sText;
		ImColor m_Color;

		void Render(const ImVec2& a_vPos) override;
		ImVec2 GetSize() const override;
		bool Find(const std::string& a_sObjective) const override;
	};

	//---------------------------------------------------------------------
	struct IconRowEntry : public RowEntry
	{
		IconRowEntry(RowInfo a_RowInfo, const std::string& a_sIconName, float a_fSize = 0) : RowEntry(a_RowInfo),
			m_sIconName(a_sIconName),
			m_fSize(a_fSize)
		{
		}

		float m_fSize = 0;
		std::string m_sIconName;

		void Render(const ImVec2& a_vPos) override;
		ImVec2 GetSize() const override;
		bool Find(const std::string& a_sObjective) const override;
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
		{
		}

		bool m_bVisible = true;
		std::vector<std::unique_ptr<RowEntry>> m_aRows;

		virtual void Render(std::function<bool(FileEntryView* fileEntry)> a_fnSelected, std::function<void(FileEntryInteractionType, FileEntryView*)> a_fnOnInteraction, float a_fIndent = 0.0f);
		virtual bool Filter(const std::string& a_sObjective);
	};

	//---------------------------------------------------------------------
	struct TreeFileEntryView : public FileEntryView
	{
		TreeFileEntryView(std::vector<std::unique_ptr<RowEntry>> a_aRows, std::vector<std::unique_ptr<FileEntryView>> a_aChildren = {}) : FileEntryView(std::move(a_aRows)),
			m_aChildren(std::move(a_aChildren))
		{
		}

		std::vector<std::unique_ptr<FileEntryView>> m_aChildren;
		bool m_bExpanded = false;
		const parsing::Chunk* m_pChunk = nullptr;

		void Render(std::function<bool(FileEntryView* fileEntry)> a_fnSelected, std::function<void(FileEntryInteractionType, FileEntryView*)> a_fnOnInteraction, float a_fIndent = 0.0f) override;
		bool Filter(const std::string& a_sObjective) override;
	};

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeTextRow(const RowInfo& a_RowInfo, const std::string& a_sLabel)
	{
		return std::make_unique<TextRowEntry>(RowInfo(50), a_sLabel, IM_COL32(236, 239, 244, 255));
	}

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeNameRow(const std::string& a_sLabel)
	{
		return std::make_unique<TextRowEntry>(RowInfo(25), a_sLabel, IM_COL32(236, 239, 244, 255));
	}

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeCountRow(const std::string& a_sLabel)
	{
		return std::make_unique<TextRowEntry>(RowInfo(50, RowInfoTextAlignment::Right), a_sLabel, IM_COL32(236, 239, 244, 255));
	}

	//---------------------------------------------------------------------
	inline std::unique_ptr<RowEntry> MakeIconRow(const std::string& a_sLabel)
	{
		return std::make_unique<IconRowEntry>(RowInfo(4), a_sLabel, ImGui::GetFontSize() + 5);
	}

	template<typename... Args>
	inline std::vector<std::unique_ptr<RowEntry>> MakeRows(Args... args)
	{
		std::vector<std::unique_ptr<RowEntry>> rows;
		(rows.push_back(std::move(args)), ...);
		return rows;
	}
}