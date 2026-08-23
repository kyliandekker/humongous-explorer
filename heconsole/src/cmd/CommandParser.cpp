#include "CommandParser.h"

#include <cstdio>
#include <cctype>

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	// CommandParser
	//---------------------------------------------------------------------
	CommandParser::CommandParser(const std::vector<CommandArg>& a_aArgs) :
		m_aArgs(a_aArgs)
	{
		for (const CommandArg& arg : m_aArgs)
		{
			if (arg.type == ArgType::Option)
			{
				m_mOptions[arg.name] = arg.defaultValue;
				m_mProvided[arg.name] = false;
			}
			else if (arg.type == ArgType::Flag)
			{
				m_mProvided[arg.name] = false;
			}
		}
	}

	//---------------------------------------------------------------------
	bool CommandParser::Parse(int a_iArgc, char* a_sArgv[])
	{
		m_aFiles.clear();
		m_bValid = true;

		for (size_t i = 0; i < a_iArgc; ++i)
		{
			std::string arg = a_sArgv[i];

			if (arg.starts_with("--help"))
			{
				m_bValid = false;
				return false;
			}
			else if (arg.starts_with("--"))
			{
				std::string name;
				std::string value;

				size_t eqPos = arg.find('=', 2);
				if (eqPos != std::string::npos)
				{
					size_t firstQuotePos = eqPos + 1;
					size_t lastQuotePos = arg.substr(firstQuotePos).find('\0');

					name = arg.substr(2, eqPos - 2);
					value = arg.substr(firstQuotePos, lastQuotePos);
					bool found = false;
					for (const CommandArg& cmdArg : m_aArgs)
					{
						if (cmdArg.name == name)
						{
							if (cmdArg.type == ArgType::Flag)
							{
								m_mProvided[name] = true;
							}
							else if (cmdArg.type == ArgType::Option)
							{
								m_mOptions[name] = value;
								m_mProvided[name] = true;
							}
							found = true;
							break;
						}
					}

					if (!found)
					{
						printf("Unknown option: --%s\n", name.c_str());
						m_bValid = false;
					}
				}
			}
			else
			{
				m_aFiles.push_back(arg);
			}
		}

		for (const CommandArg& arg : m_aArgs)
		{
			if (arg.type == ArgType::File && arg.required && m_aFiles.empty())
			{
				printf("Missing required argument: %s\n", arg.name.c_str());
				m_bValid = false;
			}
		}

		return m_bValid;
	}

	//---------------------------------------------------------------------
	const std::vector<fs::path>& CommandParser::GetFiles() const
	{
		return m_aFiles;
	}

	//---------------------------------------------------------------------
	bool CommandParser::Has(const std::string& a_sName) const
	{
		auto it = m_mProvided.find(a_sName);
		return it != m_mProvided.end() && it->second;
	}

	//---------------------------------------------------------------------
	std::string CommandParser::Get(const std::string& a_sName) const
	{
		auto it = m_mOptions.find(a_sName);
		if (it != m_mOptions.end())
		{
			return it->second;
		}
		return "";
	}

	//---------------------------------------------------------------------
	bool CommandParser::IsValid() const
	{
		return m_bValid;
	}

	//---------------------------------------------------------------------
	void CommandParser::PrintUsage(const char* a_sCommandName, const char* a_sDescription) const
	{
		if (a_sDescription[0] != '\0')
		{
			printf("%s\n\n", a_sDescription);
		}
		printf("Usage: heconsole %s", a_sCommandName);

		bool hasFiles = false;
		bool hasOptions = false;

		for (const CommandArg& arg : m_aArgs)
		{
			if (arg.type == ArgType::Option || arg.type == ArgType::Flag)
			{
				hasOptions = true;
			}
			if (arg.type == ArgType::File)
			{
				hasFiles = true;
			}
		}

		if (hasFiles)
		{
			for (const CommandArg& arg : m_aArgs)
			{
				if (arg.type == ArgType::File)
				{
					if (arg.required)
					{
						printf(" <%s...>", arg.name.c_str());
					}
					else
					{
						printf(" [%s...]", arg.name.c_str());
					}
				}
			}
		}
		if (hasOptions)
		{
			printf(" [options]");
		}
		printf("\n\n");

		bool printedFiles = false;
		for (const CommandArg& arg : m_aArgs)
		{
			if (arg.type == ArgType::File)
			{
				if (!printedFiles)
				{
					printf("Arguments:\n");
					printedFiles = true;
				}
				printf("  %-13s %s%s\n", (arg.required ? "<" + arg.name + ">" : "[" + arg.name + "]").c_str(), arg.description.c_str(), arg.required ? " (required)" : "");
			}
		}

		bool printedOptions = false;
		for (const CommandArg& arg : m_aArgs)
		{
			if (arg.type == ArgType::Option || arg.type == ArgType::Flag)
			{
				if (!printedOptions)
				{
					if (printedFiles)
					{
						printf("\n");
					}
					printf("Options:\n");
					printedOptions = true;
				}
				if (arg.type == ArgType::Flag)
				{
					printf("  --%-10s %s\n", arg.name.c_str(), arg.description.c_str());
				}
				else
				{
					std::string defaultValue = arg.defaultValue.empty() ? "" : " (default: " + arg.defaultValue + ")";
					printf("  --%-10s %s%s\n", arg.name.c_str(), arg.description.c_str(), defaultValue.c_str());
				}
			}
		}
	}
}
