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

			fs::path outputFile = outputPath.string() + "/" + filePath.filename().generic_string() + ".xml";

			tinyxml2::XMLDocument doc;
			xml::XMLStruct xmlInfo;
			xmlInfo.m_iMaxDepth = maxDepth;
			xml::CreateXMLFromArchive(archive, doc, xmlInfo);

			tinyxml2::XMLError xmlResult = doc.SaveFile(outputFile.string().c_str());
			if (xmlResult != tinyxml2::XML_SUCCESS)
			{
				core::Log(core::LogLevel::Error, "Failed to save " + outputFile.string());
				failures++;
				continue;
			}

			core::Log(core::LogLevel::Success, "Created: \"" + outputFile.string() + "\".");
		}

		return failures;
	}
}
