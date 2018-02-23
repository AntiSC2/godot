/*************************************************************************/
/*  vulkan_device.cpp                                                    */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2018 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2018 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "vulkan_device.h"

#if defined(VULKAN_ENABLED)
#include "core/vector.h"
#include "core/version_generated.gen.h"
#include <stdio.h>
#ifndef VULKAN_INCLUDE_H
#include <vulkan/vulkan.h>
#else
#include VULKAN_INCLUDE_H
#endif

struct VulkanDevice_Private {
	VkInstance vk_instance;	
	uint32_t device_count;
	uint32_t extension_count;
	uint32_t enabled_layer_count;
	Vector<VkPhysicalDevice> devices;
	Vector<const char *> extension_names;
	Vector<const char *> layer_names;
};

VulkanDevice *VulkanDevice::singleton = NULL;

bool is_suitable_device(VkPhysicalDevice device) {

	return true;
}

void VulkanDevice::create_vulkan_instance() {
	if (p == NULL) {
		printf("There is not a private member for VulkanDevice! Returning...\n");
		return;
	}

	vkEnumerateInstanceExtensionProperties(nullptr, &p->extension_count, nullptr);
	VkExtensionProperties *extensions = VK_NULL_HANDLE;
	extensions = memnew_arr(VkExtensionProperties, p->extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &p->extension_count, extensions);

	for (int i = 0; i < p->extension_count; i++) {
		p->extension_names.push_back(extensions[i].extensionName);
	}

	printf("Extensions 1: %s\n", p->extension_names[0]);
	memdelete_arr(extensions);

	VkApplicationInfo app_info = {};
	VkInstanceCreateInfo instance_info = {};

	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Godot Editor";
	app_info.applicationVersion = VK_MAKE_VERSION(VERSION_MAJOR, VERSION_MINOR, 0);
	app_info.pEngineName = "Godot Engine";
	app_info.engineVersion = VK_MAKE_VERSION(3, 1, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info.pApplicationInfo = &app_info;
	instance_info.enabledExtensionCount = p->extension_count;
	instance_info.ppEnabledExtensionNames = p->extension_names.ptr();
	instance_info.enabledLayerCount = p->enabled_layer_count;
	instance_info.ppEnabledLayerNames = p->layer_names.ptr();

	VkResult result = vkCreateInstance(&instance_info, nullptr, &p->vk_instance);
	if (result != VK_SUCCESS)
		ERR_FAIL();
}

void VulkanDevice::pick_physical_device() {
	if (p == NULL) {
		printf("There is not a private member for VulkanDevice! Returning...\n");
		return;
	}

	vkEnumeratePhysicalDevices(p->vk_instance, &p->device_count, nullptr);

	if (p->device_count == 0) {
		printf("No GPU with Vulkan support was found...\n");
		return;
	}

	VkPhysicalDevice *candidates = VK_NULL_HANDLE;
	candidates = memnew_arr(VkPhysicalDevice, p->device_count);
	vkEnumeratePhysicalDevices(p->vk_instance, &p->device_count, candidates);	

	for (int i = 0; i < p->device_count; i++) {
		if (is_suitable_device(candidates[i]))
			p->devices.push_back(candidates[i]);
	}

	memdelete_arr(candidates);

	if (p->device_count == 0) {
		printf("No suitable GPU was found...\n");
		return;
	}
}

void VulkanDevice::initialize_logical_device() {

	;
}

VulkanDevice *VulkanDevice::get_singleton() {

	return singleton;
}

VulkanDevice::VulkanDevice() {

	ERR_FAIL_COND(singleton);

	singleton = this;
}

VulkanDevice::~VulkanDevice() {
	if (p != NULL)
		vkDestroyInstance(p->vk_instance, nullptr);

	if (singleton == this)
		singleton = NULL;
}

#endif
