#pragma once

#include <iostream>
#include <string>

namespace Learning2DEngine
{
	namespace System
	{
#define LOG_WARNING_COLOR "\033[33m"
#define LOG_ERROR_COLOR "\033[31m"
#define LOG_CLEAR_COLOR "\033[0m"

		
#define LOG_INFO(message) Log::Info(message,__FILE__, __LINE__)
#define LOG_WARNING(message) Log::Warning(message,__FILE__, __LINE__)
#define LOG_ERROR(message) Log::Error(message,__FILE__, __LINE__)

		/// <summary>
		/// The LOG_INFO, LOG_WARNING, LOG_ERROR macros are defined.
		/// </summary>
		class Log
		{
		private:
			inline static std::string GetMessage(const std::string& message, const std::string& filePath, int fileLine)
			{
				return message + "\n " + filePath + " Line: " + std::to_string(fileLine);
			}
		public:

			inline static void Info(const std::string& message, const std::string& filePath, int fileLine)
			{
				std::cout << "INFO::" << GetMessage(message, filePath, fileLine) << std::endl;
			}

			inline static void Warning(const std::string& message, const std::string& filePath, int fileLine)
			{
				std::cout << LOG_WARNING_COLOR << "WARNING::" << GetMessage(message, filePath, fileLine) << LOG_CLEAR_COLOR << std::endl;
			}

			inline static void Error(const std::string& message, const std::string& filePath, int fileLine)
			{
				std::cout << LOG_ERROR_COLOR << "ERROR::" << GetMessage(message, filePath, fileLine) << LOG_CLEAR_COLOR << std::endl;
			}
		};
	}
}