#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "file/FILEPCH.h"
#include "Command.h"

namespace humongousexplorer::cmd
{
	//---------------------------------------------------------------------
	// CommandParser
	//---------------------------------------------------------------------
	/// <summary>
	/// Parses command-line arguments against a set of registered CommandArg definitions.
	/// </summary>
	class CommandParser
	{
	public:
		/// <summary>
		/// Constructs a parser with the given argument definitions.
		/// </summary>
		/// <param name="a_Args">The argument definitions to parse against.</param>
		CommandParser(const std::vector<CommandArg>& a_aArgs);

		/// <summary>
		/// Parses the raw argv array against the registered arguments.
		/// </summary>
		/// <param name="a_Argc">Number of arguments.</param>
		/// <param name="a_Argv">Argument strings.</param>
		/// <returns>True if parsing succeeded and all required args are present.</returns>
		bool Parse(int a_iArgc, char* a_sArgv[]);

		/// <summary>
		/// Retrieves the list of positional file arguments.
		/// </summary>
		/// <returns>A const reference to the file list.</returns>
		const std::vector<fs::path>& GetFiles() const;

		/// <summary>
		/// Checks if an option was provided.
		/// </summary>
		/// <param name="a_Name">The option name (without --).</param>
		/// <returns>True if the option was provided.</returns>
		bool Has(const std::string& a_sName) const;

		/// <summary>
		/// Retrieves an option value.
		/// </summary>
		/// <param name="a_Name">The option name (without --).</param>
		/// <returns>The option value, or the default if not provided.</returns>
		std::string Get(const std::string& a_sName) const;

		/// <summary>
		/// Checks if the parse was valid (all required args present, no unknown options).
		/// </summary>
		/// <returns>True if valid.</returns>
		bool IsValid() const;

		/// <summary>
		/// Prints usage information for the command.
		/// </summary>
		/// <param name="a_sCommandName">The command name to display.</param>
		/// <param name="a_sDescription">The command description to display.</param>
		void PrintUsage(const char* a_sCommandName, const char* a_sDescription = "") const;

	private:
		std::vector<CommandArg> m_aArgs;
		std::vector<fs::path> m_aFiles;
		std::unordered_map<std::string, std::string> m_mOptions;
		std::unordered_map<std::string, bool> m_mProvided;
		bool m_bValid = false;
	};
}
