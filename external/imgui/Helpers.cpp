#include "Helpers.h"

#include <imgui_internal.h>
#include <cmath>

//---------------------------------------------------------------------
std::string ToUpperSnakeCase(const std::string& a_String)
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

//---------------------------------------------------------------------
bool TextButton(const std::string& a_Label, const ImVec2& a_Size, const ImVec4& a_Color)
{
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 btnSize = a_Size;

	ImVec4 invisible(0, 0, 0, 0);
	ImGui::PushStyleColor(ImGuiCol_Text, invisible);
	bool b = ImGui::Button(a_Label.c_str(), btnSize);
	ImGui::PopStyleColor();

	const char* display = a_Label.c_str();
	const char* sep = strstr(a_Label.c_str(), "###");
	std::string displayText;
	if (sep)
	{
		displayText = a_Label.substr(0, sep - a_Label.c_str());
		display = displayText.c_str();
	}

	ImVec2 textSize = ImGui::CalcTextSize(display);
	ImVec2 textPos(
		pos.x + (btnSize.x - textSize.x) * 0.5f,
		pos.y + (btnSize.y - textSize.y) * 0.5f);

	ImGui::PushStyleColor(ImGuiCol_Text, a_Color);
	ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), display);
	ImGui::PopStyleColor();

	return b;
}

//---------------------------------------------------------------------
bool Knob(char const* label, float* p_value, float v_min, float v_max, ImVec2 const& size, float default_value)
{
	bool showLabel = label[0] != '#' && label[1] != '#' && label[0] != '\0';

	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 s(size.x - 4, size.y - 4);

	float radius_outer = std::fmin(s.x, s.y) / 2.0f;
	ImVec2 pos = ImGui::GetCursorScreenPos();
	pos = ImVec2(pos.x + 2, pos.y + 2);
	ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);

	float line_height = ImGui::GetTextLineHeight();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float ANGLE_MIN = 3.141592f * 0.70f;
	float ANGLE_MAX = 3.141592f * 2.30f;

	if (s.x != 0.0f && s.y != 0.0f)
	{
		center.x = pos.x + (s.x / 2.0f);
		center.y = pos.y + (s.y / 2.0f);
		ImGui::InvisibleButton(label, ImVec2(s.x, s.y + (showLabel ? line_height + style.ItemInnerSpacing.y : 0)));
	}
	else
	{
		ImGui::InvisibleButton(label, ImVec2(radius_outer * 2, radius_outer * 2 + (showLabel ? line_height + style.ItemInnerSpacing.y : 0)));
	}
	bool value_changed = false;
	bool is_active = ImGui::IsItemActive();
	bool is_hovered = ImGui::IsItemActive();
	bool double_clicked = ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);

	if (is_active && double_clicked)
	{
		*p_value = default_value;
		value_changed = true;
		return value_changed;
	}

	if (is_active && io.MouseDelta.y != 0.0f)
	{
		float step = (v_max - v_min) / 200.0f;
		*p_value -= io.MouseDelta.y * step;
		if (*p_value < v_min)
			*p_value = v_min;
		if (*p_value > v_max)
			*p_value = v_max;
		value_changed = true;
	}

	float angle = ANGLE_MIN + (ANGLE_MAX - ANGLE_MIN) * (*p_value - v_min) / (v_max - v_min);
	float angle_cos = cosf(angle);
	float angle_sin = sinf(angle);

	draw_list->AddCircleFilled(center, radius_outer * 0.7f, ImGui::GetColorU32(ImGuiCol_Button), 16);
	draw_list->PathArcTo(center, radius_outer, ANGLE_MIN, ANGLE_MAX, 16);
	draw_list->PathStroke(ImGui::GetColorU32(ImVec4(0.25f, 0.25f, 0.25f, 1.0f)), false, 3.0f);
	draw_list->AddLine(
		ImVec2(center.x + angle_cos * (radius_outer * 0.35f), center.y + angle_sin * (radius_outer * 0.35f)),
		ImVec2(center.x + angle_cos * (radius_outer * 0.7f), center.y + angle_sin * (radius_outer * 0.7f)),
		ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
	draw_list->PathArcTo(center, radius_outer, ANGLE_MIN, angle + 0.02f, 16);
	draw_list->PathStroke(ImGui::GetColorU32(ImGuiCol_SliderGrab), false, 3.0f);

	if (showLabel)
	{
		std::string lbl = std::string(label);
		std::size_t lbl_pos = lbl.find("#");
		std::string label_text = lbl.substr(0, lbl_pos);
		auto textSize = ImGui::CalcTextSize(label_text.c_str());
		draw_list->AddText(ImVec2(pos.x + ((size.x / 2) - (textSize.x / 2)), pos.y + radius_outer * 2 + style.ItemInnerSpacing.y), ImGui::GetColorU32(ImGuiCol_Text), label_text.c_str());
	}

	if (is_active || is_hovered)
	{
		ImGui::SetNextWindowPos(ImVec2(pos.x - style.WindowPadding.x, pos.y - (line_height * 2) - style.ItemInnerSpacing.y - style.WindowPadding.y));
		ImGui::BeginTooltip();
		if (showLabel)
		{
			ImGui::Text("Value: \n%.3f", static_cast<double>(*p_value));
		}
		else
		{
			ImGui::Text("%.3f", static_cast<double>(*p_value));
		}
		ImGui::EndTooltip();
	}

	return value_changed;
}