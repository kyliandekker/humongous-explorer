#include "core/Log.h"
#include "archive/ArchiveSet.h"

#include "script/OPCodesHE.h"
#include "script/ScrResource.h"

#include "core/DataStream.h"

using namespace humongousexplorer;

int main()
{
	core::InitializeLog();

	archive::ArchiveSet set;
	bool loaded = set.LoadArchives("C:/Program Files (x86)/Steam/steamapps/common/Spy Fox 3/SPYOZON.HE2");

	if (!loaded)
	{
		return 1;
	}

	std::string testDataPath = "./testdata/test.data";

	core::Data data;
	if (!file::LoadFile(testDataPath, data))
	{
		core::Log(core::LogLevel::Error, "Could not find file: \"" + testDataPath + "\".");
		return 0;
	}

	script::OPCodeMap map;
	script::GetOPCodeTable(map, 6, 98);

	script::ScrResource script;
	if (!script.Parse(data, map))
	{
		core::Log(core::LogLevel::Error, "Could not parse script data: \"" + testDataPath + "\".");
		return 0;
	}

	core::DataStream newData;
	if (!script.Build(newData, map))
	{
		core::Log(core::LogLevel::Error, "Could not build script data.");
		return 0;
	}

	std::string saveTestDataPath = "./testdata/test.test";
	if (!file::SaveFile(saveTestDataPath, newData))
	{
		core::Log(core::LogLevel::Error, "Could not save script file: \"" + saveTestDataPath + "\".");
		return 0;
	}

	core::Log(core::LogLevel::Success, "Parsed, built and saved script file: \"" + saveTestDataPath + "\".");

	core::DestroyLog();

	return 0;
}
