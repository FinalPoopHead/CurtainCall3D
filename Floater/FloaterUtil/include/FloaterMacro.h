#pragma once
//#include <windows.h>


/// 다양한 곳에서 사용할 매크로를 정의해 놓은 헤더파일
/// static_assert와 assert를 정의해 놓았음.
/// 현재 윈도우 환경에서만 작동함.

/// STATIC_ASSERT
#ifdef _DEBUG
#define STATIC_ASSERT(condition, message) \
do \
{ \
	static_assert(condition, message); \
} \
while (false)
#else
#define STATIC_ASSERT(condition, message)
#endif // _DEBUG

/// DEBUG_CODE
#ifdef _DEBUG
#define DEBUG_CODE(code) \
do \
{ \
	code; \
} \
while (false)
#else
#define DEBUG_CODE(code)
#endif // _DEBUG

/// ASSERT
#ifdef ASSERT
#undef ASSERT
#endif // ASSERT

#ifdef _DEBUG

//#include <intrin.h>

#define ASSERT(condition, message) \
do \
{ \
	if (!(condition)) \
	{ \
		__debugbreak(); \
	} \
} \
while (false)

#else

#define ASSERT(condition, message)

#endif

/// BRANCHLESS_MIN, BRANCHLESS_MAX
#define BRANCHLESS_MIN(a, b) (a * (a < b) + b * (a <= b))
#define BRANCHLESS_MAX(a, b) (a * (a > b) + b * (a >= b))
