#include "world_modulation.h"

#include "../config_vars.h"

#include "../../base/sdk.h"

#include "../../base/other/color.h"
#include "../../base/other/game_functions.h"

#include "../../base/sdk/entity.h"

#include "../features.h"

struct color24_t {
	unsigned char r{}, g{}, b{};
};

std::vector<std::string> skybox_list{
	xor_strs::aa_default.c_str(),
	xor_str("cs_tibet"),
	xor_str("cs_baggage_skybox_"),
	xor_str("italy"),
	xor_str("jungle"),
	xor_str("office"),
	xor_str("sky_cs15_daylight01_hdr"),
	xor_str("sky_cs15_daylight02_hdr"),
	xor_str("vertigoblue_hdr"),
	xor_str("vertigo"),
	xor_str("sky_day02_05_hdr"),
	xor_str("nukeblank"),
	xor_str("sky_venice"),
	xor_str("sky_cs15_daylight03_hdr"),
	xor_str("sky_cs15_daylight04_hdr"),
	xor_str("sky_csgo_cloudy01"),
	xor_str("sky_csgo_night02"),
	xor_str("sky_csgo_night02b"),
	xor_str("sky_csgo_night_flat"),
	xor_str("sky_dust"),
	xor_str("vietnam"),
	xor_str("embassy"),
	xor_str("Custom")
};

void c_world_modulation::skybox_changer() {
	if (g_ctx.uninject) {
		func_ptrs::load_named_sky(g_ctx.sky_name.c_str());
		return;
	}

	bool nightmode = g_cfg.misc.skybox > 0;

	g_cfg.misc.skybox = std::clamp(g_cfg.misc.skybox, 0, 22);

	const auto sky_name = g_cfg.misc.skybox == 22 ? g_cfg.misc.skybox_name : skybox_list[g_cfg.misc.skybox].c_str();
	if (nightmode && old_sky_name != sky_name) {
		func_ptrs::load_named_sky(sky_name);
		old_sky_name = sky_name;
	}
	else if (!nightmode && old_sky_name != g_ctx.sky_name) {
		func_ptrs::load_named_sky(g_ctx.sky_name.c_str());
		old_sky_name = g_ctx.sky_name;
	}

	// disable 3d sky
	g_ctx.local->skybox_area() = 255;
}

void c_world_modulation::fog_changer() {
	if (!(g_cfg.misc.custom_fog)) {
		if (cvars::fog_override->get_int())
			cvars::fog_override->set_value(FALSE);

		return;
	}

	if (!cvars::fog_override->get_int())
		cvars::fog_override->set_value(TRUE);

	if (cvars::fog_start->get_int() != g_cfg.misc.fog_start)
		cvars::fog_start->set_value(g_cfg.misc.fog_start);

	if (cvars::fog_end->get_int() != g_cfg.misc.fog_end)
		cvars::fog_end->set_value(g_cfg.misc.fog_end);

	float density = g_cfg.misc.fog_density / 100.f;
	if (cvars::fog_maxdensity->get_float() != density)
		cvars::fog_maxdensity->set_value(density);

	auto cur_clr = g_cfg.misc.world_clr[fog];

	char buffer_color[12];
	sprintf_s(buffer_color, 12, xor_c("%i %i %i"), cur_clr.r(), cur_clr.g(), cur_clr.b());

	if (strcmp(cvars::fog_color->get_string(), buffer_color))
		cvars::fog_color->set_value(buffer_color);
}

void c_world_modulation::sunset_mode() {
	cvars::cl_csm_rot_override->callbacks.m_size = 0;
	cvars::cl_csm_rot_x->callbacks.m_size = 0;
	cvars::cl_csm_rot_y->callbacks.m_size = 0;
	cvars::cl_csm_rot_z->callbacks.m_size = 0;

	// it's -1.f, -1.f, -0.01f converted to angles
	auto shadow_angles = vector3d(0.405136f, 225.f, 0.f);

	bool sunset = (g_cfg.misc.world_modulation & 2);
	cvars::cl_csm_rot_override->set_value((int)sunset);
	cvars::cl_csm_rot_x->set_value(shadow_angles.x);
	cvars::cl_csm_rot_y->set_value(shadow_angles.y);
	cvars::cl_csm_rot_z->set_value(shadow_angles.z);
}

void c_world_modulation::light_props_modulation() {
	auto& light_array = g_listener_entity->get_entity(ent_light);
	if (light_array.empty())
		return;

	auto cur_clr = g_cfg.misc.world_clr[props];
	auto custom_clr = color24_t(cur_clr.r() / 255, cur_clr.g() / 255, cur_clr.b() / 255 );
	auto white_clr = color24_t(255, 255, 255);

	for (const auto& l : light_array) {
		auto entity = l.m_entity;
		if (!entity)
			continue;

		auto& clr_render = *(color24_t*)((uintptr_t)entity + 0x70);
		clr_render = (g_cfg.misc.world_modulation & 1) ? custom_clr : white_clr;
	}
}

void c_world_modulation::on_render_start(int stage) {
	if (!g_ctx.local)
		return;

	if (!g_ctx.in_game)
		return;

	if (stage != frame_render_start)
		return;

	this->fog_changer();
	this->skybox_changer();
	this->sunset_mode();
	this->light_props_modulation();
}