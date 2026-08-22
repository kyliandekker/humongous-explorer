#include "AppLogger.h"

namespace humongousexplorer::logger
{
	core::SimpleEvent<core::LogLevel, const std::string&>& GetLogEvent()
	{
		static core::SimpleEvent<core::LogLevel, const std::string&> logEvent;
		return logEvent;
	}
}