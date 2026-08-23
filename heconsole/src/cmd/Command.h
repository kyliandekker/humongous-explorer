#pragma once

#include <string>
#include <vector>

namespace humongousexplorer::cmd
{
	enum class ArgType
	{
		File,
		Option,
		Flag,
		Int
	};

	//---------------------------------------------------------------------
	// CommandArg
	//---------------------------------------------------------------------
	/// <summary>
	/// Describes a single argument that a command accepts.
	/// </summary>
	struct CommandArg
	{
		std::string name;
		std::string description;
		ArgType type = ArgType::File;
		bool required = false;
		std::string defaultValue;
	};

	//---------------------------------------------------------------------
	// Command
	//---------------------------------------------------------------------
	/// <summary>
	/// Base interface for a CLI command. Each command registers its args and handles execution.
	/// </summary>
	class Command
	{
	public:
		virtual ~Command() = default;

		/// <summary>
		/// Retrieves the command name used to invoke it (e.g. "xml", "replace", "extract").
		/// </summary>
		virtual const char* GetName() const = 0;

		/// <summary>
		/// Retrieves a short description of what the command does.
		/// </summary>
		virtual const char* GetDescription() const = 0;

		/// <summary>
		/// Retrieves the list of arguments this command accepts.
		/// </summary>
		virtual std::vector<CommandArg> GetArgs() const = 0;

		/// <summary>
		/// Executes the command with the given arguments.
		/// </summary>
		/// <param name="a_Argc">Number of arguments (excluding command name).</param>
		/// <param name="a_Argv">Argument strings (excluding command name).</param>
		/// <returns>Exit code (0 = success).</returns>
		virtual int Execute(int a_Argc, char* a_Argv[]) = 0;
	};
}
