#pragma once

#include "cmd/Command.h"

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	// XmlCommand
	//---------------------------------------------------------------------
	/// <summary>
	/// Exports one or more archive files as XML, showing the chunk hierarchy.
	/// </summary>
	class XmlCommand : public Command
	{
	public:
		const char* GetName() const override;
		const char* GetDescription() const override;
		std::vector<CommandArg> GetArgs() const override;
		int Execute(int a_Argc, char* a_Argv[]) override;
	};
}
