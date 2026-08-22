#pragma once

#include <string>

#include "core/Event.h"
#include "core/Log.h"

namespace humongousexplorer::logger
{
	extern core::SimpleEvent<core::LogLevel, const std::string&>& GetLogEvent();
}