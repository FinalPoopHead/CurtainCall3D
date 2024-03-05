#include "DX11Node.h"

flt::DX11Node::DX11Node(Transform& transform, const bool& isDraw) : 
	transform(transform),
	isDraw(isDraw),
	meshes(),
	camera(nullptr),
	boneIndex(NO_BONE)
{

}
