#pragma once

#include <iostream>
#include <string>

namespace Learning2DEngine
{
	namespace System
	{
#define LOG_WARNING_COLOR 33
#define LOG_ERROR_COLOR 31
		class Log
		{
		public:
			inline static void Info(const std::string& message)
			{
				std::cout << "INFO::" << message << std::endl;
			}

			inline static void Warning(const std::string& message)
			{
				std::cout << "\033[" << LOG_WARNING_COLOR << "m" << "WARNING::" << message << "\033[0m" << std::endl;
			}

			inline static void Error(const std::string& message)
			{
				std::cout << "\033[" << LOG_ERROR_COLOR << "m" << "ERROR::" << message << "\033[0m" << std::endl;
			}
		};
	}
}