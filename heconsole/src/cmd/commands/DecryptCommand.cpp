#include "DecryptCommand.h"

#include <filesystem>
#include <string>

#include "archive/Archive.h"
#include "core/Log.h"
#include "core/DataStream.h"
#include "file/file.h"

#include "cmd/CommandParser.h"

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	const char* DecryptCommand::GetName() const
	{
		return "decrypt";
	}

	//---------------------------------------------------------------------
	const char* DecryptCommand::GetDescription() const
	{
		return "Decrypt a file and export it";
	}

	//---------------------------------------------------------------------
	std::vector<CommandArg> DecryptCommand::GetArgs() const
	{
		return {
			{ "files",  "Archive file to convert",			ArgType::File,   true,  "" },
			{ "output", "Output file or directory",			ArgType::Option, false, "" },
		};
	}

	//---------------------------------------------------------------------
	int DecryptCommand::Execute(int a_iArgc, char* a_sArgv[])
	{
		CommandParser parser(GetArgs());
		if (!parser.Parse(a_iArgc, a_sArgv))
		{
			parser.PrintUsage(GetName(), GetDescription());
			return 1;
		}

		const auto& files = parser.GetFiles();

		fs::path output;
		bool hasOutput = parser.Has("output");
		if (hasOutput)
		{
			output = parser.Get("output");
		}

		int failures = 0;
		for (const fs::path& filePath : files)
		{
			archive::Archive archive;
			core::LoadResult result = archive.Load(filePath);

			if (result.status != core::LoadStatus::Success)
			{
				core::Log(core::LogLevel::Error, "Failed to load: \"" + filePath.string() + "\" because: " + result.errorMessage);
				failures++;
				continue;
			}

			fs::path outputPath;
			if (hasOutput)
			{
				outputPath = output;
				if (fs::is_directory(outputPath))
				{
					fs::create_directories(outputPath);
				}
			}
			else
			{
				outputPath = filePath.parent_path();
				fs::create_directories(outputPath);
			}

			fs::path outputFile = outputPath.string() + "/" + filePath.stem().string() + "_decrypted" + filePath.extension().string();

			core::DataStream dataStream;
			archive.Build(dataStream, false);

			if (!file::SaveFile(outputFile, dataStream))
			{
				core::Log(core::LogLevel::Error, "Could not save file: \"" + outputFile.string() + "\".");
				failures++;
				continue;
			}

			core::Log(core::LogLevel::Success, "Created: \"" + outputFile.string() + "\".");
		}

		return failures;
	}
}
