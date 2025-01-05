#pragma once

namespace flt
{
	enum class RendererType
	{
		SOFTWARE,
		DX11,
		DX12,
		ROCKET_DX11,
		VULKAN
	};

	enum class WindowMode
	{
		WINDOWED,
		FULLSCREEN,
		BORDERLESS
	};
}

