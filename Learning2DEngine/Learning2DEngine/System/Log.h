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

#if _DEBUG || LEARNING2DENGINE_DEBUG
#define LOG_INFO(message) Learning2DEngine::System::Log::Info(message,__FILE__, __LINE__)
#define LOG_WARNING(message) Learning2DEngine::System::Log::Warning(message,__FILE__, __LINE__)
#define LOG_ERROR(message) Learning2DEngine::System::Log::Error(message,__FILE__, __LINE__)
#else
#define LOG_INFO(message)
#define LOG_WARNING(message)
#define LOG_ERROR(message)
#endif


		/// <summary>
		/// Use this Log class only, if you want to the logs everytime.
		/// The LOG_INFO, LOG_WARNING, LOG_ERROR macros are defined.
		/// These macros run only, if _DEBUG or LEARNING2DENGINE_DEBUG macros are defined as 1.
		/// </summary>
		class Log
		{
		private:
			Log() {};

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