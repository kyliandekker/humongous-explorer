#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "imgui/ImGuiSetup.h"
#include "imgui/FileEntryView.h"

namespace humongousexplorer::imgui
{
	static std::vector<TreeFileEntryView> s_aArchives;

	//---------------------------------------------------------------------
	// ArchiveContentsWindow
	//---------------------------------------------------------------------
	ArchiveContentsWindow::ArchiveContentsWindow()
		: BaseWindow(ImGuiWindowFlags_NoCollapse, "ARCHIVE CONTENTS", "ArchiveContentsWindow")
	{
	}

	//---------------------------------------------------------------------
	bool imgui::ArchiveContentsWindow::Initialize()
	{
		s_aArchives.push_back(TreeFileEntryView(
			MakeRows(MakeIconRow("../icons/icon_a.svg"), MakeNameRow("SPYOZON.(a)"), MakeCountRow("200"))
		));
		
		s_aArchives.push_back(TreeFileEntryView(
			MakeRows(MakeIconRow("../icons/icon_he0.svg"), MakeNameRow("SPYOZON.HE0"), MakeCountRow("200"))
		));
		
		s_aArchives.push_back(TreeFileEntryView(
			MakeRows(MakeIconRow("../icons/icon_he2.svg"), MakeNameRow("SPYOZON.HE2"), MakeCountRow("200"))
		));
		
		s_aArchives.push_back(TreeFileEntryView(
			MakeRows(MakeIconRow("../icons/icon_he4.svg"), MakeNameRow("SPYOZON.HE4"), MakeCountRow("200"))
		));

		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::Update()
	{
		for (TreeFileEntryView& view : s_aArchives)
		{
			FileEntryInteractionType interaction = view.Render([this, &view]()
				{
					return m_pFilterFileEntryView == &view;
				});

			if (interaction == FileEntryInteractionType::LeftClicked)
			{
				m_pFilterFileEntryView = &view;
			}
		}
	}
}