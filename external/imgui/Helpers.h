#pragma once

#include <vector>
#include <string>
#include <imgui.h>

#define POPUP_WINDOW_ID "POPUP"
#define POPUP_WINDOW_BUTTON_ID "POPUP_DIALOG_SAVE_BUTTON"
#define CHILD_ID "CHILD"
#define BUTTON_ID "BUTTON"
#define COMBO_ID "COMBO"
#define WINDOW_ID "WINDOW"
#define MENU_ITEM_ID "MENU_ITEM"
#define TREE_NODE_ID "TREE_NODE"
#define MENU_ID "MENU_"
#define INPUT_ID "INPUT_"
#define CHECKBOX_ID "CHECKBOX"
#define SWITCH_ID "SWITCH"
#define FOLD_ID "FOLD"
#define SLIDER_ID "SLIDER"
#define DRAG_ITEM_ID "DRAG_ITEM"
#define SELECTABLE_ID "SELECTABLE"
#define FOLDOUT_ID "FOLDOUT"
#define DROPDOWN_ID "DROPDOWN"
#define COLOR_WHEEL_ID "COLOR_WHEEL"
#define TABLE_ID "TABLE"
#define TAB_ID "TAB"
#define PLOT_ID "PLOT"
#define SEARCHBAR_ID "SEARCHBAR"
#define KNOB_ID "KNOB"

#ifdef _DEBUG
	//---------------------------------------------------------------------
	std::string ToUpperSnakeCase(const std::string& a_String);

	//---------------------------------------------------------------------
	template<typename... TArgs>
	inline std::string FormatId(const std::string& a_sUIName, const TArgs&... a_Args)
	{
		std::string id;

		bool first = true;

		auto appendPart = [&](const auto& part)
			{
				if (!first)
				{
					id += '_';
				}

				id += ToUpperSnakeCase(std::string(part));
				first = false;
			};

		(appendPart(a_Args), ...);

		std::string formatted = a_sUIName + "###" + id;

		return formatted;
	}
	
	
	//---------------------------------------------------------------------
	bool TextButton(const std::string& a_Label, const ImVec2& a_Size = ImVec2(), const ImVec4& a_Color = ImVec4(1, 1, 1, 1));

	//---------------------------------------------------------------------
	bool Knob(char const* label, float* p_value, float v_min, float v_max, ImVec2 const& size, float default_value);

#endif