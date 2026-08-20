#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "cmd/Command.h"
#include "core/Log.h"

#include "cmd/commands/XmlCommand.h"
#include "cmd/commands/SummaryCommand.h"

static void PrintUsage(const std::vector<std::unique_ptr<humongousexplorer::cmd::Command>>& a_Commands)
{
	printf("Usage: heconsole <command> [options]\n\n");
	printf("Commands:\n");
	for (const auto& cmd : a_Commands)
	{
		printf("  %-12s %s\n", cmd->GetName(), cmd->GetDescription());
	}
	printf("\nRun 'heconsole <command> --help' for command-specific help.\n");
}

int main(int argc, char* argv[])
{
	humongousexplorer::core::InitializeLog();

	std::vector<std::unique_ptr<humongousexplorer::cmd::Command>> commands;
	commands.push_back(std::make_unique<humongousexplorer::cmd::XmlCommand>());
	commands.push_back(std::make_unique<humongousexplorer::cmd::SummaryCommand>());

	if (argc < 2)
	{
		PrintUsage(commands);
		humongousexplorer::core::DestroyLog();
		return 1;
	}

	std::string commandName = argv[1];

	for (const auto& cmd : commands)
	{
		if (commandName == cmd->GetName())
		{
			int result = cmd->Execute(argc - 2, argv + 2);
			humongousexplorer::core::DestroyLog();
			return result;
		}
	}

	printf("Unknown command: %s\n\n", commandName.c_str());
	PrintUsage(commands);

	humongousexplorer::core::DestroyLog();
	return 1;
}