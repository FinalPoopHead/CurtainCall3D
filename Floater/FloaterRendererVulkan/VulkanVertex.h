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

			// binding 은 다양한 버퍼를 독립적으로 사용하고 싶을때 이용 가능합니다.
			// 예를들어 per vertex 데이터와 per instance 데이터를 분리하고 싶을때 사용할 수 있습니다.
			// 이와 같은경우 inputRate를 VK_VERTEX_INPUT_RATE_INSTANCE로 설정하면 됩니다.
			// 주의할 점은 쉐이더에서 사용하기 위한 location은 binding과 무관하게 독립적이어야합니다
			// binding이 다르더라도 location이 같으면 안됩니다.
			// https://docs.vulkan.org/guide/latest/vertex_input_data_processing.html

			// binding -> cpu쪽 데이터가 어떤 버퍼에 담겨있는지를 의미
			// location -> gpu쪽에서 데이터를 어떻게 찾아야 하는지를 의미

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

	struct VulkanVertex2
	{
		Vector3f pos;
		Vector2f uv;
		Vector3f normal;
		Vector3f binormal;
		Vector3f tangent;

		static VkVertexInputBindingDescription GetBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(VulkanVertex2);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 5> GetAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 5> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(VulkanVertex2, pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(VulkanVertex2, uv);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(VulkanVertex2, normal);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 3;
			attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(VulkanVertex2, binormal);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 4;
			attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(VulkanVertex2, tangent);

			return attributeDescriptions;
		}
	};
}
