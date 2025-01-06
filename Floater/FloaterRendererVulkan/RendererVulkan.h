#pragma once
#include "../FloaterRendererCommon/include/IRenderer.h"
#include <windows.h>
#include "vulkan/vulkan.h"

#include <vector>
#include <optional>


#pragma comment(lib, "vulkan-1.lib")

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
		bool CreateGraphicsPipeline();
		bool CreateFramebuffers();
		bool CreateCommandPool();
		bool CreateCommandBuffer();
		bool CreateSyncObjects();

		bool RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);;

		bool CheckValidationLayerSupport();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkShaderModule CreateShaderModule(const std::vector<char>& code);

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

		VkSwapchainKHR _swapChain;
		std::vector<VkImage> _swapChainImages;
		VkFormat _swapChainImageFormat;
		VkExtent2D _swapChainExtent;
		std::vector<VkImageView> _swapChainImageViews;

		VkRenderPass _renderPass;
		VkPipelineLayout _pipelineLayout;
		VkPipeline _graphicsPipeline;

		std::vector<VkFramebuffer> _swapChainFramebuffers;

		VkCommandPool _commandPool;
		std::vector<VkCommandBuffer> _commandBuffers; // commandPool이 정리 될 때 자동으로 정리 됨

		std::vector<VkSemaphore> _imageAvailableSemaphores;
		std::vector<VkSemaphore> _renderFinishedSemaphores;
		std::vector<VkFence> _inFlightFences;

		uint32_t _currentFrame;
	};
}


