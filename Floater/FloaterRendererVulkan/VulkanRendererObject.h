#pragma once
#include "../FloaterRendererCommon/include/RendererObject.h"
#include "vulkan/vulkan_core.h"

namespace flt
{
	struct VulkanNode;
	class Camera;

	class VulkanRendererObject
	{
	public:
		VulkanRendererObject(const RendererObject& rendererObject, VkDevice device);
	
	private:
		bool ConvertRawNodeToVulkanNodeRecursive(const RawNode* rawNode, VulkanNode* vulkanNode);

	private:
		VkDevice _device;
		Transform* _transform;
		const bool& _isDraw;
		VulkanNode* _node;
		std::vector<RawMaterial> _materials;
		Camera* _camera;
	};
}
