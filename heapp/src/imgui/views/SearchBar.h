#pragma once

#include <string>

namespace humongousexplorer::imgui
{
	class SearchBar
	{
	public:
		SearchBar(const std::string& a_sName, const std::string& a_sHint) :
			m_sName(a_sName),
			m_sHint(a_sHint)
		{}

		bool Render();

		std::string GetText() const { return m_aBuffer; }
		bool HasText() const { return m_aBuffer[0] != '\0'; }
	private:
		std::string m_sHint;
		std::string m_sName;
		std::string m_sIconPath = "icon_search.svg";
		char m_aBuffer[256] = "";
	};
}