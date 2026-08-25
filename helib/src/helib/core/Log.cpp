#include "Log.h"

#include <cstdio>

namespace humongousexplorer::core
{
	struct LogMessage
	{
		LogLevel level;
		std::string message;
	};

	static LogCallback s_fnCallback = nullptr;
	static std::queue<LogMessage> s_Messages;
	static std::mutex s_Mutex;
	static std::condition_variable s_CondVar;
	static std::thread s_Thread;
	static bool s_bRunning = false;

	//---------------------------------------------------------------------
	static void LogThread()
	{
		while (true)
		{
			std::unique_lock lock(s_Mutex);
			s_CondVar.wait(lock, []
			{
				return !s_Messages.empty() || !s_bRunning;
			});

			while (!s_Messages.empty())
			{
				LogMessage msg = std::move(s_Messages.front());
				s_Messages.pop();

				lock.unlock();

				if (s_fnCallback)
				{
					s_fnCallback(msg.level, msg.message);
				}
				else
				{
					const char* prefix = "";
					switch (msg.level)
					{
						case LogLevel::Success: prefix = "[OK]     "; break;
						case LogLevel::Error:   prefix = "[FAIL]   "; break;
						case LogLevel::Warning: prefix = "[WARN]   "; break;
						case LogLevel::Info:    prefix = "[INFO]   "; break;
					}
					printf("%s%s\n", prefix, msg.message.c_str());
				}

				lock.lock();
			}

			if (s_bRunning)
			{
				continue;
			}

			break;
		}
	}

	//---------------------------------------------------------------------
	void InitializeLog()
	{
		s_bRunning = true;
		s_Thread = std::thread(LogThread);
	}

	//---------------------------------------------------------------------
	void DestroyLog()
	{
		{
			std::scoped_lock lock(s_Mutex);
			s_bRunning = false;
		}
		s_CondVar.notify_all();
		if (s_Thread.joinable())
		{
			s_Thread.join();
		}
		fflush(stdout);
	}

	//---------------------------------------------------------------------
	void SetLogCallback(LogCallback a_fnCallback)
	{
		s_fnCallback = std::move(a_fnCallback);
	}

	//---------------------------------------------------------------------
	void Log(LogLevel a_Level, const std::string& a_sMessage)
	{
		{
			std::scoped_lock lock(s_Mutex);
			s_Messages.push({ a_Level, a_sMessage });
		}
		s_CondVar.notify_all();
	}
}
