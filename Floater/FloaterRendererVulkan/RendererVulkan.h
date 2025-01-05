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
		RendererVulkan() {};
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
		bool CreateGraphicsPipeline();

		bool CheckValidationLayerSupport();
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
		bool IsDeviceSuitable(VkPhysicalDevice device);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

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
	};
}


