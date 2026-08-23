#include "SummaryCommand.h"

#include <cstdio>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <vector>

#include "archive/Archive.h"
#include "core/Log.h"
#include "parsing/Chunk.h"

#include "../CommandParser.h"

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	static void CountChunks(const parsing::Chunk& a_Chunk, std::unordered_map<std::string, size_t>& a_Counts)
	{
		for (const std::unique_ptr<parsing::Chunk>& child : a_Chunk.GetChildren())
		{
			if (!child)
			{
				continue;
			}

			a_Counts[child->GetTag()]++;
			CountChunks(*child, a_Counts);
		}
	}

	//---------------------------------------------------------------------
	const char* SummaryCommand::GetName() const
	{
		return "summary";
	}

	//---------------------------------------------------------------------
	const char* SummaryCommand::GetDescription() const
	{
		return "Display chunk statistics for archive files";
	}

	//---------------------------------------------------------------------
	std::vector<CommandArg> SummaryCommand::GetArgs() const
	{
		return {
			{ "files",  "Archive files to analyze",   ArgType::File,   true,  "" },
			{ "output", "Output file or directory",			ArgType::Option, false, "" },
			{ "csv",    "Output as CSV instead of table", ArgType::Flag, false, "" },
		};
	}

	//---------------------------------------------------------------------
	int SummaryCommand::Execute(int a_iArgc, char* a_sArgv[])
	{
		CommandParser parser(GetArgs());
		if (!parser.Parse(a_iArgc, a_sArgv))
		{
			parser.PrintUsage(GetName(), GetDescription());
			return 1;
		}

		const auto& files = parser.GetFiles();
		bool csvMode = parser.Has("csv");

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
			if (!archive.Load(filePath))
			{
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

			std::string ext = csvMode ? ".csv" : ".table";
			fs::path outputFile = outputPath.string() + "/" + filePath.filename().string() + ext;

			FILE* file = nullptr;
			fopen_s(&file, outputFile.generic_string().c_str(), "w");
			if (!file)
			{
				core::Log(core::LogLevel::Error, "Failed to create \"" + outputFile.generic_string() + "\".");
				failures++;
				continue;
			}

			std::unordered_map<std::string, size_t> counts;
			CountChunks(archive.GetRoot(), counts);

			std::vector<std::pair<std::string, size_t>> sorted(counts.begin(), counts.end());
			std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

			if (csvMode)
			{
				fprintf(file, "Tag,Count\n");
				for (const auto& [tag, count] : sorted)
				{
					fprintf(file, "%s,%zu\n", tag.c_str(), count);
				}
			}
			else
			{
				size_t total = 0;
				for (const auto& [tag, count] : counts)
				{
					total += count;
				}

				fprintf(file, "%s  (%zu chunks)\n", fs::path(filePath).filename().string().c_str(), total);

				for (const auto& [tag, count] : sorted)
				{
					fprintf(file, "  %-8s %zu\n", tag.c_str(), count);
				}
			}

			fclose(file);
			core::Log(core::LogLevel::Success, "Created: \"" + outputFile.string() + "\".");
		}

		return failures;
	}
}
