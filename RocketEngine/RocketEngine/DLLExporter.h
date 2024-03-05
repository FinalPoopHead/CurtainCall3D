#pragma once

#ifdef _EXPORT_API
#define ROCKET_API __declspec(dllexport)
#else
#define ROCKET_API __declspec(dllimport)
#endif //_EXPORT_API