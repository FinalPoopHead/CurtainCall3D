#include "./include/RawNode.h"
#include "./include/RawSkeleton.h"
#include "./include/RawAnimation.h"
#include "./include/Camera.h"
#include "./include/Light.h"

flt::RawNode::RawNode(const std::wstring& name) :
	name(name),
	meshes(),
	skeleton(nullptr),
	animationClip(nullptr),
	camera(nullptr),
	light(nullptr),
	parent(nullptr),
	children()
{

}

flt::RawNode::~RawNode()
{
	delete skeleton;
	delete animationClip;
	delete camera;
	delete light;

	for (auto& child : children)
	{
		delete child;
	}
}
