#pragma once

#ifdef FLOATERMATH_EXPORTS
#define FLOATER_MATH_API __declspec(dllexport)
#else
#define FLOATER_MATH_API __declspec(dllimport)
#endif
