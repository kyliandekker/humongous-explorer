#pragma once

#include "cmd/Command.h"

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	// SummaryCommand
	//---------------------------------------------------------------------
	/// <summary>
	/// Displays chunk statistics for one or more archive files.
	/// </summary>
	class SummaryCommand : public Command
	{
	public:
		const char* GetName() const override;
		const char* GetDescription() const override;
		std::vector<CommandArg> GetArgs() const override;
		int Execute(int a_Argc, char* a_Argv[]) override;
	};
}
