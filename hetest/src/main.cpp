#include <cassert>

#include "core/Data.h"
#include "core/DataStream.h"

#include "script/OPCodesHE.h"
#include "script/ScrInstruction.h"
#include "script/ScrArgumentType.h"
#include "script/ScrResource.h"

#include "file/file.h"

using namespace humongousexplorer::script;
using namespace humongousexplorer;

int main()
{
	std::string testDataPath = "./testdata/test.data";

	core::Data data;
	if (!file::LoadFile(testDataPath, data))
	{
		printf("Could not find file: \"%s\".", testDataPath.c_str());
		return 0;
	}

	OPCodeMap map;
	GetOPCodeTable(map, 6, 98);

	ScrResource script;
	if (!script.Parse(data, map))
	{
		printf("Could not parse script data: \"%s\".", testDataPath.c_str());
		return 0;
	}

	core::DataStream newData;
	if (!script.Build(newData, map))
	{
		printf("Could not build script data.");
		return 0;
	}

	std::string saveTestDataPath = "./testdata/test.test";
	if (!file::SaveFile(saveTestDataPath, newData))
	{
		printf("Could not save file: \"%s\".", saveTestDataPath.c_str());
		return 0;
	}

	return 0;
}