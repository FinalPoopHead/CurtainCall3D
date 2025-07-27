#include "VulkanMemoryAllocator.h"

bool flt::VulkanMemoryAllocator::FindMemoryPreferences(
	bool isIntegratedGPU
	, const FltVulkanAllocationCreateInfo& allocCreateInfo
	//, VmaBufferImageUsage bufImgUsage
	, VkMemoryPropertyFlags* outRequiredFlags, VkMemoryPropertyFlags* outPreferredFlags, VkMemoryPropertyFlags* outNotPreferredFlags)
{
	/// vulkan 메모리 속성 플래그 결정.
	// VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT GPU에서 가장 효율적인 메모리 일반적으로 VRAM
	// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT CPU에서 맵핑하여 읽고 쓸 수있는 메모리.
	//		VK_MEMORY_PROPERTY_HOST_COHERENT_BIT CPU와 GPU 간 일관성을 자동으로 보장. vkFlushMappedMemoryRanges 같은 명시적인 flush 필요 없음.
	//		VK_MEMORY_PROPERTY_HOST_CACHED_BIT	CPU가 이 메모리에 접근할 때 CPU 캐시 사용이 가능해짐. 이 플래그가 없으면 보통 CPU접근은 write-combined 방식으로 동작한다.(버퍼링되어 순차처리)
	//											HOST_COHERENT_BIT와 함께 사용되지 않으면 CPU쓰기 이후 vkFlushMappedMemoryRanges 호출이 필요하며 GPU 쓰기 이후 vkInvalidateMappedMemoryRanges 호출해야한다.

	///const bool deviceAccess = bufImgUsage.ContainsDeviceAccess();
	const bool deviceAccess = false; /// 임시로 false로 설정. 실제로는 버퍼 이미지 사용에 따라 결정되어야 함.
	const bool hostAccessSequentialWrite = (allocCreateInfo.flags & FLT_VULKAN_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT) != 0;
	// cpu가 버퍼 데이터 랜덤 읽기쓰기 필요할 경우
	const bool hostAccessRandom = (allocCreateInfo.flags & FLT_VULKAN_ALLOCATION_CREATE_HOST_ACCESS_RANDOM_BIT) != 0;
	const bool hostAccessAllowTransferInstead = (allocCreateInfo.flags & FLT_VULKAN_ALLOCATION_CREATE_HOST_ACCESS_ALLOW_TRANSFER_INSTEAD_BIT) != 0;
	const bool preferDevice = allocCreateInfo.usage == FLT_VULKAN_MEMORY_USAGE_PREFER_GPU;
	const bool preferHost = allocCreateInfo.usage == FLT_VULKAN_MEMORY_USAGE_PREFER_CPU;

	// CPU에서 랜덤 접근이 필요한 경우
	// GPU 계산 결과를 CPU에서 읽어야 하거나 소량의 데이터를 CPU에서 자주 수정하는경우
	if (hostAccessRandom)
	{
		// Prefer cached. Cannot require it, because some platforms don't have it (e.g. Raspberry Pi - see #362)!
		*outPreferredFlags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

		// 외장 GPU 사용 && GPU 접근 가능 && CPU접근은 하지만 매핑방식이 아니어도 된다. (staging buffer를 통한 transfer 작업 허용한다) && CPU 메모리 할당 선호 안함
		if (!isIntegratedGPU && deviceAccess && hostAccessAllowTransferInstead && !preferHost)
		{
			// Nice if it will end up in HOST_VISIBLE, but more importantly prefer DEVICE_LOCAL.
			// Omitting HOST_VISIBLE here is intentional.
			// In case there is DEVICE_LOCAL | HOST_VISIBLE | HOST_CACHED, it will pick that one.
			// Otherwise, this will give same weight to DEVICE_LOCAL as HOST_VISIBLE | HOST_CACHED and select the former if occurs first on the list.
			// 이 경우 GPU VRAM 사용하는걸 선호하게 된다.
			*outPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		}
		else
		{
			// Always CPU memory.
			// CPU 메모리 할당 필요
			*outRequiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		}
	}
	// CPU에서 순차 쓰기를 하는 경우
	// 프레임마다 업데이트 해야하는 유니폼 버퍼/업로드 버퍼 같은 경우.
	else if (hostAccessSequentialWrite)
	{
		// CPU 캐시 사용 불가능한 write-combined 방식으로 동작 CPU 쓰기는 버퍼링되어 순차적으로 GPU에 전달된다.
		*outNotPreferredFlags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

		if (!isIntegratedGPU && deviceAccess && hostAccessAllowTransferInstead && !preferHost)
		{
			*outPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		}
		else
		{
			*outRequiredFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			// Direct GPU access, CPU sequential write (e.g. a dynamic uniform buffer updated every frame)
			if (deviceAccess)
			{
				// Could go to CPU memory or GPU BAR/unified. Up to the user to decide. If no preference, choose GPU memory.
				if (preferHost)
					*outNotPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				else
					*outPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			}
			// GPU no direct access, CPU sequential write (e.g. an upload buffer to be transferred to the GPU)
			else
			{
				// Could go to CPU memory or GPU BAR/unified. Up to the user to decide. If no preference, choose CPU memory.
				if (preferDevice)
					*outPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
				else
					*outNotPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
			}
		}
	}
	// No CPU access
	else
	{
		// if(deviceAccess)
		//
		// GPU access, no CPU access (e.g. a color attachment image) - prefer GPU memory,
		// unless there is a clear preference from the user not to do so.
		//
		// else:
		//
		// No direct GPU access, no CPU access, just transfers.
		// It may be staging copy intended for e.g. preserving image for next frame (then better GPU memory) or
		// a "swap file" copy to free some GPU memory (then better CPU memory).
		// Up to the user to decide. If no preferece, assume the former and choose GPU memory.

		if (preferHost)
			*outNotPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		else
			*outPreferredFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
	}

	return false; /// 임시로 false 리턴.
}
