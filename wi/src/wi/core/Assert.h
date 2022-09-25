#pragma once

#ifdef WI_PLATFORM_WINDOWS
	#define WI_DEBUG_BREAK __debugbreak()
#else
	#define WI_DEBUG_BREAK
#endif // WI_PLATFORM_WINDOWS

#ifdef WI_DEBUG
	#define WI_ENABLE_ASSERTS
#endif

#define WI_ENABLE_VERIFY

#ifdef WI_ENABLE_ASSERTS
	#define CORE_ASSERT_MESSAGE_INTERNAL(...)  ::wi::Log::PrintAssertMessage(::wi::Log::Type::Core, "Assertion Failed", __VA_ARGS__)
	#define ASSERT_MESSAGE_INTERNAL(...)  ::wi::Log::PrintAssertMessage(::wi::Log::Type::Client, "Assertion Failed", __VA_ARGS__)
	
	#define CORE_ASSERT(condition, ...) { if(!(condition)) { CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); WI_DEBUG_BREAK; } }
	#define ASSERT(condition, ...) { if(!(condition)) { ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); WI_DEBUG_BREAK; } }
#else
	#define CORE_ASSERT(condition, ...)
	#define ASSERT(condition, ...)
#endif

#ifdef WI_ENABLE_VERIFY
	#define CORE_VERIFY_MESSAGE_INTERNAL(...)  ::wi::Log::PrintAssertMessage(::wi::Log::Type::Core, "Verify Failed", __VA_ARGS__)
	#define VERIFY_MESSAGE_INTERNAL(...)  ::wi::Log::PrintAssertMessage(::wi::Log::Type::Client, "Verify Failed", __VA_ARGS__)
	
	#define CORE_VERIFY(condition, ...) { if(!(condition)) { CORE_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); WI_DEBUG_BREAK; } }
	#define VERIFY(condition, ...) { if(!(condition)) { VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); WI_DEBUG_BREAK; } }
#else
	#define CORE_VERIFY(condition, ...)
	#define VERIFY(condition, ...)
#endif
