/*************************************************************************/
/*  context_gl_x11.cpp                                                   */
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

#include "vulkan_device_x11.h"

#ifdef X11_ENABLED
#ifdef VULKAN_ENABLED
#include "core/vector.h"
#include <vulkan/vulkan.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xutil.h>

struct VulkanDevice_Private {
	VkInstance vk_instance;	
	uint32_t device_count;
	uint32_t extension_count;
	uint32_t enabled_layer_count;
	Vector<VkPhysicalDevice> devices;
	Vector<const char *> extension_names;
	Vector<const char *> layer_names;
};

void VulkanDevice_X11::release_current() {

}

void VulkanDevice_X11::make_current() {

}

void VulkanDevice_X11::swap_buffers() {

}

static bool ctxErrorOccurred = false;
static int ctxErrorHandler(Display *dpy, XErrorEvent *ev) {
	ctxErrorOccurred = true;
	return 0;
}

static void set_class_hint(Display *p_display, Window p_window) {
	XClassHint *classHint;

	/* set the name and class hints for the window manager to use */
	classHint = XAllocClassHint();
	if (classHint) {
		classHint->res_name = (char *)"Godot_Engine";
		classHint->res_class = (char *)"Godot";
	}
	XSetClassHint(p_display, p_window, classHint);
	XFree(classHint);
}

Error VulkanDevice_X11::initialize() {
	create_vulkan_instance();
	pick_physical_device();
	initialize_logical_device();
	/*VkXlibSurfaceCreateInfoKHR xlib = { VK_STRUCTURE_TYPE_XLIB_SURFACE_CREATE_INFO_KHR };
	xlib.flags = 0;
	xlib.dpy = (Display *)winsysData1_;
	xlib.window = (Window)winsysData2_;
	VkResult res = vkCreateXlibSurfaceKHR(instance_, &xlib, nullptr, &surface_);
	assert(res == VK_SUCCESS);
	x11_window = XCreateWindow(x11_display, RootWindow(x11_display, vi->screen), 0, 0, OS::get_singleton()->get_video_mode().width, OS::get_singleton()->get_video_mode().height, 0, vi->depth, InputOutput, vi->visual, CWBorderPixel | CWColormap | CWEventMask, &swa);
	ERR_FAIL_COND_V(!x11_window, ERR_UNCONFIGURED);
	set_class_hint(x11_display, x11_window);
	XMapWindow(x11_display, x11_window);

	int (*oldHandler)(Display *, XErrorEvent *) =
			XSetErrorHandler(&ctxErrorHandler);



	XSync(x11_display, False);
	XSetErrorHandler(oldHandler);
	*/
	return OK;
}

int VulkanDevice_X11::get_window_width() {

	XWindowAttributes xwa;
	XGetWindowAttributes(x11_display, x11_window, &xwa);

	return xwa.width;
}

int VulkanDevice_X11::get_window_height() {
	XWindowAttributes xwa;
	XGetWindowAttributes(x11_display, x11_window, &xwa);

	return xwa.height;
}

void VulkanDevice_X11::set_use_vsync(bool p_use) {
	
	use_vsync = p_use;
}
bool VulkanDevice_X11::is_using_vsync() const {

	return use_vsync;
}

VulkanDevice_X11::VulkanDevice_X11(::Display *p_x11_display, ::Window &p_x11_window, const OS::VideoMode &p_default_video_mode) :
		x11_window(p_x11_window) {

	default_video_mode = p_default_video_mode;
	x11_display = p_x11_display;

	double_buffer = false;
	direct_render = false;
	p = memnew(VulkanDevice_Private);
	use_vsync = false;
}

VulkanDevice_X11::~VulkanDevice_X11() {

	release_current();
	memdelete(p);
}

#endif
#endif
