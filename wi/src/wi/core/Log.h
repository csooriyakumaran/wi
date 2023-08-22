#pragma once


#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include <glm/glm.hpp>
#include <map>

#define HAS_CONSOLE !WI_DIST

namespace wi {

	class Log
	{
	public:

		enum class Type : uint8_t
		{
			Core = 0, Client = 1
		};

		enum class Level : uint8_t
		{
			Trace = 0, Debug, Info, Warn, Error, Fatal, Off
		};

		struct TagDetails
		{
			bool Enabled = true;
			Level LevelFilter = Level::Trace;
		};

	public:

		static void Init();
		static void Shutdown();

		template<typename T, typename ... Args>
		static void AddSink(Args&& ... args);

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_EngineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

		static bool HasTag(const std::string& tag) { return s_EnabledTags.find(tag) != s_EnabledTags.end(); }
		static std::map<std::string, TagDetails>& EnabledTags() { return s_EnabledTags; }

		template<typename... Args>
		static void PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args);

		template<typename... Args>
		static void PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args);


	public:
		// enum utils
		static const char* LevelToString(Level level)
		{
			switch (level)
			{
				case Level::Trace: return "Trace";
				case Level::Debug: return "Debug";
				case Level::Info:  return "Info";
				case Level::Warn:  return "Warn";
				case Level::Error: return "Error";
				case Level::Fatal: return "Fatal";
				case Level::Off: return "Off";
			}
			return "";
		}
		static Level LevelFromString(std::string_view string)
		{
			if (string == "Trace") return Level::Trace;
			if (string == "Debug") return Level::Debug;
			if (string == "Info")  return Level::Info;
			if (string == "Warn")  return Level::Warn;
			if (string == "Error") return Level::Error;
			if (string == "Fatal") return Level::Fatal;
			if (string == "Off") return Level::Off;

			return Level::Trace;
		}

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

		inline static std::map<std::string, TagDetails> s_EnabledTags;
	};

}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tagged logs (prefer these!)                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core logging
#define LOG_CORE_TRACE_TAG(tag, ...) ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Trace, tag, __VA_ARGS__)
#define LOG_CORE_INFO_TAG(tag, ...)  ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Info, tag, __VA_ARGS__)
#define LOG_CORE_WARN_TAG(tag, ...)  ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Warn, tag, __VA_ARGS__)
#define LOG_CORE_ERROR_TAG(tag, ...) ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Error, tag, __VA_ARGS__)
#define LOG_CORE_FATAL_TAG(tag, ...) ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Fatal, tag, __VA_ARGS__)

// Client logging
#define LOG_TRACE_TAG(tag, ...) ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Trace, tag, __VA_ARGS__)
#define LOG_INFO_TAG(tag, ...)  ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Info, tag, __VA_ARGS__)
#define LOG_WARN_TAG(tag, ...)  ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Warn, tag, __VA_ARGS__)
#define LOG_ERROR_TAG(tag, ...) ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Error, tag, __VA_ARGS__)
#define LOG_FATAL_TAG(tag, ...) ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Fatal, tag, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Core Logging
#define LOG_CORE_TRACE(...)  ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Trace, "", __VA_ARGS__)
#define LOG_CORE_INFO(...)   ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Info, "", __VA_ARGS__)
#define LOG_CORE_WARN(...)   ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Warn, "", __VA_ARGS__)
#define LOG_CORE_ERROR(...)  ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Error, "", __VA_ARGS__)
#define LOG_CORE_FATAL(...)  ::wi::Log::PrintMessage(::wi::Log::Type::Core, ::wi::Log::Level::Fatal, "", __VA_ARGS__)

// Client Logging
#define LOG_TRACE(...)   ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Trace, "", __VA_ARGS__)
#define LOG_INFO(...)    ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Info, "", __VA_ARGS__)
#define LOG_WARN(...)    ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Warn, "", __VA_ARGS__)
#define LOG_ERROR(...)   ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Error, "", __VA_ARGS__)
#define LOG_FATAL(...)   ::wi::Log::PrintMessage(::wi::Log::Type::Client, ::wi::Log::Level::Fatal, "", __VA_ARGS__)


namespace wi 
{
	template<typename... Args>
	void Log::PrintMessage(Log::Type type, Log::Level level, std::string_view tag, Args&&... args)
	{
		TagDetails& detail = s_EnabledTags[std::string(tag)];
		if (detail.Enabled && detail.LevelFilter <= level)
		{
			std::shared_ptr<spdlog::logger>& logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
			std::string logString = tag.empty() ? "{0}{1}" : "<{0}> {1}";
			switch (level)
			{
			case Level::Trace:
				logger->trace(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Info:
				logger->info(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Warn:
				logger->warn(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Error:
				logger->error(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			case Level::Fatal:
				logger->critical(logString, tag, fmt::format(std::forward<Args>(args)...));
				break;
			}
		}
	}

	template<typename... Args>
	void Log::PrintAssertMessage(Log::Type type, std::string_view prefix, Args&&... args)
	{
		std::shared_ptr<spdlog::logger>& logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}: {1}", prefix, fmt::format(std::forward<Args>(args)...));
	}

	template<>
	inline void Log::PrintAssertMessage(Log::Type type, std::string_view prefix)
	{
		std::shared_ptr<spdlog::logger>& logger = (type == Type::Core) ? GetCoreLogger() : GetClientLogger();
		logger->error("{0}", prefix);
	}

	template<typename T, typename ... Args>
	void Log::AddSink(Args&& ... args)
	{
		//std::static_assert()  // is base of std::make_shared<EmbeddedConsoleSink>(2)

		spdlog::sink_ptr newsink = std::make_shared<T>(std::forward<Args>(args) ...);
		newsink->set_pattern("%^[%d-%m-%Y %H:%M:%S:%e] %n: %v%$");

		s_EngineLogger->sinks().push_back(newsink);
		s_ClientLogger->sinks().push_back(newsink);

	}
}
