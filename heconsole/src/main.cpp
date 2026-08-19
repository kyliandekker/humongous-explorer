#include <tinyxml/tinyxml2.h>

#include "xml/XMLCreator.h"

#include "core/Log.h"

#include "archive/ArchiveSet.h"

int main()
{
	humongousexplorer::core::InitializeLog();

	humongousexplorer::archive::ArchiveSet set;
	bool loaded = set.LoadArchives("C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/SPYOZON.HE2");

	humongousexplorer::core::DestroyLog();

	tinyxml2::XMLDocument doc;
	humongousexplorer::xml::CreateXMLFromArchive(*set.GetArchive(3), doc);

	doc.SaveFile("./test.xml");

	return 0;
}