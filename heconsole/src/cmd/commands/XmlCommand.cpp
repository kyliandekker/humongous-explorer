#include "XmlCommand.h"

#include <filesystem>
#include <string>

#include "tinyxml/tinyxml2.h"

#include "archive/Archive.h"
#include "core/Log.h"
#include "xml/XMLCreator.h"
#include "xml/XMLStruct.h"

#include "cmd/CommandParser.h"

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	const char* XmlCommand::GetName() const
	{
		return "xml";
	}

	//---------------------------------------------------------------------
	const char* XmlCommand::GetDescription() const
	{
		return "Export archive files as XML";
	}

	//---------------------------------------------------------------------
	std::vector<CommandArg> XmlCommand::GetArgs() const
	{
		return {
			{ "files",  "Archive files to convert",        ArgType::File,   true,  "" },
			{ "depth",  "Max chunk tree depth",            ArgType::Option, false, "" },
			{ "output", "Output file or directory",        ArgType::Option, false, "" },
		};
	}

	//---------------------------------------------------------------------
	int XmlCommand::Execute(int a_iArgc, char* a_sArgv[])
	{
		CommandParser parser(GetArgs());
		if (!parser.Parse(a_iArgc, a_sArgv))
		{
			parser.PrintUsage(GetName(), GetDescription());
			return 1;
		}

		const auto& files = parser.GetFiles();

		int maxDepth = -1;
		if (parser.Has("depth"))
		{
			maxDepth = std::stoi(parser.Get("depth"));
		}

		std::string output = parser.Get("output");
		bool hasOutput = parser.Has("output");

		bool multiFile = files.size() > 1;

		int failures = 0;

		for (const fs::path& filePath : files)
		{
			archive::Archive archive;
			core::LoadResult result = archive.Load(filePath);

			if (result.status != core::LoadStatus::Success)
			{
				core::Log(core::LogLevel::Error, "Failed to load " + filePath.string() + ": " + result.errorMessage);
				failures++;
				continue;
			}

			tinyxml2::XMLDocument doc;
			xml::XMLStruct xmlInfo;
			xmlInfo.m_iMaxDepth = maxDepth;
			xml::CreateXMLFromArchive(archive, doc, xmlInfo);

			fs::path inputPath(filePath);
			fs::path outputPath;

			if (hasOutput)
			{
				fs::path outputArg(output);

				if (multiFile || fs::is_directory(outputArg))
				{
					fs::create_directories(outputArg);
					outputPath = outputArg / (inputPath.filename().string() + ".xml");
				}
				else
				{
					fs::path parent = outputArg.parent_path();
					if (!parent.empty())
					{
						fs::create_directories(parent);
					}
					outputPath = outputArg;
				}
			}
			else
			{
				outputPath = inputPath += ".xml";
			}

			tinyxml2::XMLError xmlResult = doc.SaveFile(outputPath.string().c_str());
			if (xmlResult != tinyxml2::XML_SUCCESS)
			{
				core::Log(core::LogLevel::Error, "Failed to save " + outputPath.string());
				failures++;
				continue;
			}

			core::Log(core::LogLevel::Success, "Created " + outputPath.string());
		}

		return failures;
	}
}
