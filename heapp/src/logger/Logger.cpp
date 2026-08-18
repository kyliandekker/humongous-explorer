#include "Logger.h"

// external
#include <filesystem>
#include <cstdarg>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <format>
#include <iostream>

#include "win32/WINPCH.h"

#include "file/FILEPCH.h"

namespace humongousexplorer
{
	logger::Logger& humongousexplorer::GetLogger()
	{
		static humongousexplorer::logger::Logger logger;

		return logger;
	}
}
namespace humongousexplorer::logger
{

	//---------------------------------------------------------------------
	// LoggerMessage
	//---------------------------------------------------------------------
	LoggerMessage::LoggerMessage(const std::string& a_sRawMessage, const std::string& a_Location, uint32_t a_iLine, LogSeverity a_Severity, const std::chrono::system_clock::time_point& a_Time) :
		m_sRawMessage(a_sRawMessage),
		m_sLocation(a_Location),
		m_iLine(a_iLine),
		m_Severity(a_Severity),
		m_Time(a_Time)
	{}

	//---------------------------------------------------------------------
	static bool HasConsole()
	{
		return GetConsoleWindow() != nullptr;
	}

	//---------------------------------------------------------------------
	const std::string& LoggerMessage::GetRawMessage() const
	{
		return m_sRawMessage;
	}

	//---------------------------------------------------------------------
	const std::string& LoggerMessage::GetLocation() const
	{
		return m_sLocation;
	}

	//---------------------------------------------------------------------
	uint32_t LoggerMessage::GetLine() const
	{
		return m_iLine;
	}

	//---------------------------------------------------------------------
	LogSeverity LoggerMessage::GetSeverity() const
	{
		return m_Severity;
	}

	//---------------------------------------------------------------------
	const std::chrono::system_clock::time_point& LoggerMessage::GetTime() const
	{
		return m_Time;
	}

	FILE* s_pConsole = nullptr;
	FILE* s_pLogFile = nullptr;

	//---------------------------------------------------------------------
	// Logger
	//---------------------------------------------------------------------
	bool Logger::InitThreadWorker()
	{
		// Terminal/Console initialization for debug builds.
#ifdef _DEBUG
		if (!HasConsole())
		{
			AllocConsole();
			freopen_s(&s_pConsole, "CONOUT$", "w", stdout);
		}

		// Enable virtual terminal processing (for ANSI escape codes, if needed)
		const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		DWORD dwMode = 0;
		GetConsoleMode(hOut, &dwMode);
		dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		SetConsoleMode(hOut, dwMode);

		// Set console screen buffer size to avoid wrapping issues
		const COORD coord = { 80, 300 };  // Width: 80, Height: 500
		SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coord);

		// Optionally adjust console window size
		const HWND consoleWindow = GetConsoleWindow();
		MoveWindow(consoleWindow, 100, 100, 800, 600, TRUE);
#endif // _DEBUG

		time_t time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		struct tm buf;

		localtime_s(&buf, &time_t);

		std::string logFilename(50, '\0');
		std::strftime(&logFilename[0], logFilename.size(), "./log-%Y-%m-%d %H-%M-%S.log", &buf);  // Changed colon to dash

		LOG(LOGSEVERITY_SUCCESS, "Initialized logger.");

		WakeUp();

		return true;
	}

	//---------------------------------------------------------------------
	void Logger::Finalize()
	{
#ifdef _DEBUG
		if (s_pConsole)
		{
			fclose(s_pConsole);
			s_pConsole = nullptr;
		}
#endif // _DEBUG
	}

	// Colors for console.
	constexpr auto COLOR_YELLOW = "\033[0;33m";
	constexpr auto COLOR_WHITE = "\033[0;37m";
	constexpr auto COLOR_GREEN = "\033[0;32m";
	constexpr auto COLOR_RED = "\033[31m";
	constexpr auto COLOR_CYAN = "\033[0;36m";
	constexpr auto COLOR_PURPLE = "\033[0;35m";
	constexpr auto COLOR_BLUE = "\033[0;34m";
	constexpr auto COLOR_PINK = "\033[1;35m";

	// Colors for the different severity levels.
	std::string LOGGER_SEVERITY_COLOR[8] =
	{
		COLOR_RED, // ASSERT
		COLOR_RED, // ERROR
		COLOR_YELLOW, // WARNING
		COLOR_CYAN, // INFO
		COLOR_CYAN, // TEST
		COLOR_GREEN, // SUCCESS
		COLOR_BLUE, // INFO SUCCESS
		COLOR_PINK, // AWESOME
	};

	//---------------------------------------------------------------------
	Logger::~Logger()
	{
	}

	//---------------------------------------------------------------------
	void Logger::Loop()
	{
		// This is a loop because this makes it so that it will display all messages before destruction.
		while (!m_Messages.empty())
		{
			std::scoped_lock lock(m_MessagesMutex);
			const LoggerMessage lm = m_Messages.front();
			m_Messages.pop();

			fs::path filePath = lm.GetLocation();
			std::string fileName = filePath.parent_path().filename().generic_string() + "/" + filePath.filename().generic_string();
			// Format the message.
			std::string message =
				"[" + LOGGER_SEVERITY_COLOR[lm.GetSeverity()] +
				LogSeverityToString(lm.GetSeverity()) +
				COLOR_WHITE + "] " + lm.GetRawMessage() + "\n\t\"" +
				fileName + "\" on line " + std::to_string(lm.GetLine()) + "\n\n";

			// Print the message to the console.
			printf(message.c_str());
			fflush(stdout);

			// Format the message.
			message =
				"[" +
				LogSeverityToString(lm.GetSeverity()) + "] " + lm.GetRawMessage() + "\"" +
				fileName + "\" on line " + std::to_string(lm.GetLine()) + "\n";

			if (lm.GetSeverity() <= m_AssertLevel)
			{
				MessageBoxA(
					nullptr,
					lm.GetRawMessage().c_str(),
					"Assertion Failed",
					MB_OK | MB_ICONERROR
				);

				__debugbreak();
			}
		}
	}

	//---------------------------------------------------------------------
	bool Logger::Destroy()
	{
		if (!ThreadedSystem::Destroy())
		{
			LOG_RETURN(false, LOGSEVERITY_ERROR, "Failed destroying logger.");
		}

		LOG_RETURN(true, LOGSEVERITY_INFO, "Destroyed logger.");
	}

	//---------------------------------------------------------------------
	void Logger::Log( LogSeverity a_Severity, const std::string& a_sMessage, const std::string& a_sFile, int a_iLine)
	{
		PrintMessage(a_Severity, a_sMessage, a_sFile, a_iLine);
	}

	//---------------------------------------------------------------------
	void Logger::LogF(LogSeverity a_Severity, const std::string& a_sMessage, const std::string& a_sFile, int a_iLine, ...)
	{
		va_list va_format_list;
		va_start(va_format_list, a_iLine);

		const size_t buffersize = vsnprintf(NULL, 0, a_sMessage.c_str(), va_format_list) + 1;
		char* formatted_message = (char*) malloc(buffersize);
		vsnprintf(formatted_message, buffersize, a_sMessage.c_str(), va_format_list);

		PrintMessage(a_Severity, formatted_message, a_sFile, a_iLine);

		free(formatted_message);
	}

	//---------------------------------------------------------------------
	void Logger::PrintMessage(LogSeverity a_Severity, const std::string& a_sMessage, const std::string& a_sFile, int a_iLine)
	{
		const std::string fileName = a_sFile;

		std::scoped_lock lock(m_MessagesMutex);
		m_Messages.push(LoggerMessage(a_sMessage, fileName, a_iLine, a_Severity, std::chrono::system_clock::now()));

		WakeUp();
	}

	//---------------------------------------------------------------------
	bool Logger::Sleep() const
	{
		std::scoped_lock lock(m_MessagesMutex);
		return !m_Messages.empty();
	}
}
