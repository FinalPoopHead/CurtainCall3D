#pragma once
#include "vulkan/vulkan.h"

namespace flt
{
	enum FltVulkanAllocationCreateFlagBits
	{
		FLT_VULKAN_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT = 0x00000001,
		FLT_VULKAN_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT = 0x00000002,
		FLT_VULKAN_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT = 0x00000004
	};
	enum FltVulkanMemorUsage
	{
		FLT_VULKAN_MEMORY_USAGE_AUTO = 0x00000000,
		FLT_VULKAN_MEMORY_USAGE_PREFER_GPU = 0x00000001,
		FLT_VULKAN_MEMORY_USAGE_PREFER_CPU = 0x00000002
	};

	struct FltVulkanAllocationCreateInfo
	{
		FltVulkanAllocationCreateFlagBits flags;
		FltVulkanMemorUsage usage;
	};

	/// <summary>
	/// https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator를 보고 구현.
	/// 
	/// </summary>
	class VulkanMemoryAllocator
	{
	public:

	private:
		static bool FindMemoryPreferences(
			bool isIntegratedGPU
			, const FltVulkanAllocationCreateInfo& allocCreateInfo
			//, VmaBufferImageUsage bufImgUsage
			, VkMemoryPropertyFlags* outRequiredFlags
			, VkMemoryPropertyFlags* outPreferredFlags
			, VkMemoryPropertyFlags* outNotPreferredFlags);


	private:
	};
}
