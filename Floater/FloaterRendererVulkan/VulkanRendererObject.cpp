#include "VulkanRendererObject.h"
#include "VulkanNode.h"
#include "../FloaterRendererCommon/include/Camera.h"

flt::VulkanRendererObject::VulkanRendererObject(const RendererObject& rendererObject, VkDevice device)
	: _device(device)
	, _transform(rendererObject.transform)
	, _isDraw(rendererObject.isDraw)
	, _node(nullptr)
	, _materials(rendererObject.materials)
	, _camera(rendererObject.camera)
{
	VulkanNode* vulkanNode = new VulkanNode(_device);
	if (!ConvertRawNodeToVulkanNodeRecursive(rendererObject.node, vulkanNode))
	{
		delete vulkanNode;
		vulkanNode = nullptr;
	}

	_node = vulkanNode;
}

bool flt::VulkanRendererObject::ConvertRawNodeToVulkanNodeRecursive(const RawNode* rawNode, VulkanNode* vulkanNode)
{
	if (!vulkanNode || !rawNode)
	{
		return false;
	}

	/// 일단 맨 처음 meshes만 사용한다고 가정
	if (!rawNode->meshes.empty())
	{
		vulkanNode->SetMesh(rawNode->meshes[0]);
	}

	for (const auto& childRawNode : rawNode->children)
	{
		VulkanNode* childVulkanNode = new VulkanNode(_device);
		if (!ConvertRawNodeToVulkanNodeRecursive(childRawNode, childVulkanNode))
		{
			delete childVulkanNode;
			return false;
		}
		vulkanNode->children.push_back(childVulkanNode);
	}

	return true;
}
