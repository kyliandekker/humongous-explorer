#pragma once

#include <functional>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace humongousexplorer::core
{
	enum class LogLevel
	{
		Success,
		Error,
		Warning,
		Info,
	};

	using LogCallback = std::function<void(LogLevel, const std::string&)>;

	/// <summary>
	/// Initializes the threaded log system. Must be called before any Log calls.
	/// </summary>
	void InitializeLog();

	/// <summary>
	/// Destroys the threaded log system, flushing remaining messages.
	/// </summary>
	void DestroyLog();

	/// <summary>
	/// Sets the global log callback used by helib to report messages.
	/// </summary>
	/// <param name="a_fnCallback">The callback to invoke on each log message.</param>
	void SetLogCallback(LogCallback a_fnCallback);

	/// <summary>
	/// Logs a message. The message is queued and processed on a background thread.
	/// </summary>
	/// <param name="a_Level">The severity level of the message.</param>
	/// <param name="a_sMessage">The message to log.</param>
	void Log(LogLevel a_Level, const std::string& a_sMessage);
}
