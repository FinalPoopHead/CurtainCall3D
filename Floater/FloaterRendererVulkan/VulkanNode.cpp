#include "VulkanNode.h"


flt::VulkanNode::VulkanNode(VkDevice device)
	: device(device)
	, name(L"")
	, transform(nullptr)
	, vertexBuffer(VK_NULL_HANDLE)
	, vertexBufferMemory(VK_NULL_HANDLE)
	, indexBuffer(VK_NULL_HANDLE)
	, indexBufferMemory(VK_NULL_HANDLE)
{

}

flt::VulkanNode::VulkanNode(VkDevice device, const std::wstring& name, Transform* transform, VkBuffer vertexBuffer, VkDeviceMemory vertexBufferMemory, VkBuffer indexBuffer, VkDeviceMemory indexBufferMemory)
	: device(device)
	, name(name)
	, transform(transform)
	, vertexBuffer(vertexBuffer)
	, vertexBufferMemory(vertexBufferMemory)
	, indexBuffer(indexBuffer)
	, indexBufferMemory(indexBufferMemory)
{

}

flt::VulkanNode::~VulkanNode()
{
	if (vertexBuffer)
	{
		vkDestroyBuffer(device, vertexBuffer, nullptr);
	}

	if (vertexBufferMemory)
	{
		vkFreeMemory(device, vertexBufferMemory, nullptr);
	}

	if (indexBuffer)
	{
		vkDestroyBuffer(device, indexBuffer, nullptr);
	}

	if (indexBufferMemory)
	{
		vkFreeMemory(device, indexBufferMemory, nullptr);
	}

	for (auto& child : children)
	{
		delete child;
	}
}

bool flt::VulkanNode::SetMesh(const RawMesh& mesh)
{
	if (mesh.vertices.empty() || mesh.indices.empty())
	{
		return true;
	}

	std::vector<VulkanVertex2> vertices;
	vertices.reserve(mesh.vertices.size());

	for (const auto& vertex : mesh.vertices)
	{
		VulkanVertex2 vulkanVertex;
		vulkanVertex.pos = vertex.pos;
		vulkanVertex.uv = vertex.uvs[0]; // Assuming we only use the first UV set
		vulkanVertex.normal = vertex.normal;
		vulkanVertex.binormal = vertex.binormal;
		vulkanVertex.tangent = vertex.tangent;
		vertices.push_back(vulkanVertex);
	}

	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer{};
	VkDeviceMemory stagingBufferMemory{};

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);



	return true;
}
