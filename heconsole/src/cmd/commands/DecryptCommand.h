#pragma once

#include "cmd/Command.h"

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	// DecryptCommand
	//---------------------------------------------------------------------
	/// <summary>
	/// Decrypts a file and then exports it.
	/// </summary>
	class DecryptCommand : public Command
	{
	public:
		const char* GetName() const override;
		const char* GetDescription() const override;
		std::vector<CommandArg> GetArgs() const override;
		int Execute(int a_iArgc, char* a_sArgv[]) override;
	};
}
