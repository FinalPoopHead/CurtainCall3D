#include "./include/RawNode.h"

flt::RawNode::RawNode(const std::wstring& name) :
	name(name),
	meshes(),
	skeleton(nullptr),
	camera(nullptr),
	boneIndex(-1),
	parent(nullptr),
	children()
{

}

flt::RawNode::~RawNode()
{
	//delete animation;
	//delete camera;
	//delete skeleton;
	//delete animation;

	for (auto& child : children)
	{
		delete child;
	}
}
