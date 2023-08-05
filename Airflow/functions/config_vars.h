#pragma once
#include "../base/other/color.h"
#include "menu/menu.h"

#include <array>

enum fov_t {
	world = 0,
	arms,
	zoom
};

enum world_clr_t {
	walls = 0,
	props,
	sky,
	fog
};

enum weapon_group_t {
	global = 0,
	auto_snipers,
	heavy_pistols,
	pistols,
	scout,
	awp,
	weapon_max
};

enum quick_stop_options_t {
	early			= 1,
	between_shots	= 2,
	force_accuracy	= 4,
};

enum removals_t {
	scope			= 1,
	vis_recoil		= 2,
	post_process	= 4,
	smoke			= 8,
	flash			= 16,
	fog_			= 32,
	shadow			= 64,
	viewmodel_move	= 128,
};

enum rage_hitbox_t {
	head		= 1,
	chest		= 2,
	stomach		= 4,
	pelvis		= 8,
	arms_		= 16,
	legs		= 32
};

enum binds_idx_t {
	fd_b = 0,
	sw_b,
	dt_b,
	hs_b,
	ap_b,
	tp_b,
	inv_b,
	edge_b,
	left_b,
	right_b,
	back_b,
	override_dmg_b,
	ens_lean_b,
	spike_b,
	force_body_b,
	binds_max
};

enum esp_type_t {
	esp_enemy,
	esp_weapon,
	esp_max
};

enum chams_type_t {
	c_vis,
	c_xqz,
	c_history,
	c_onshot,
	c_ragdolls,
	c_local,
	c_viewmodel,
	c_wpn,
	c_attachments,
	c_fake,
	c_max
};

enum weapon_cfg_type_t {
	weapon_cfg_deagle = 0,
	weapon_cfg_duals,
	weapon_cfg_fiveseven,
	weapon_cfg_glock,
	weapon_cfg_ak47,
	weapon_cfg_aug,
	weapon_cfg_awp,
	weapon_cfg_famas,
	weapon_cfg_g3sg1,
	weapon_cfg_galil,
	weapon_cfg_m249,
	weapon_cfg_m4a1,
	weapon_cfg_m4a1s,
	weapon_cfg_mac10,
	weapon_cfg_p90,
	weapon_cfg_mp5sd,
	weapon_cfg_ump45,
	weapon_cfg_xm1014,
	weapon_cfg_bizon,
	weapon_cfg_mag7,
	weapon_cfg_negev,
	weapon_cfg_sawedoff,
	weapon_cfg_tec9,
	weapon_cfg_p2000,
	weapon_cfg_mp7,
	weapon_cfg_mp9,
	weapon_cfg_nova,
	weapon_cfg_p250,
	weapon_cfg_scar20,
	weapon_cfg_sg556,
	weapon_cfg_ssg08,
	weapon_cfg_usps,
	weapon_cfg_cz75,
	weapon_cfg_revolver,
	weapon_cfg_knife,
	weapon_cfg_max
};

struct skin_weapon_t {
	bool	enable{};
	int		skin{};
	int		knife_model{};
	int		old_model = -1;
	int		old_skin = -1;

	char	skins_search[256]{};
};

struct chams_t {
	bool	enable{};
	bool	interp{};

	int		material{};
	int		glow_fill = 100;
	int		shot_duration = 5;

	color main_color = color(255, 255, 255, 255);
	color glow_color = color(255, 255, 255, 255);
};

struct key_bind_info_t;

struct anti_aim_angles_t
{
	int		pitch{};
	int		yaw{};

	bool	at_targets{};

	int		jitter_mode{};
	int		jitter_range{};

	int		distortion_range = 60;
	int		crooked_offset = 90;
	int		distortion_speed = 25;
	bool	random_speed = false;

	int		yaw_add{};
};

struct weapon_config_t {
	bool			enable{};
	bool			quick_stop{};
	bool			auto_scope{};

	int				hitchance{};
	int				mindamage = 1;
	int				damage_override = 1;
	int				scale_head = 80;
	int				scale_body = 80;
	int				group_type{};
	int				hitbox_type{};
	int				target_selection{};

	unsigned int	hitboxes{};
	unsigned int	quick_stop_options{};

	std::array<unsigned int, 6> hitbox_cond{ };
};

struct configs_t {
	struct ragebot_t {
		bool	enable{};
		bool	delay_shot{};

		bool	auto_fire{};
		bool	jitter_fix{};
		bool	adaptive_spike{};
		int		spike_amt{};

		int		group_type{};

		std::array<weapon_config_t, 6>	weapon{};
	}rage;

	struct anti_hit_t {
		bool	enable{};

		int condition_type{};

		std::array<anti_aim_angles_t, 3> angles{};

		bool	desync{};
		int		desync_type{};
		int		desync_range = 90;

		bool			fakelag{};
		unsigned int	fakelag_conditions{};
		int				fakelag_limit = 14;
		int				fakewalk_speed = 14;
	}antihit;

	struct visuals_t {
		chams_t chams[c_max]{
			chams_t{false, false, 0, 100, 5, color(116, 113, 254), color(80, 78, 138, 150)}, // C_VIS
			chams_t{false, false, 0, 100, 5, color(77, 104, 253), color(80, 78, 138, 150)}, // C_XQZ
			chams_t{false, false, 3, 100, 5, color(169, 166, 255, 80), color(80, 78, 138, 80)}, // C_HISTORY 
			chams_t{false, false, 4, 100, 5, color(0, 0, 0, 0), color(102, 92, 138, 100)}, // C_ONSHOT
			chams_t{false, false, 0, 100, 5, color(108, 126, 184), color(80, 78, 138, 150)}, // C_RAGDOLLS
			chams_t{false, false, 3, 100, 5, color(148, 168, 255, 100), color(100, 100, 100, 150)}, // C_LOCAL
			chams_t{false, false, 3, 100, 5, color(169, 125, 255, 100), color(168, 169, 255, 150)}, // C_VIEWMODEL
			chams_t{false, false, 3, 100, 5, color(218, 125, 255, 100), color(100, 100, 100, 150)}, // C_WPN
			chams_t{false, false, 3, 100, 5, color(218, 125, 255, 100), color(100, 100, 100, 150)}, // C_ATTACHMENTS
			chams_t{false, false, 3, 100, 5, color(255, 255, 255, 100), color(0, 0, 0, 0)}, // C_FAKE
		};

		struct event_logs_t {
			bool			enable{};
			bool			filter_console{};
			unsigned int	logs{};
		}eventlog;

		struct esp_t {
			bool			enable{ };
			unsigned int	elements{};

			struct esp_colors_t {
				color box				= color(255, 255, 255, 255);
				color name				= color(255, 255, 255, 255);
				color health			= color(162, 255, 115);
				color weapon			= color(255, 255, 255, 255);
				color ammo_bar			= color(164, 128, 255);
				color offscreen_arrow	= color(255, 0, 0);
				color glow				= color(204, 94, 253, 130);
				color skeleton			= color(219, 232, 255, 130);

				color smoke_range		= color(94, 112, 251, 50);
				color molotov_range		= color(251, 94, 204, 50);
			}colors;
		};

		std::array<esp_t, esp_max> esp{};

		bool			grenade_predict{};

		color	predict_clr = color(156, 150, 255, 255);

		bool			grenade_warning{};
		bool			grenade_warning_line{};

		color	warning_clr = color(156, 200, 255, 255);
	}visuals;

	struct misc_t {
		bool	menu = false;

		bool	auto_jump{};
		bool	auto_strafe{};

		bool	fast_stop{};
		bool	slide_walk{};

		int		fovs[3] = { 0, 0, 100 };
		int		viewmodel_pos[3] = {};

		bool	viewmodel_scope{};
		bool	fix_sensitivity{};
		bool	skip_second_zoom{};

		int		aspect_ratio{};
		int		strafe_smooth = 70;

		int		thirdperson_dist = 150;
		bool	thirdperson_dead{};

		bool	blend_scope{};
		int		scope_amt = 50;

		bool	impacts{};

		color server_clr = color(0, 0, 255, 127);
		color client_clr = color(255, 0, 0, 127);

		int impact_size = 10;

		unsigned int tracers{};
		int tracer_type{};

		color trace_clr[3] = {
			color(245, 66, 66),
			color(66, 164, 245),
			color(191, 66, 245)
		};

		bool	custom_bloom{};
		int		bloom_scale{};
		int		exposure_min{};
		int		exposure_max{};

		bool	custom_fog{};
		int		fog_start{};
		int		fog_end			= 9300;
		int		fog_density		= 70;

		char	skybox_name[128]{};
		int		skybox{};
		int		prop_alpha = 100;

		unsigned int world_material_options{};
		unsigned int world_modulation{};

		bool			retrack_peek{};

		color	autopeek_clr_back = color(184, 255, 97, 255);
		color	autopeek_clr = color(156, 200, 255, 255);

		color world_clr[4] {
			color(13, 13, 21),
			color(92, 98, 161),
			color(147, 135, 222),
			color(149, 149, 180)
		};

		unsigned int	hitmarker = 0;
		color	hitmarker_clr = color(255, 255, 255);

		bool			damage{};
		color	damage_clr = color(150, 113, 220);

		int		sound{};

		bool	unlock_inventory{};
		bool	snip_crosshair{};
		bool	preverse_killfeed{};
		bool	remove_ads{};
		bool	force_radar{};

		int		ragdoll_gravity{};

		bool	clantag{};
		bool	bypass_sv_pure{};
		bool	spoof_sv_cheats{};
		bool	unlock_hidden_cvars{};
		bool	pen_xhair{};

		unsigned int removals{};

		struct buy_bot_t {
			bool			enable{};
			int				main_weapon{};
			int				second_weapon{};
			unsigned int	other_items{};
		}buybot;

		unsigned int menu_indicators = 1 | 2 | 4;
		unsigned int animation_changes{};

		ImVec2 keybind_position{};
		ImVec2 bomb_position{};
		ImVec2 watermark_position{};

		color ui_color{ 150, 113, 220 };
	}misc;

	struct skinchanger_t {
		int group_type{};

		int model_glove{};
		int glove_skin{};

		int masks{};

		std::array<skin_weapon_t, weapon_cfg_max> skin_weapon{};
	}skins;

	std::array<key_bind_info_t, binds_max> binds;
};

extern configs_t g_cfg;