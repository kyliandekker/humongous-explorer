#include "./ArchiveContentsWindow.h"

#include <imgui/imgui.h>
#include <string>

#include "imgui/ImGuiSetup.h"
#include "imgui/views/FileEntryView.h"

namespace humongousexplorer::imgui
{
	static std::vector<std::unique_ptr<TreeFileEntryView>> s_aArchives;

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
		s_aArchives.push_back(std::make_unique<TreeFileEntryView>(
			MakeRows(MakeIconRow("../icons/icon_a.svg"), MakeNameRow("SPYOZON.(a)"), MakeCountRow("200"))
		));

		s_aArchives.push_back(std::make_unique<TreeFileEntryView>(
			MakeRows(MakeIconRow("../icons/icon_he0.svg"), MakeNameRow("SPYOZON.HE0"), MakeCountRow("200")),
			[]()
			{
				std::vector<std::unique_ptr<FileEntryView>> children;
				children.push_back(std::make_unique<TreeFileEntryView>(MakeRows(MakeNameRow("Helogo"))));
				children.push_back(std::make_unique<TreeFileEntryView>(MakeRows(MakeNameRow("Interface"))));
				children.push_back(std::make_unique<TreeFileEntryView>(MakeRows(MakeNameRow("Saveload"))));
				children.push_back(std::make_unique<TreeFileEntryView>(MakeRows(MakeNameRow("Spywatch"))));
				children.push_back(std::make_unique<TreeFileEntryView>(MakeRows(MakeNameRow("Mobcom"))));
				return children;
			}()
		));

		s_aArchives.push_back(std::make_unique<TreeFileEntryView>(
			MakeRows(MakeIconRow("../icons/icon_he2.svg"), MakeNameRow("SPYOZON.HE2"), MakeCountRow("200"))
		));

		s_aArchives.push_back(std::make_unique<TreeFileEntryView>(
			MakeRows(MakeIconRow("../icons/icon_he4.svg"), MakeNameRow("SPYOZON.HE4"), MakeCountRow("200"))
		));

		return BaseWindow::Initialize();
	}

	//---------------------------------------------------------------------
	void ArchiveContentsWindow::Update()
	{
		for (const std::unique_ptr<TreeFileEntryView>& view : s_aArchives)
		{
			FileEntryInteractionType interaction = view->Render([this, &view]()
				{
					return m_pFilterFileEntryView == view.get();
				});

			if (interaction == FileEntryInteractionType::LeftClicked)
			{
				m_pFilterFileEntryView = view.get();
			}
		}
	}
}
