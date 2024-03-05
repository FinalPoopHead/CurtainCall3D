#pragma once
//#include <windows.h>


/// 다양한 곳에서 사용할 매크로를 정의해 놓은 헤더파일
/// static_assert와 assert를 정의해 놓았음.
/// 현재 윈도우 환경에서만 작동함.

#ifdef _DEBUG
//#include <string_view>

#define STATIC_ASSERT(condition, message) \
do \
{ \
	static_assert(condition, message); \
} \
while (false)
#else
#define STATIC_ASSERT(condition, message)
#endif // _DEBUG



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

#define BRANCHLESS_MIN(a, b) (a * (a < b) + b * (a <= b))
#define BRANCHLESS_MAX(a, b) (a * (a > b) + b * (a >= b))
