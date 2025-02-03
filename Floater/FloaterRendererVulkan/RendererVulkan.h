#pragma once
#include "../FloaterRendererCommon/include/IRenderer.h"
#include <windows.h>
#include "vulkan/vulkan.h"

#include <vector>
#include <optional>


#pragma comment(lib, "vulkan-1.lib")

/// TODO : 커스텀 메모리 allocator 구현 (buddy allocator)
/// TODO : 버퍼 복사를 위한 커맨트 풀 분리(이 경우 장점 확인), transfer 전용 queue
/// TODO : 버텍스, 인덱스 버퍼같은 여러 버퍼를 단일 VkBuffer에 저장하고
/// vkCmdBindVertexBuffers, vkCmdBindIndexBuffer를 사용하여 오프셋을 이용해 여러 버퍼를 바인딩하는 방법을 알아보자.
/// 또한 앨리어싱(aliasing) 기법을 사용하여 버퍼를 관리하는 방법을 알아보자.

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

namespace flt
{
	class RendererVulkan : public IRenderer
	{
	public:
		RendererVulkan();
		~RendererVulkan() {};

		bool Initialize(HWND hwnd, HWND debugHWnd = NULL);
		bool Finalize();

		bool Render(float deltaTime);
		virtual HOBJECT RegisterObject(RendererObject& renderable);
		virtual bool DeregisterObject(HOBJECT renderable);

		virtual bool SetFullScreen(bool isFullScreen);
		virtual bool Resize(uint32 width, uint32 height);

	private:
		/// Initialization functions

		/// <summary>
		/// 벌컨 인스턴스를 생성합니다.
		/// 전역을 사용할 확장을 설정합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateInstance();

		/// <summary>
		/// 디버그 메신저를 설정합니다.
		/// 콜백 함수를 등록하고 받을 메세지의 종류를 설정합니다.
		/// </summary>
		/// <returns></returns>
		bool SetupDebugMessenger();

		/// <summary>
		/// 그려질 윈도우 surface를 생성합니다.
		/// </summary>
		/// <param name="hwnd"></param>
		/// <returns></returns>
		bool CreateWin32Surface(HWND hwnd);

		/// <summary>
		/// 사용할 그래픽 카드를 선택해 물리 디바이스를 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool PickPhysicalDevice();

		/// <summary>
		/// 사용할 큐 패밀리를 선택해 디바이스를 생성합니다.
		/// 생성한 디바이스로 큐를 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateLogicalDevice();

		/// <summary>
		/// 생성한 surface에 맞는 스왑체인을 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateSwapChain();

		/// <summary>
		/// 스왑체인 Image에 대한 ImageView를 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateImageViews();

		/// <summary>
		/// 렌더 패스를 생성합니다.
		/// 사용할 attachment, subpass간 의존등을 설정합니다.
		/// RenderPass
		///  └─ SubPass(1개 이상)
		///	  └─ PipeLine(1개 이상)
		/// </summary>
		/// <returns></returns>
		bool CreateRenderPass();

		/// <summary>
		/// DescriptorSetLayout을 생성합니다.
		/// 파이프라인이 리소스에 엑세스 하는 데 사용할 DescriptorSet의 구조를 정의합니다.
		/// DescriptorSet과 PipelineLayout을 만들 때 사용합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateDescriptorSetLayout();

		/// <summary>
		/// GraphicsPipeline을 생성합니다.
		/// 실제 렌더링 방식을 정의합니다.
		/// 파이프라인은 고정되며 셰이더 등을 변경해야 한다면 다른 파이프라인 객체를 사용해야 합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateGraphicsPipeline();

		/// <summary>
		/// RenderPass의 RenderTarget으로 사용되는 attachment를 나타내는 framebuffer를 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateFramebuffers();

		/// <summary>
		/// command buffer가 사용할 메모리를 할당해 주는 command pool을 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateCommandPool();

		/// <summary>
		/// depthbuffer에 사용할 image, image memory, imageview를 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateDepthResources();

		/// <summary>
		/// texture를 위한 image, image memory를 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateTextureImage();

		bool CreateTextureImageView();

		/// <summary>
		/// sampler를 생성합니다.
		/// 비등방성 필터링을 사용하려면 physical device에서 samplerAnisotropy를 지원하는지 확인 하고
		/// device 에서 deviceFeatures.samplerAnisotropy = VK_TRUE; 를 설정 해야 합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateTextureSampler();

		/// <summary>
		/// 버텍스 버퍼를 생성합니다.
		/// cpu에서 접근 하기 유리한 staging 버퍼에 데이터를 복사하고
		/// 
		/// </summary>
		/// <returns></returns>
		bool CreateVertexBuffer();

		bool CreateIndexBuffer();

		bool CreateUniformBuffers();

		/// <summary>
		/// DescriptorSet이 사용할 메모리를 할당해 주는 DescriptorPool을 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateDescriptorPool();

		/// <summary>
		/// 유니폼 버퍼, 이미지, 샘플러 등 GPU리소스에 대한 참조를 가진 DescriptorSet을 생성합니다.
		/// </summary>
		/// <returns></returns>
		bool CreateDescriptorSets();

		bool CreateCommandBuffer();

		bool CreateSyncObjects();

	private:
		void CleanupSwapChain();
		void RecreateSwapChain();

		bool RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		void UpdateUniformBuffer(uint32_t currentImage);

		VkCommandBuffer BeginSingleTimeCommands();
		void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

		VkCommandBuffer BeginSingleTimeTransferCommands();
		void EndSingleTimeTransferCommands(VkCommandBuffer commandBuffer);

		bool CheckValidationLayerSupport();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		std::optional<uint32_t> FindTransferQueueFamilies(VkPhysicalDevice device);
		VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat FindDepthFormat();
		bool HasStencilComponent(VkFormat format);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		bool CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE);
		bool CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

		void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	private:
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
		static std::vector<char> ReadFile(const std::string& filename);

	private:
		VkInstance _instance;
		bool _enableValidationLayers;
		VkDebugUtilsMessengerEXT _debugMessenger;
		VkSurfaceKHR _surface;

		VkPhysicalDevice _physicalDevice;
		VkDevice _device;

		VkQueue _graphicsQueue;
		VkQueue _presentQueue;
		VkQueue _transferQueue;

		VkSwapchainKHR _swapChain;
		std::vector<VkImage> _swapChainImages;
		VkFormat _swapChainImageFormat;
		VkExtent2D _swapChainExtent;
		std::vector<VkImageView> _swapChainImageViews;

		VkRenderPass _renderPass;
		VkDescriptorSetLayout _descriptorSetLayout;
		VkPipelineLayout _pipelineLayout;
		VkPipeline _graphicsPipeline;

		std::vector<VkFramebuffer> _swapChainFramebuffers;

		VkCommandPool _commandPool;
		std::vector<VkCommandBuffer> _commandBuffers; // commandPool이 정리 될 때 자동으로 정리 됨

		VkCommandPool _transferCommandPool;
		VkCommandBuffer _transferCommandBuffer;

		std::vector<VkSemaphore> _imageAvailableSemaphores;
		std::vector<VkSemaphore> _renderFinishedSemaphores;
		std::vector<VkFence> _inFlightFences;

		VkBuffer _vertexBuffer;
		VkDeviceMemory _vertexBufferMemory;

		VkBuffer _indexBuffer;
		VkDeviceMemory _indexBufferMemory;

		std::vector<VkBuffer> _uniformBuffers;
		std::vector<VkDeviceMemory> _uniformBuffersMemory;
		std::vector<void*> _uniformBuffersMapped;

		VkDescriptorPool _descriptorPool;
		std::vector<VkDescriptorSet> _descriptorSets;

		VkImage _depthImage;
		VkDeviceMemory _depthImageMemory;
		VkImageView _depthImageView;

		VkImage _textureImage;
		VkDeviceMemory _textureImageMemory;

		VkImageView _textureImageView;
		VkSampler _textureSampler;

		bool _framebufferResized;
		bool _isMinimized;
		uint32_t _currentFrame;
	};
}
