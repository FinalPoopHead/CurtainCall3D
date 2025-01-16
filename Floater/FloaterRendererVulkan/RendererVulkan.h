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
		bool CreateInstance();
		bool SetupDebugMessenger();
		bool CreateWin32Surface(HWND hwnd);
		bool PickPhysicalDevice();
		bool CreateLogicalDevice();
		bool CreateSwapChain();
		bool CreateImageViews();
		bool CreateRenderPass();
		bool CreateDescriptorSetLayout();
		bool CreateGraphicsPipeline();
		bool CreateFramebuffers();
		bool CreateCommandPool();
		bool CreateTextureImage();
		bool CreateTextureImageView();
		bool CreateTextureSampler();
		bool CreateVertexBuffer();
		bool CreateIndexBuffer();
		bool CreateUniformBuffers();
		bool CreateDescriptorPool();
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

		bool CheckValidationLayerSupport();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		std::optional<uint32_t> FindTransferQueueFamilies(VkPhysicalDevice device);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		bool CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkSharingMode sharingMode = VK_SHARING_MODE_EXCLUSIVE);
		bool CreateImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkImageView CreateImageView(VkImage image, VkFormat format);

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

		VkImage _textureImage;
		VkDeviceMemory _textureImageMemory;

		VkImageView _textureImageView;
		VkSampler _textureSampler;

		bool _framebufferResized;
		bool _isMinimized;
		uint32_t _currentFrame;
	};
}


