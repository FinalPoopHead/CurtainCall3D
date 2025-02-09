#pragma once
//#include "glm/glm.hpp"
#include "vulkan/vulkan.h"
#include <array>
#include "../FloaterMath/include/Vector2f.h"
#include "../FloaterMath/include/Vector3f.h"


namespace flt
{
	/// <summary>
	/// 정점 데이터를 저장하는 구조체.
	/// </summary>
	struct VulkanVertex
	{
		Vector3f pos;
		Vector3f color;
		Vector2f texCoord;

		/// <summary>
		/// 벌칸에서 정점 데이터를 어떻게 읽어올지에 대한 정보를 반환합니다.
		/// 데이터를 어떤 빈도로(버택스 별, 인스턴스 별) 읽어야 하는지에 대한 정보를 가지고 있습니다.
		/// </summary>
		/// <returns></returns>
		static VkVertexInputBindingDescription GetBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VulkanVertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		/// <summary>
		/// 각 정점 별 레이아웃을 반환합니다.
		/// </summary>
		/// <returns></returns>
		static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
		{
			// 셰이더 에서는 다음과 같이 사용됩니다.
			// layout(location = 0) in vec3 inPosition;
			// layout(location = 1) in vec3 inColor;
			// layout(location = 2) in vec2 inTexCoord;

			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
			// Position
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(VulkanVertex, pos);

			// Color
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(VulkanVertex, color);

			// Texture coordinates
			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(VulkanVertex, texCoord);

			return attributeDescriptions;
		}
	};
}
