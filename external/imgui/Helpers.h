#include <vector>
#include <string>

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
#define PLOT_ID "PLOT"

#ifdef _DEBUG
	inline std::string ToUpperSnakeCase(const std::string& a_String)
	{
		std::string result;

		for (size_t i = 0; i < a_String.size(); ++i)
		{
			const char current = a_String[i];

			if (
				(i > 0) &&
				std::islower(static_cast<unsigned char>(a_String[i - 1])) &&
				std::isupper(static_cast<unsigned char>(current))
				)
			{
				result += '_';
			}

			result += static_cast<char>(
				std::toupper(static_cast<unsigned char>(current))
				);
		}

		return result;
	}

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
#endif