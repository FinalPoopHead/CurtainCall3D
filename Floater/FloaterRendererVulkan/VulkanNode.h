#pragma once
#include <string>
#include <vector>
#include "VulkanVertex.h"
#include "vulkan/vulkan.h"
#include "../FloaterRendererCommon/include/RawMesh.h"


namespace flt
{
	class Transform;

	struct VulkanNode
	{
		VulkanNode(VkDevice device);
		VulkanNode(VkDevice device
			, const std::wstring& name
			, Transform* transform
			, VkBuffer vertexBuffer
			, VkDeviceMemory vertexBufferMemory
			, VkBuffer indexBuffer
			, VkDeviceMemory indexBufferMemory
		);
		~VulkanNode();

		bool SetMesh(const RawMesh& mesh);

		std::wstring name;
		Transform* transform;
		std::vector<VulkanNode*> children;

		VkDevice device;

		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
	};
}
