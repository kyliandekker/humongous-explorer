#include "./SummaryCommand.h"

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
			{ "csv",    "Output as CSV instead of table", ArgType::Flag, false, "" },
		};
	}

	//---------------------------------------------------------------------
	int SummaryCommand::Execute(int a_Argc, char* a_Argv[])
	{
		CommandParser parser(GetArgs());
		if (!parser.Parse(a_Argc, a_Argv))
		{
			parser.PrintUsage(GetName(), GetDescription());
			return 1;
		}

		const auto& files = parser.GetFiles();
		bool csvMode = parser.Has("csv");

		int failures = 0;

		for (const std::string& filePath : files)
		{
			archive::Archive archive;
			core::LoadResult result = archive.Load(filePath);

			if (result.status != core::LoadStatus::Success)
			{
				core::Log(core::LogLevel::Error, "Failed to load " + filePath + ": " + result.errorMessage);
				failures++;
				continue;
			}

			std::unordered_map<std::string, size_t> counts;
			CountChunks(archive.GetRoot(), counts);

			fs::path stem = fs::path(filePath).stem();
			fs::path parent = fs::path(filePath).parent_path();
			std::string ext = csvMode ? ".csv" : ".table";
			std::string outPath = (parent / stem).string() + ext;

			std::vector<std::pair<std::string, size_t>> sorted(counts.begin(), counts.end());
			std::sort(sorted.begin(), sorted.end(), [](const auto& a, const auto& b) { return a.second > b.second; });

			FILE* file = nullptr;
			fopen_s(&file, outPath.c_str(), "w");
			if (!file)
			{
				core::Log(core::LogLevel::Error, "Failed to create " + outPath + ".");
				failures++;
				continue;
			}

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
			core::Log(core::LogLevel::Success, "Saved " + outPath + ".");
		}

		return failures;
	}
}
