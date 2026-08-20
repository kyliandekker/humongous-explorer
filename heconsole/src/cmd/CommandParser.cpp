#include "./CommandParser.h"

#include <cstdio>

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	CommandParser::CommandParser(const std::vector<CommandArg>& a_Args) :
		m_Args(a_Args)
	{
		for (const CommandArg& arg : m_Args)
		{
			if (arg.type == ArgType::Option)
			{
				m_Options[arg.name] = arg.defaultValue;
				m_Provided[arg.name] = false;
			}
			else if (arg.type == ArgType::Flag)
			{
				m_Provided[arg.name] = false;
			}
		}
	}

	//---------------------------------------------------------------------
	bool CommandParser::Parse(int a_Argc, char* a_Argv[])
	{
		m_Files.clear();
		m_bValid = true;

		for (int i = 0; i < a_Argc; i++)
		{
			std::string arg = a_Argv[i];

			if (arg == "--help")
			{
				m_bValid = false;
				return false;
			}

			if (arg.starts_with("--"))
			{
				std::string name;
				std::string value;
				bool hasInlineValue = false;

				size_t eqPos = arg.find('=', 2);
				if (eqPos != std::string::npos)
				{
					name = arg.substr(2, eqPos - 2);
					value = arg.substr(eqPos + 1);
					hasInlineValue = true;
				}
				else
				{
					name = arg.substr(2);
				}

				bool found = false;
				for (const CommandArg& cmdArg : m_Args)
				{
					if (cmdArg.name == name)
					{
						if (cmdArg.type == ArgType::Flag)
						{
							m_Provided[name] = true;
						}
						else if (cmdArg.type == ArgType::Option)
						{
							if (hasInlineValue)
							{
								m_Options[name] = value;
								m_Provided[name] = true;
							}
							else if (i + 1 < a_Argc)
							{
								m_Options[name] = a_Argv[i + 1];
								m_Provided[name] = true;
								i++;
							}
							else
							{
								printf("Option --%s requires a value.\n", name.c_str());
								m_bValid = false;
							}
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
			else
			{
				m_Files.push_back(arg);
			}
		}

		for (const CommandArg& arg : m_Args)
		{
			if (arg.type == ArgType::File && arg.required && m_Files.empty())
			{
				printf("Missing required argument: %s\n", arg.name.c_str());
				m_bValid = false;
			}
		}

		return m_bValid;
	}

	//---------------------------------------------------------------------
	const std::vector<std::string>& CommandParser::GetFiles() const
	{
		return m_Files;
	}

	//---------------------------------------------------------------------
	bool CommandParser::Has(const std::string& a_Name) const
	{
		auto it = m_Provided.find(a_Name);
		return it != m_Provided.end() && it->second;
	}

	//---------------------------------------------------------------------
	std::string CommandParser::Get(const std::string& a_Name) const
	{
		auto it = m_Options.find(a_Name);
		if (it != m_Options.end())
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

		for (const CommandArg& arg : m_Args)
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
			for (const CommandArg& arg : m_Args)
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
		for (const CommandArg& arg : m_Args)
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
		for (const CommandArg& arg : m_Args)
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
