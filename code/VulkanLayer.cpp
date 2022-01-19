#include "GFXLayer.h"

struct VulkanState
{
	VkInstance instance;
};

global VulkanState _vkState;

void CreateInstance()
{
	// app info
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = NULL;
	appInfo.pApplicationName = "vmorph";
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = "vmorph";
	appInfo.apiVersion = VK_API_VERSION_1_0;

	// instance create info
	VkInstanceCreateInfo instInfo = {};
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instInfo.pNext = NULL;
	instInfo.flags = 0;
	instInfo.pApplicationInfo = &appInfo;
	instInfo.enabledExtensionCount = 0;
	instInfo.ppEnabledExtensionNames = NULL;
	instInfo.enabledLayerCount = 0;
	instInfo.ppEnabledLayerNames = 0;

	VkResult res;
	res = vkCreateInstance(&instInfo, NULL, &_vkState.instance);
	if(res == VK_ERROR_INCOMPATIBLE_DRIVER) {
		printf("cannot find compatible Vulkan ICD\n");
		exit(-1);
	}
	else if(res) {
		printf("unkown vulkan initialization error\n");
		exit(-1);
	}

}

void GFXLayerInit()
{
	CreateInstance();
}

void GFXLayerCleanup()
{
	vkDestroyInstance(_vkState.instance, NULL);
}
