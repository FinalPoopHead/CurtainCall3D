#pragma once
#include <string>

#ifdef _FLOATER
const std::string TEXTURE_PATH = "../../RocketEngine/Resources/Textures/";
const std::string MODEL_PATH = "../../RocketEngine/Resources/Models/";
const std::wstring FONT_PATH = L"../../RocketEngine/Resources/Font/";
const std::wstring HLSL_PATH = L"../../RocketEngine/Resources/Shaders/";
#else
const std::string TEXTURE_PATH = "Resources/Textures/";
const std::string MODEL_PATH = "Resources/Models/";
const std::wstring FONT_PATH = L"Resources/Font/";
const std::wstring HLSL_PATH = L"Resources/Shaders/";
#endif
