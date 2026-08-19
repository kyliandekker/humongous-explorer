#include "core/Log.h"
#include "archive/ArchiveSet.h"

using namespace humongousexplorer;

int main()
{
	core::InitializeLog();

	archive::ArchiveSet set;
	bool loaded = set.LoadArchives("C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/SPYOZON.HE2");

	core::DestroyLog();

	if (!loaded)
	{
		return 1;
	}

	return 0;
}
