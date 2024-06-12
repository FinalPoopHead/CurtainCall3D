#pragma once
#include "RawNode.h"
#include "RawMaterial.h"
#include "RawMesh.h"



namespace flt
{
	struct RawScene
	{
		std::vector<RawNode*> nodes;
	};
}
