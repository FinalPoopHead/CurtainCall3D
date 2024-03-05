#pragma once

namespace flt
{
	struct Rect
	{
		Rect() : Rect(0, 0) {}
		Rect(float width, float height) : width(width), height(height) {}

		float width;
		float height;
	};
}
