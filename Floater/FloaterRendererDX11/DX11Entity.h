#pragma once
#include "../FloaterRendererCommon/include/Transform.h"
#include "../FloaterRendererCommon/include/TransformOwner.h"


namespace flt
{
	struct DX11Node;

	struct DX11Entity : public TransformOwner
	{
		DX11Entity() : isDraw(true), node(nullptr) {}
		~DX11Entity() 
		{ 
			delete node; 
		}

		bool isDraw;
		DX11Node* node;
	};
}
