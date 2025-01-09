#include "RendererVulkan.h"

#include "vulkan/vulkan_win32.h"
#include <fstream>
#include <filesystem>
#include <set>
#include <algorithm>

#include "VulkanVertex.h"
#include "../FloaterRendererCommon/include/Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


//struct UniformBufferObject
//{
//	flt::Matrix4f model;
//	flt::Matrix4f view;
//	flt::Matrix4f proj;
//};

struct UniformBufferObjectGLM
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

const std::vector<flt::VulkanVertex> vertices =
{
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
	{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

const std::vector<uint16_t> indices =
{
	0, 1, 2,
	2, 3, 0
};

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<const char*> validationLayers =
{
	"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

VKAPI_ATTR VkBool32 VKAPI_CALL flt::RendererVulkan::DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
	{
		std::cerr << "ERROR  " << pCallbackData->pMessage << std::endl;
	}
	else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
	{
		std::cerr << "WARNING  " << pCallbackData->pMessage << std::endl;
	}
	else
	{
		std::cerr << "INFO  " << pCallbackData->pMessage << std::endl;
	}

	//std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

	return VK_FALSE;
}

std::vector<char> flt::RendererVulkan::ReadFile(const std::string& filename)
{
	std::filesystem::path path{ filename };
	auto absPath = std::filesystem::absolute(path);
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if (func != nullptr)
	{
		func(instance, debugMessenger, pAllocator);
	}
}

flt::RendererVulkan::RendererVulkan()
	: _instance(VK_NULL_HANDLE)
	, _enableValidationLayers(false)
	, _debugMessenger(VK_NULL_HANDLE)
	, _surface(VK_NULL_HANDLE)
	, _physicalDevice(VK_NULL_HANDLE)
	, _device(VK_NULL_HANDLE)
	, _graphicsQueue(VK_NULL_HANDLE)
	, _presentQueue(VK_NULL_HANDLE)
	, _transferQueue(VK_NULL_HANDLE)
	, _swapChain(VK_NULL_HANDLE)
	, _swapChainImageFormat(VK_FORMAT_UNDEFINED)
	, _swapChainExtent({ 0, 0 })
	, _renderPass(VK_NULL_HANDLE)
	, _descriptorSetLayout(VK_NULL_HANDLE)
	, _graphicsPipeline(VK_NULL_HANDLE)
	, _pipelineLayout(VK_NULL_HANDLE)
	, _swapChainFramebuffers()
	, _commandPool(VK_NULL_HANDLE)
	, _commandBuffers()
	, _transferCommandPool(VK_NULL_HANDLE)
	, _transferCommandBuffer(VK_NULL_HANDLE)
	, _imageAvailableSemaphores()
	, _renderFinishedSemaphores()
	, _inFlightFences()
	, _vertexBuffer(VK_NULL_HANDLE)
	, _vertexBufferMemory(VK_NULL_HANDLE)
	, _indexBuffer(VK_NULL_HANDLE)
	, _indexBufferMemory(VK_NULL_HANDLE)
	, _uniformBuffers()
	, _uniformBuffersMemory()
	, _uniformBuffersMapped()
	, _framebufferResized(false)
	, _isMinimized(false)
	, _currentFrame(0)
{

}

bool flt::RendererVulkan::Initialize(HWND hwnd, HWND debugHWnd)
{
	/// TODO : 디버그 여부에 따라서 validation layer를 사용할지 말지 결정해야 함
	_enableValidationLayers = true;

	bool result = true;
	result &= CreateInstance();
	result &= SetupDebugMessenger();
	result &= CreateWin32Surface(hwnd);
	result &= PickPhysicalDevice();
	result &= CreateLogicalDevice();
	result &= CreateSwapChain();
	result &= CreateImageViews();
	result &= CreateRenderPass();
	result &= CreateDescriptorSetLayout();
	result &= CreateGraphicsPipeline();
	result &= CreateFramebuffers();
	result &= CreateCommandPool();
	result &= CreateVertexBuffer();
	result &= CreateIndexBuffer();
	result &= CreateUniformBuffers();
	result &= CreateDescriptorPool();
	result &= CreateDescriptorSets();
	result &= CreateCommandBuffer();
	result &= CreateSyncObjects();

	ASSERT(result, "failed to initialize vulkan renderer!");

	return result;
}

bool flt::RendererVulkan::Finalize()
{
	vkDeviceWaitIdle(_device);

	CleanupSwapChain();
	_swapChainFramebuffers.clear();
	_swapChainImageViews.clear();
	_swapChain = VK_NULL_HANDLE;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		vkDestroyBuffer(_device, _uniformBuffers[i], nullptr);
		vkFreeMemory(_device, _uniformBuffersMemory[i], nullptr);
	}
	_uniformBuffers.clear();
	_uniformBuffersMemory.clear();

	vkDestroyDescriptorPool(_device, _descriptorPool, nullptr);
	_descriptorPool = VK_NULL_HANDLE;

	vkDestroyDescriptorSetLayout(_device, _descriptorSetLayout, nullptr);
	_descriptorSetLayout = VK_NULL_HANDLE;

	vkDestroyBuffer(_device, _indexBuffer, nullptr);
	_indexBuffer = VK_NULL_HANDLE;
	vkFreeMemory(_device, _indexBufferMemory, nullptr);
	_indexBufferMemory = VK_NULL_HANDLE;

	vkDestroyBuffer(_device, _vertexBuffer, nullptr);
	_vertexBuffer = VK_NULL_HANDLE;
	vkFreeMemory(_device, _vertexBufferMemory, nullptr);
	_vertexBufferMemory = VK_NULL_HANDLE;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		vkDestroySemaphore(_device, _imageAvailableSemaphores[i], nullptr);
		vkDestroySemaphore(_device, _renderFinishedSemaphores[i], nullptr);
		vkDestroyFence(_device, _inFlightFences[i], nullptr);
	}
	_imageAvailableSemaphores.clear();
	_renderFinishedSemaphores.clear();
	_inFlightFences.clear();

	vkDestroyPipeline(_device, _graphicsPipeline, nullptr);
	_graphicsPipeline = VK_NULL_HANDLE;

	vkDestroyPipelineLayout(_device, _pipelineLayout, nullptr);
	_pipelineLayout = VK_NULL_HANDLE;

	vkDestroyRenderPass(_device, _renderPass, nullptr);
	_renderPass = VK_NULL_HANDLE;

	vkDestroyCommandPool(_device, _commandPool, nullptr);
	_commandPool = VK_NULL_HANDLE;

	vkDestroyCommandPool(_device, _transferCommandPool, nullptr);
	_transferCommandPool = VK_NULL_HANDLE;

	vkDestroyDevice(_device, nullptr);
	_device = VK_NULL_HANDLE;

	if (_enableValidationLayers)
	{
		DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);
		_debugMessenger = VK_NULL_HANDLE;
	}

	vkDestroySurfaceKHR(_instance, _surface, nullptr);
	_surface = VK_NULL_HANDLE;

	vkDestroyInstance(_instance, nullptr);
	_instance = VK_NULL_HANDLE;

	return true;
}

bool flt::RendererVulkan::Render(float deltaTime)
{
	if (_isMinimized)
	{
		return true;
	}

	vkWaitForFences(_device, 1, &_inFlightFences[_currentFrame], VK_TRUE, UINT64_MAX);

	uint32_t imageIndex{};
	VkResult result = vkAcquireNextImageKHR(_device, _swapChain, UINT64_MAX, _imageAvailableSemaphores[_currentFrame], VK_NULL_HANDLE, &imageIndex);
	if (result == VK_ERROR_OUT_OF_DATE_KHR)
	{
		RecreateSwapChain();
		return true;
	}
	else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		ASSERT(false, "failed to acquire swap chain image!");
	}

	UpdateUniformBuffer(_currentFrame);

	vkResetFences(_device, 1, &_inFlightFences[_currentFrame]);

	vkResetCommandBuffer(_commandBuffers[_currentFrame], 0);
	RecordCommandBuffer(_commandBuffers[_currentFrame], imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &_commandBuffers[_currentFrame];
	VkSemaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	result = vkQueueSubmit(_graphicsQueue, 1, &submitInfo, _inFlightFences[_currentFrame]);
	if (result != VK_SUCCESS)
	{
		return false;
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { _swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr;

	result = vkQueuePresentKHR(_presentQueue, &presentInfo);
	if (result == VK_ERROR_OUT_OF_DATE_KHR 
		|| result == VK_SUBOPTIMAL_KHR
		|| _framebufferResized)
	{
		_framebufferResized = false;
		RecreateSwapChain();
	}
	else if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to present swap chain image!");
		return false;
	}

	_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	return true;
}

flt::HOBJECT flt::RendererVulkan::RegisterObject(RendererObject& renderable)
{
	return HOBJECT();
}

bool flt::RendererVulkan::DeregisterObject(HOBJECT renderable)
{
	return false;
}

bool flt::RendererVulkan::SetFullScreen(bool isFullScreen)
{
	return false;
}

bool flt::RendererVulkan::Resize(uint32 width, uint32 height)
{
	if (width == 0 || height == 0)
	{
		_isMinimized = true;
		return true;
	}

	_framebufferResized = true;
	_isMinimized = false;
	return true;
}

bool flt::RendererVulkan::CreateInstance()
{
	if (_enableValidationLayers && !CheckValidationLayerSupport())
	{
		return false;
	}

	/// 불칸 인스턴스 생성
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Hello Vulkan";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.pEngineName = "Floater";
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	std::vector<const char*> extensions = { "VK_KHR_surface", "VK_KHR_win32_surface" };
	if (_enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
	if (_enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugCreateInfo.pfnUserCallback = DebugCallback;
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	VkResult result = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (result != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

bool flt::RendererVulkan::SetupDebugMessenger()
{
	if (!_enableValidationLayers)
	{
		return true;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	createInfo.pfnUserCallback = DebugCallback;

	VkResult result = CreateDebugUtilsMessengerEXT(_instance, &createInfo, nullptr, &_debugMessenger);
	if (result != VK_SUCCESS) {
		return false;
	}

	return true;
}

bool flt::RendererVulkan::CreateWin32Surface(HWND hwnd)
{
	/// 윈도우 핸들로 surface 생성
	VkWin32SurfaceCreateInfoKHR surfaceCreateInfo{};
	surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceCreateInfo.hwnd = hwnd;
	surfaceCreateInfo.hinstance = GetModuleHandle(nullptr);

	VkResult result = vkCreateWin32SurfaceKHR(_instance, &surfaceCreateInfo, nullptr, &_surface);
	if (result != VK_SUCCESS)
	{
		return false;
	}

	return true;
}

bool flt::RendererVulkan::PickPhysicalDevice()
{
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		return false;
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			_physicalDevice = device;
			break;
		}
	}

	if (_physicalDevice == VK_NULL_HANDLE) {
		return false;
		//throw std::runtime_error("failed to find a suitable GPU!");
	}

	return true;
}

bool flt::RendererVulkan::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);

	std::optional<uint32_t> transferQueueFamilyIndex = FindTransferQueueFamilies(_physicalDevice);
	if (!transferQueueFamilyIndex.has_value())
	{
		ASSERT(false, "failed to find a suitable transfer queue family!");
		return false;
	}

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value(), transferQueueFamilyIndex.value()};


	float queuePriority = 1.0f;
	for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();

	createInfo.pEnabledFeatures = &deviceFeatures;

	createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = deviceExtensions.data();

	if (_enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	VkResult result = vkCreateDevice(_physicalDevice, &createInfo, nullptr, &_device);
	if (result != VK_SUCCESS)
	{
		return false;
	}

	vkGetDeviceQueue(_device, indices.graphicsFamily.value(), 0, &_graphicsQueue);
	vkGetDeviceQueue(_device, indices.presentFamily.value(), 0, &_presentQueue);
	vkGetDeviceQueue(_device, transferQueueFamilyIndex.value(), 0, &_transferQueue);

	return true;
}

bool flt::RendererVulkan::CreateSwapChain()
{
	///81p
	SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(_physicalDevice);

	VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
	VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
	VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = _surface;

	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(_physicalDevice);
	uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

	if (indices.graphicsFamily != indices.presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(_device, &createInfo, nullptr, &_swapChain) != VK_SUCCESS)
	{
		return false;
	}

	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, nullptr);
	_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(_device, _swapChain, &imageCount, _swapChainImages.data());

	_swapChainImageFormat = surfaceFormat.format;
	_swapChainExtent = extent;

	return true;
}

bool flt::RendererVulkan::CreateImageViews()
{
	_swapChainImageViews.resize(_swapChainImages.size());

	for (size_t i = 0; i < _swapChainImages.size(); ++i)
	{
		VkImageViewCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		createInfo.image = _swapChainImages[i];
		createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		createInfo.format = _swapChainImageFormat;
		createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		createInfo.subresourceRange.baseMipLevel = 0;
		createInfo.subresourceRange.levelCount = 1;
		createInfo.subresourceRange.baseArrayLayer = 0;
		createInfo.subresourceRange.layerCount = 1;

		VkResult result = vkCreateImageView(_device, &createInfo, nullptr, &_swapChainImageViews[i]);
		if (result != VK_SUCCESS)
		{
			return false;
		}
	}

	return true;
}

bool flt::RendererVulkan::CreateRenderPass()
{
	VkAttachmentDescription colorAttachment{};
	colorAttachment.format = _swapChainImageFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass{};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentRef;

	VkSubpassDependency dependency{};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.pAttachments = &colorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.pSubpasses = &subpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	VkResult result = vkCreateRenderPass(_device, &renderPassInfo, nullptr, &_renderPass);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create render pass!");
		return false;
	}

	return true;
}

bool flt::RendererVulkan::CreateDescriptorSetLayout()
{
	VkDescriptorSetLayoutBinding uboLayoutBinding{};
	uboLayoutBinding.binding = 0;
	uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	uboLayoutBinding.descriptorCount = 1;
	uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	uboLayoutBinding.pImmutableSamplers = nullptr;

	VkDescriptorSetLayoutCreateInfo layoutInfo{};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &uboLayoutBinding;

	VkResult result = vkCreateDescriptorSetLayout(_device, &layoutInfo, nullptr, &_descriptorSetLayout);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create descriptor set layout!");
		return false;
	}

	return true;
}

bool flt::RendererVulkan::CreateGraphicsPipeline()
{
	std::vector<char> vertShaderCode = ReadFile("shaders/vert.spv");
	std::vector<char> fragShaderCode = ReadFile("shaders/frag.spv");

	VkShaderModule vertShaderModule = CreateShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = CreateShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 0;
	vertexInputInfo.vertexAttributeDescriptionCount = 0;
	auto bindingDescription = VulkanVertex::GetBindingDescription();
	auto attributeDescriptions = VulkanVertex::GetAttributeDescriptions();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkPipelineViewportStateCreateInfo viewportState{};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.scissorCount = 1;

	VkPipelineRasterizationStateCreateInfo rasterizer{};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	//rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;

	VkPipelineMultisampleStateCreateInfo multisampling{};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

	VkPipelineColorBlendAttachmentState colorBlendAttachment{};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlending{};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY;
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates =
	{
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{};
	dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicState.pDynamicStates = dynamicStates.data();

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 1;
	pipelineLayoutInfo.pSetLayouts = &_descriptorSetLayout;
	//pipelineLayoutInfo.pushConstantRangeCount = 0;

	VkResult result = vkCreatePipelineLayout(_device, &pipelineLayoutInfo, nullptr, &_pipelineLayout);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create pipeline layout!");
		return false;
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = nullptr;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = &dynamicState;
	pipelineInfo.layout = _pipelineLayout;
	pipelineInfo.renderPass = _renderPass;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	//pipelineInfo.basePipelineIndex = -1;

	result = vkCreateGraphicsPipelines(_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &_graphicsPipeline);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create graphics pipeline!");
		return false;
	}

	vkDestroyShaderModule(_device, fragShaderModule, nullptr);
	vkDestroyShaderModule(_device, vertShaderModule, nullptr);
	return true;
}

bool flt::RendererVulkan::CreateFramebuffers()
{
	_swapChainFramebuffers.resize(_swapChainImageViews.size());

	for (size_t i = 0; i < _swapChainFramebuffers.size(); ++i)
	{
		VkImageView attachments[] = { _swapChainImageViews[i] };

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = _renderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.pAttachments = attachments;
		framebufferInfo.width = _swapChainExtent.width;
		framebufferInfo.height = _swapChainExtent.height;
		framebufferInfo.layers = 1;

		VkResult result = vkCreateFramebuffer(_device, &framebufferInfo, nullptr, &_swapChainFramebuffers[i]);
		if (result != VK_SUCCESS)
		{
			ASSERT(false, "failed to create framebuffer!");
			return false;
		}
	}
	return true;
}

bool flt::RendererVulkan::CreateCommandPool()
{
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(_physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

	VkResult result = vkCreateCommandPool(_device, &poolInfo, nullptr, &_commandPool);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create command pool!");
		return false;
	}


	std::optional<uint32_t> transferQueueFamilyIndex = FindTransferQueueFamilies(_physicalDevice);
	if (!transferQueueFamilyIndex.has_value())
	{
		ASSERT(false, "failed to find a suitable transfer queue family!");
		return false;
	}

	VkCommandPoolCreateInfo transferPoolInfo{};
	transferPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	transferPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	transferPoolInfo.queueFamilyIndex = transferQueueFamilyIndex.value();

	result = vkCreateCommandPool(_device, &transferPoolInfo, nullptr, &_transferCommandPool);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create transfer command pool!");
		return false;
	}

	return true;
}

bool flt::RendererVulkan::CreateVertexBuffer()
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	VkBuffer stagingBuffer{};
	VkDeviceMemory stagingBufferMemory{};
	bool result = CreateBuffer(
		bufferSize
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		, stagingBuffer
		, stagingBufferMemory, VK_SHARING_MODE_EXCLUSIVE);
	if (!result)
	{
		return false;
	}

	void* data;
	vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(_device, stagingBufferMemory);

	result = CreateBuffer(
		bufferSize
		, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		, _vertexBuffer
		, _vertexBufferMemory, VK_SHARING_MODE_EXCLUSIVE);
	if (!result)
	{
		return false;
	}

	CopyBuffer(stagingBuffer, _vertexBuffer, bufferSize);

	vkDestroyBuffer(_device, stagingBuffer, nullptr);
	vkFreeMemory(_device, stagingBufferMemory, nullptr);

	return true;
}

bool flt::RendererVulkan::CreateIndexBuffer()
{
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	VkBuffer stagingBuffer{};
	VkDeviceMemory stagingBufferMemory{};
	bool result = CreateBuffer(bufferSize
		, VK_BUFFER_USAGE_TRANSFER_SRC_BIT
		, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		, stagingBuffer
		, stagingBufferMemory, VK_SHARING_MODE_EXCLUSIVE);
	if (!result)
	{
		return false;
	}

	void* data;
	vkMapMemory(_device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(_device, stagingBufferMemory);

	result = CreateBuffer(bufferSize
		, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
		, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
		, _indexBuffer
		, _indexBufferMemory, VK_SHARING_MODE_EXCLUSIVE);
	if (!result)
	{
		return false;
	}

	CopyBuffer(stagingBuffer, _indexBuffer, bufferSize);

	vkDestroyBuffer(_device, stagingBuffer, nullptr);
	vkFreeMemory(_device, stagingBufferMemory, nullptr);

	return true;
}

bool flt::RendererVulkan::CreateUniformBuffers()
{
	//VkDeviceSize bufferSize = sizeof(UniformBufferObject);
	VkDeviceSize bufferSize = sizeof(UniformBufferObjectGLM);

	_uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
	_uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
	_uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		bool result = CreateBuffer(bufferSize
			, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
			, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
			, _uniformBuffers[i]
			, _uniformBuffersMemory[i]);

		if (!result)
		{
			ASSERT(false, "failed to create uniform buffer!");
			return false;
		}

		VkResult ret = vkMapMemory(_device, _uniformBuffersMemory[i], 0, bufferSize, 0, &_uniformBuffersMapped[i]);
		if (ret != VK_SUCCESS)
		{
			ASSERT(false, "failed to map uniform buffer memory!");
			return false;
			}

	}

	return true;
}

bool flt::RendererVulkan::CreateDescriptorPool()
{
	VkDescriptorPoolSize poolSize{};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

	VkResult result = vkCreateDescriptorPool(_device, &poolInfo, nullptr, &_descriptorPool);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create descriptor pool!");
		return false;
	}

	return true;
}

bool flt::RendererVulkan::CreateDescriptorSets()
{
	std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, _descriptorSetLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = _descriptorPool;
	allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
	allocInfo.pSetLayouts = layouts.data();

	_descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
	VkResult result = vkAllocateDescriptorSets(_device, &allocInfo, _descriptorSets.data());
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to allocate descriptor sets!");
		return false;
	}

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = _uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObjectGLM);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = _descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr;
		descriptorWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(_device, 1, &descriptorWrite, 0, nullptr);
	}




	return true;
}

bool flt::RendererVulkan::CreateCommandBuffer()
{
	/// graphics, present용 command buffer 생성
	_commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)_commandBuffers.size();

	VkResult result = vkAllocateCommandBuffers(_device, &allocInfo, _commandBuffers.data());
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to allocate command buffers!");
		return false;
	}

	///// transfer용 command buffer 생성
	//VkCommandBufferAllocateInfo transferAllocInfo{};

	return true;
}

bool flt::RendererVulkan::CreateSyncObjects()
{
	_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
	{
		VkResult result = vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]);
		if (result != VK_SUCCESS)
		{
			ASSERT(false, "failed to create synchronization objects!");
			return false;
		}

		result = vkCreateSemaphore(_device, &semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]);
		if (result != VK_SUCCESS)
		{
			ASSERT(false, "failed to create synchronization objects!");
			return false;
		}

		result = vkCreateFence(_device, &fenceInfo, nullptr, &_inFlightFences[i]);
		if (result != VK_SUCCESS)
		{
			ASSERT(false, "failed to create synchronization objects!");
			return false;
		}
	}

	return true;
}

void flt::RendererVulkan::CleanupSwapChain()
{
	for (size_t i = 0; i < _swapChainFramebuffers.size(); ++i)
	{
		vkDestroyFramebuffer(_device, _swapChainFramebuffers[i], nullptr);
	}

	for (size_t i = 0; i < _swapChainImageViews.size(); ++i)
	{
		vkDestroyImageView(_device, _swapChainImageViews[i], nullptr);
	}

	vkDestroySwapchainKHR(_device, _swapChain, nullptr);
}

void flt::RendererVulkan::RecreateSwapChain()
{
	//최소화 됐을 경우 (너비와 높이가 0이 될 경우) 재생성하지 않음

	vkDeviceWaitIdle(_device);

	CleanupSwapChain();

	CreateSwapChain();
	CreateImageViews();
	CreateFramebuffers();
}

bool flt::RendererVulkan::RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	//beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkResult result = vkBeginCommandBuffer(commandBuffer, &beginInfo);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to begin recording command buffer!");
		return false;
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = _renderPass;
	renderPassInfo.framebuffer = _swapChainFramebuffers[imageIndex];
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = _swapChainExtent;

	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;

	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _graphicsPipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)_swapChainExtent.width;
	viewport.height = (float)_swapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = _swapChainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	VkBuffer vertexBuffers[] = { _vertexBuffer };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdBindIndexBuffer(commandBuffer, _indexBuffer, 0, VK_INDEX_TYPE_UINT16);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSets[_currentFrame], 0, nullptr);

	//vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertices.size()), 1, 0, 0);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

	vkCmdEndRenderPass(commandBuffer);

	result = vkEndCommandBuffer(commandBuffer);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to record command buffer!");
		return false;
	}
	return true;
}

void flt::RendererVulkan::UpdateUniformBuffer(uint32_t currentImage)
{
	static auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();
	float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

	UniformBufferObjectGLM ubo{};
	ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	ubo.proj = glm::perspective(glm::radians(45.0f), _swapChainExtent.width / (float)_swapChainExtent.height, 0.1f, 10.0f);
	ubo.proj[1][1] *= -1;

	auto size = sizeof(ubo);
	memcpy(_uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

bool flt::RendererVulkan::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : validationLayers)
	{
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (!layerFound)
		{
			return false;
		}
	}

	return true;
}

bool flt::RendererVulkan::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool flt::RendererVulkan::IsDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices{};
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

		if (presentSupport) {
			indices.presentFamily = i;
		}

		if (indices.IsComplete()) {
			break;
		}

		++i;
	}

	bool extensionsSupported = CheckDeviceExtensionSupport(device);

	bool swapChainAdequate = false;
	if (extensionsSupported) {
		SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.IsComplete() && extensionsSupported && swapChainAdequate;
}

SwapChainSupportDetails flt::RendererVulkan::QuerySwapChainSupport(VkPhysicalDevice device)
{
	SwapChainSupportDetails details{};

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, _surface, &details.capabilities);

	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, nullptr);

	if (formatCount != 0) {
		details.formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, _surface, &formatCount, details.formats.data());
	}

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, nullptr);

	if (presentModeCount != 0) {
		details.presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, _surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

QueueFamilyIndices flt::RendererVulkan::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilyCount; ++i)
	{
		const auto& queueFamily = queueFamilies[i];

		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);

		if (presentSupport) 
		{
			indices.presentFamily = i;
		}

		if (indices.IsComplete()) 
		{
			break;
		}
	}

	return indices;
}

std::optional<uint32_t> flt::RendererVulkan::FindTransferQueueFamilies(VkPhysicalDevice device)
{
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	for (uint32_t i = 0; i < queueFamilyCount; ++i)
	{
		if (queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT 
			&& !(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			return i;
		}
	}

	return std::nullopt;
}

VkSurfaceFormatKHR flt::RendererVulkan::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}

	return availableFormats[0];
}

VkPresentModeKHR flt::RendererVulkan::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}

	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D flt::RendererVulkan::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	if (capabilities.currentExtent.width != UINT_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width = 0;
		int height = 0;
		//glfwGetFramebufferSize(window, &width, &height);

		VkExtent2D actualExtent
		{
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

		return actualExtent;
	}
}

VkShaderModule flt::RendererVulkan::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	VkResult result = vkCreateShaderModule(_device, &createInfo, nullptr, &shaderModule);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create shader module!");
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}

bool flt::RendererVulkan::CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory, VkSharingMode sharingMode)
{
	VkBufferCreateInfo bufferInfo{};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = size;
	bufferInfo.usage = usage;
	bufferInfo.sharingMode = sharingMode; // default, graphics queue에서 사용할 때
	//bufferInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
	uint32_t queueFamilyIndices[] = { 0, 1 };
	bufferInfo.queueFamilyIndexCount = 2;
	bufferInfo.pQueueFamilyIndices = queueFamilyIndices;

	VkResult result = vkCreateBuffer(_device, &bufferInfo, nullptr, &buffer);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to create buffer!");
		return false;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(_device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

	result = vkAllocateMemory(_device, &allocInfo, nullptr, &bufferMemory);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to allocate buffer memory!");
		return false;
	}

	result = vkBindBufferMemory(_device, buffer, bufferMemory, 0);
	if (result != VK_SUCCESS)
	{
		ASSERT(false, "failed to bind buffer memory!");
		return false;
	}

	return true;
}

void flt::RendererVulkan::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	//allocInfo.commandPool = _transferCommandPool;
	allocInfo.commandPool = _commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	VkBufferCopy copyRegion{};
	copyRegion.srcOffset = 0;
	copyRegion.dstOffset = 0;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	//vkQueueSubmit(_transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
	//vkQueueWaitIdle(_transferQueue);
	vkQueueSubmit(_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(_graphicsQueue);

	vkFreeCommandBuffers(_device, _commandPool, 1, &commandBuffer);
}

uint32_t flt::RendererVulkan::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties{};
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
	{
		if ((typeFilter & (1 << i))
			&& ((memProperties.memoryTypes[i].propertyFlags & properties) == properties))
		{
			return i;
		}
	}

	ASSERT(false, "failed to find suitable memory type!");
	return UINT_MAX;
}
