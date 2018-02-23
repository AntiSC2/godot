/*************************************************************************/
/*  rasterizer_vulkan.cpp                                                */
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

#include "rasterizer_vulkan.h"

#include "vulkan_device/vulkan_device.h"
#include "os/os.h"
#include "project_settings.h"

RasterizerStorage *RasterizerVulkan::get_storage() {

	return NULL;
}

RasterizerCanvas *RasterizerVulkan::get_canvas() {

	return NULL;
}

RasterizerScene *RasterizerVulkan::get_scene() {

	return NULL;
}

void RasterizerVulkan::initialize() {

	if (OS::get_singleton()->is_stdout_verbose()) {
		print_line("Using Vulkan video driver");
	}

}

void RasterizerVulkan::begin_frame() {

	uint64_t tick = OS::get_singleton()->get_ticks_usec();

	double delta = double(tick - prev_ticks) / 1000000.0;
	delta *= Engine::get_singleton()->get_time_scale();

	time_total += delta;

	if (delta == 0) {
		//to avoid hiccups
		delta = 0.001;
	}

	prev_ticks = tick;

	double time_roll_over = GLOBAL_GET("rendering/limits/time/time_rollover_secs");
	if (time_total > time_roll_over)
		time_total = 0; //roll over every day (should be customz

}

void RasterizerVulkan::set_current_render_target(RID p_render_target) {

	;
}

void RasterizerVulkan::restore_render_target() {

	;
}

void RasterizerVulkan::clear_render_target(const Color &p_color) {

	;
}

void RasterizerVulkan::set_boot_image(const Ref<Image> &p_image, const Color &p_color, bool p_scale) {

	;
}

void RasterizerVulkan::blit_render_target_to_screen(RID p_render_target, const Rect2 &p_screen_rect, int p_screen) {

	;
}

void RasterizerVulkan::end_frame(bool p_swap_buffers) {

	;
}

void RasterizerVulkan::finalize() {

	;
}

Rasterizer *RasterizerVulkan::_create_current() {

	return memnew(RasterizerVulkan);
}

void RasterizerVulkan::make_current() {
	_create_func = _create_current;
}

void RasterizerVulkan::register_config() {

	GLOBAL_DEF("rendering/quality/filters/use_nearest_mipmap_filter", false);
	GLOBAL_DEF("rendering/quality/filters/anisotropic_filter_level", 4);
	ProjectSettings::get_singleton()->set_custom_property_info("rendering/quality/filters/anisotropic_filter_level", PropertyInfo(Variant::INT, "rendering/quality/filters/anisotropic_filter_level", PROPERTY_HINT_RANGE, "1,16,1"));
	GLOBAL_DEF("rendering/limits/time/time_rollover_secs", 3600);
}

RasterizerVulkan::RasterizerVulkan() {

	prev_ticks = 0;
	time_total = 0;
}

RasterizerVulkan::~RasterizerVulkan() {

	;
}
