#include "menu.h"

#include "../config_system.h"
#include "../config_vars.h"

#include "../../base/sdk.h"

#include "../../base/tools/render.h"

#include <ShlObj.h>
#include <algorithm>

menu_info_t menu_info = {};

const int array_cnt = 5;

void c_menu::create_animation( float& mod, bool cond, float speed_multiplier, unsigned int animation_flags ) {
	float time = ( g_ctx.animation_speed * 5.f ) * speed_multiplier;

	if ( ( animation_flags & skip_enable ) && cond )
		mod = 1.f;
	else if ( ( animation_flags & skip_disable ) && !cond )
		mod = 0.f;

	if ( animation_flags & lerp_animation )
		mod = std::lerp( mod, ( float )cond, time );
	else {
		if ( cond && mod <= 1.f )
			mod += time;
		else if ( !cond && mod >= 0.f )
			mod -= time;
	}

	mod = std::clamp( mod, 0.f, 1.f );
}

std::array<std::string, array_cnt> tabs =
{
	"ragebot",
	"anti-hit",
	"visuals",
	"extra",
	"configs"
};

int cursor_pos[2] = {};

bool item_hovered(int x, int y, int w, int h)
{
	return cursor_pos[0] > x && cursor_pos[1] > y
		&& cursor_pos[0] < x + w && cursor_pos[1] < y + h;
}

int selector = 0;
int chams_type[2] = {};

void ragebot_tab()
{
	static int sub_tab = 1;
	button( "<", 0, 0, [ ]( ) {
		sub_tab--;
	} );

	button( ">", 0, 1, [ ]( ) {
		sub_tab++;
	} );

	sub_tab = std::clamp( sub_tab, 1, 4 );

	std::string sub_tab_str = std::to_string( sub_tab ) + " / 4";

	render::text( menu_info.pos_x + menu_info.size_x / 2, menu_info.pos_y + 15,
				  color( ), render::fonts::menu, centered_x, sub_tab_str );

	if ( g_cfg.rage.group_type > 0 ) {
		auto name = xor_str( "enable config " ) + std::to_string( g_cfg.rage.group_type );
		checkbox( name.c_str( ), &g_cfg.rage.weapon[ g_cfg.rage.group_type ].enable, 2 );
	}

	switch ( sub_tab ) {
		case 1:
		{ // "auto snipers", "deagle", "pistols", "scout", "awp"
			checkbox( xor_c( "enable" ), &g_cfg.rage.enable, 3 );
			checkbox( xor_c( "auto fire" ), &g_cfg.rage.auto_fire, 4 );
			checkbox( xor_c( "delay untill unlag" ), &g_cfg.rage.delay_shot, 5 );
			key_bind( xor_c( "force body" ), g_cfg.binds[ force_body_b ], 6 );
			key_bind( xor_c( "override damage" ), g_cfg.binds[ override_dmg_b ], 7 );
			key_bind( xor_c( "double tap" ), g_cfg.binds[ dt_b ], 8 );
			key_bind( xor_c( "hide shots" ), g_cfg.binds[ hs_b ], 9 );

			key_bind( xor_c( "ping spike" ), g_cfg.binds[ spike_b ], 10 );
			slider( xor_c( "amount" ), &g_cfg.rage.spike_amt, 0, 1000, 11, "" );
			checkbox( xor_c( "compensate spike" ), &g_cfg.rage.adaptive_spike, 12 );
		} break;
		case 2:
		{
			auto& weapon_settings = g_cfg.rage.weapon[ g_cfg.rage.group_type ];

			if ( g_cfg.rage.group_type == global || g_cfg.rage.group_type == auto_snipers || g_cfg.rage.group_type == scout || g_cfg.rage.group_type == awp )
				checkbox( xor_c( "auto scope" ), &weapon_settings.auto_scope, 4 );

			slider( xor_c( "head scale" ), &weapon_settings.scale_head, 0, 100, 6, "" );
			slider( xor_c( "body scale" ), &weapon_settings.scale_body, 0, 100, 7, "" );

			slider( xor_c( "hitchance" ), &weapon_settings.hitchance, 0, 100, 8, "" );
			slider( xor_c( "min damage" ), &weapon_settings.mindamage, 1, 120, 9, "" );
			slider( xor_c( "min damage (override)" ), &weapon_settings.damage_override, 1, 120, 10, "" );

			bool draw_removals = multi_combo_box("hitboxes", &weapon_settings.hitboxes, 5,
												  { "head", "chest", "stomach" , "pelvis", "arms", "legs" } );

			checkbox( xor_c( "quick stop" ), &weapon_settings.quick_stop, 11 );

			multi_combo_box("options quick stop", &weapon_settings.quick_stop_options, 12,
							   {"early" , "between shots","force accuracy" } );

			combo_box( "target selection", &weapon_settings.target_selection, 3, { "lowest distance", "highest damage" } );

		} break;
	}
	combo_box( "weapon configuration", &g_cfg.rage.group_type, 1, { "general", "auto snipers", "deagle", "pistols", "scout", "awp" } );

}

void anti_hit_tab()
{
	static int sub_tab = 1;
	button( "<", 0, 0, [ ]( ) {
		sub_tab--;
	} );

	button( ">", 0, 1, [ ]( ) {
		sub_tab++;
	} );

	sub_tab = std::clamp( sub_tab, 1, 4 );

	std::string sub_tab_str = std::to_string( sub_tab ) + " / 4";

	render::text( menu_info.pos_x + menu_info.size_x / 2, menu_info.pos_y + 15,
				  color( ), render::fonts::menu, centered_x, sub_tab_str );

	switch ( sub_tab ) {
		case 1:
		{
			auto& cond = g_cfg.antihit.angles[ g_cfg.antihit.condition_type ];

			checkbox( "enable anti-hit", &g_cfg.antihit.enable, 1 );		
			checkbox( "at-targets", &cond.at_targets, 2 );


			if ( cond.yaw == 2 ) { // 4
				checkbox( "randomize speed", &cond.random_speed, 5 );
				slider( "range", &cond.distortion_range, -180, 180, 6, "" );
				if ( !cond.random_speed ) {
					slider( "speed", &cond.distortion_speed, 0, 100, 7, "" );
				}

				int new_index = 6;
				if ( !cond.random_speed ) {
					new_index = 6 + 1;
				} else {
					new_index = 6;
				}


				slider( "yaw add", &cond.yaw_add, -180, 180, new_index + 1, "" );
				slider( "jitter range", &cond.jitter_range, -180, 180, new_index + 2, "" );

				combo_box( "condition", &g_cfg.antihit.condition_type, new_index + 3, { "standing", "moving", "air" } );
				combo_box( "yaw jitter", &cond.jitter_mode, new_index + 4, { "disabled", "center", "offset", "random" } );


			} else {
				slider( "yaw add", &cond.yaw_add, -180, 180, 5, "" );

				slider( "jitter range", &cond.jitter_range, -180, 180, 6, "" );
				combo_box( "condition", &g_cfg.antihit.condition_type, 8, { "standing", "moving", "air" } );
				combo_box( "yaw jitter", &cond.jitter_mode, 7, { "disabled", "center", "offset", "random" } );

			}

		
			
			combo_box( "yaw", &cond.yaw, 4, { "disabled", "backwards", "distortion" } );
			combo_box( "pitch", &cond.pitch, 3, { "disabled", "down", "up" } );
		

		} break;
		case 2:
		{
			key_bind( "freestanding", g_cfg.binds[ edge_b ], 1 );
			key_bind( "left", g_cfg.binds[ left_b ], 2 );
			key_bind( "right", g_cfg.binds[ right_b ], 3 );
			key_bind( "back", g_cfg.binds[ back_b ], 4 );
			key_bind( "fake-walk", g_cfg.binds[ sw_b ], 5 );

		} break;
		case 3:
		{
			checkbox( "enable fakelag", &g_cfg.antihit.fakelag, 1 );

			slider( "limit", &g_cfg.antihit.fakelag_limit, 0, 14, 3, "" );

			bool draw_removals = multi_combo_box( "fakelag conditions", &g_cfg.antihit.fakelag_conditions, 2,
												  { "standing", "moving", "air", "land"} );

		} break;
		case 4:
		{
			checkbox( "enable fakeangle", &g_cfg.antihit.desync, 1 );
			slider( "lby limit", &g_cfg.antihit.desync_range, -180, 180, 3, "" );
			combo_box( "lby sway mode", &g_cfg.antihit.desync_type, 2, { "default", "double" } );
		} break;
	}
}

void visuals_tab()
{
	static int sub_tab = 1;
	button("<", 0, 0, []()
		{
			sub_tab--;
		});

	button(">", 0, 1, []()
		{
			sub_tab++;
		});

	sub_tab = std::clamp(sub_tab, 1, 6);

	std::string sub_tab_str = std::to_string(sub_tab) + " / 6";

	render::text(menu_info.pos_x + menu_info.size_x / 2, menu_info.pos_y + 15,
		color(), render::fonts::menu, centered_x, sub_tab_str);

	switch (sub_tab)
	{
	case 1:
	{
		checkbox( "enable player", &g_cfg.visuals.esp[ esp_enemy ].enable, 1 );

		color_picker( "[p] box", &g_cfg.visuals.esp[ esp_enemy ].colors.box, 3, true );
		color_picker( "[p] name", &g_cfg.visuals.esp[ esp_enemy ].colors.name, 4, true );
		color_picker( "[p] health", &g_cfg.visuals.esp[ esp_enemy ].colors.health, 5, true );
		color_picker( "[p] weapon", &g_cfg.visuals.esp[ esp_enemy ].colors.weapon, 6, true );
		color_picker( "[p] weapon", &g_cfg.visuals.esp[ esp_enemy ].colors.ammo_bar, 7, true );
		color_picker( "[p] oof arrow", &g_cfg.visuals.esp[ esp_enemy ].colors.offscreen_arrow, 8, true );
		color_picker( "[p] glow", &g_cfg.visuals.esp[ esp_enemy ].colors.glow, 9, true );
		color_picker( "[p] skeleton", &g_cfg.visuals.esp[ esp_enemy ].colors.skeleton, 10, true );

		bool draw_removals = multi_combo_box( "esp elements", &g_cfg.visuals.esp[ esp_enemy ].elements, 2,
											  { "box", "name", "health", "weapon icon", "weapon name", "ammo bar", "flags", "out of fov arrow", "glow", "skeleton" } );

	}
	break;
	case 2:
	{
		static int chams_type;

		checkbox( "enable chams", &g_cfg.visuals.chams[ chams_type ].enable, 2 );
		color_picker( "material color", &g_cfg.visuals.chams[ chams_type ].main_color, 4, true );

		if ( g_cfg.visuals.chams[ chams_type ].material == 4 ) {
			color_picker( "glow color", &g_cfg.visuals.chams[ chams_type ].glow_color, 5, true );
			slider( "fill", &g_cfg.visuals.chams[ chams_type ].glow_fill, 0, 100, 6, "" );
		}

		if ( chams_type == 3 ) {
			slider( "shot duration", &g_cfg.visuals.chams[ chams_type ].shot_duration, 0, 10, 7, "%" );
		}
		else if ( chams_type == 4 ) {
			checkbox( "blend in scope", &g_cfg.misc.blend_scope, 8 );
			slider( "blend amount", &g_cfg.misc.scope_amt, 0, 100, 9, "" );
		}

		combo_box( "enemy chams", &g_cfg.visuals.chams[ chams_type ].material, 3,
				   { "textured", "shaded", "flat", "metalic", "glow", "bubble", "money" } );

		combo_box( "model", &chams_type, 1,
				   { "visible", "invisible", "history", "on-shot", "rodgolls", "local", "viewmodel", "weapon",
				   "attachments", "fake" } );
	}
	break;
	case 3:
	{
		checkbox( "penetration crosshair", &g_cfg.misc.pen_xhair, 1 );
		checkbox( "force crosshair", &g_cfg.misc.snip_crosshair, 2 );
		checkbox( "fix zoom sensivity", &g_cfg.misc.fix_sensitivity, 3 );

		slider( "viewmodel fov", &g_cfg.misc.fovs[arms], 0, 30, 4, "" );
		checkbox( "show viewmodel in scope", &g_cfg.misc.viewmodel_scope, 5 );

		slider( "offset x", &g_cfg.misc.viewmodel_pos[ 0 ], -20, 20, 6, "" );
		slider( "offset y", &g_cfg.misc.viewmodel_pos[ 1 ], -20, 20, 7, "" );
		slider( "offset z", &g_cfg.misc.viewmodel_pos[ 2 ], -20, 20, 8, "" );

	}
	break;
	case 4:
	{
		checkbox( "enable weapon", &g_cfg.visuals.esp[esp_weapon ].enable, 1 );

		checkbox( "projectile prediction", &g_cfg.visuals.grenade_predict, 3 );
		checkbox( "projectile warning", &g_cfg.visuals.grenade_warning, 4 );
		checkbox( "warning line", &g_cfg.visuals.grenade_warning_line, 5 );
		color_picker( "warning color", &g_cfg.visuals.warning_clr, 6, true );
		color_picker( "[w] box", &g_cfg.visuals.esp[ esp_weapon ].colors.box, 7, true );
		color_picker( "[w] name", &g_cfg.visuals.esp[ esp_weapon ].colors.name, 8, true );
		color_picker( "[w] ammo", &g_cfg.visuals.esp[ esp_weapon ].colors.ammo_bar, 9, true );
		color_picker( "[w] fire range", &g_cfg.visuals.esp[ esp_weapon ].colors.molotov_range, 10, true );
		color_picker( "[w] smoke range", &g_cfg.visuals.esp[ esp_weapon ].colors.smoke_range, 11, true );
		color_picker( "[w] glow", &g_cfg.visuals.esp[ esp_weapon ].colors.glow, 12, true );

		bool draw_removals = multi_combo_box( "weapon elements", &g_cfg.visuals.esp[ esp_weapon ].elements, 2,
											  { "box", "name icon", " name", "ammo bar", "fire range", "smoke range", "glow" } );

	}
	break;
	case 5:
	{
		std::vector < const char* > skyboxes = 
		{
			"default", "tibet", "bagage", "italy", "jungle", "office", "daylight 1", "daylight 2",
				"vertigo blue", "vertigo", "day", "nuke blank", "venice", "daylight 3", "daylight 4",
				"cloudy", "night", "nightb", "night fat", "dust", "vietnam", "embassy", "custom"
		};

		
		slider( "aspect ratio", &g_cfg.misc.aspect_ratio, 0, 200, 2, "%" );
		slider( "fov", &g_cfg.misc.fovs[ world ], 0, 30, 3, "" );
		slider( "zoom fov", &g_cfg.misc.fovs[ zoom ], 0, 100, 4, "" );

		color_picker( "world color", &g_cfg.misc.world_clr[ world ], 8, false );
		color_picker( "props color", &g_cfg.misc.world_clr[ props ], 9, true );
		color_picker( "skybox color", &g_cfg.misc.world_clr[ sky ], 10, false );

		bool world_modu = multi_combo_box( "world modulation", &g_cfg.misc.world_modulation, 7,
												{ "nightmode", "sunset", "fullbright" } );


		bool world_materials = multi_combo_box( "world materials", &g_cfg.misc.world_material_options, 6,
											  { "texture reflect", "force metalic", "money mode" } );


		combo_box( "sky changer", &g_cfg.misc.skybox, 5, skyboxes );
		if ( g_cfg.misc.skybox == 22 ) {
			// make a listbox
		}

		bool draw_removals = multi_combo_box( "removals", &g_cfg.misc.removals, 1,
											  { "scope overlay", "visual recoil", "post processing", "smoke effect", "flashbang effect",
											  "fog effect", "shadows", "viewmodel bob", "landing bob" } );
	
	}
	break;
	case 6: {
		checkbox( "custom bloom", &g_cfg.misc.custom_bloom, 1 );
		slider( "bloom scale", &g_cfg.misc.bloom_scale, 0, 5, 2, "" );
		slider( "exposure min", &g_cfg.misc.exposure_min, 0, 10, 3, "" );
		slider( "exposure max", &g_cfg.misc.exposure_max, 0, 10, 4, "" );
		checkbox( "custom fog", &g_cfg.misc.custom_fog, 5 );
		slider( "fog start", &g_cfg.misc.fog_start, 0, 10000, 6, "" );
		slider( "fog end", &g_cfg.misc.fog_end, 0, 10000, 7, "" );
		slider( "fog density", &g_cfg.misc.fog_density, 0, 100, 8, "" );
		color_picker( "fog color", &g_cfg.misc.world_clr[ fog ], 9, true );
	} break;
	}
}

void misc_tab()
{
	static int sub_tab = 1;
	button("<", 0, 0, []()
		{
			sub_tab--;
		});

	button(">", 0, 1, []()
		{
			sub_tab++;
		});

	sub_tab = std::clamp(sub_tab, 1, 5);

	std::string sub_tab_str = std::to_string(sub_tab) + " / 5";

	render::text(menu_info.pos_x + menu_info.size_x / 2, menu_info.pos_y + 15,
		color(), render::fonts::menu, centered_x, sub_tab_str);

	switch (sub_tab)
	{
	case 1:
	{
		checkbox( "auto jump", &g_cfg.misc.auto_jump, 1 );
		checkbox( "auto strafe", &g_cfg.misc.auto_strafe, 2 );
		slider( "strafe smooth", &g_cfg.misc.strafe_smooth, 0, 100, 3, "" );
		checkbox( "fast stop", &g_cfg.misc.fast_stop, 4 );
		checkbox( "slide walk", &g_cfg.misc.slide_walk, 5 );

		key_bind( "auto peek", g_cfg.binds[ ap_b ], 6 );
		color_picker( "[a] start", &g_cfg.misc.autopeek_clr, 7, true );
		color_picker( "[a] end", &g_cfg.misc.autopeek_clr_back, 8, true );
		checkbox( "return on key release", &g_cfg.misc.retrack_peek, 9 );

		key_bind( "thirdperson key", g_cfg.binds[ tp_b ], 10 );
		slider( "third person distance", &g_cfg.misc.thirdperson_dist, 50, 300, 11, "ft" );
		checkbox( "while dead", &g_cfg.misc.thirdperson_dead, 12 );

	}
	break;
	case 2:
	{
		checkbox( "bullet impacts", &g_cfg.misc.impacts, 1 );
		color_picker( "[i] server", &g_cfg.misc.server_clr, 2, true );
		color_picker( "[i] client", &g_cfg.misc.client_clr, 3, true );
		slider( "impacts size", &g_cfg.misc.impact_size, 5, 15, 4, "" );


		color_picker( "[b] enemy", &g_cfg.misc.trace_clr[0], 6, true );
		color_picker( "[b] team", &g_cfg.misc.trace_clr[2], 7, true );
		color_picker( "[b] local", &g_cfg.misc.trace_clr[1], 8, true );

		bool world_modu = multi_combo_box( "bullet beams", &g_cfg.misc.tracers, 5,
										   { "enemy", "team", "local" } );

		combo_box( "tracer type", &g_cfg.misc.tracer_type, 9, { "beam", "line" } );


	}
	break;
	case 3:
	{
		checkbox( "event log", &g_cfg.visuals.eventlog.enable, 1 );
		checkbox( "draw debug logs", &g_cfg.visuals.eventlog.render_debug_log, 9 );
		checkbox( "filter console", &g_cfg.visuals.eventlog.filter_console, 2 );
		checkbox( "damage indicator", &g_cfg.misc.damage, 7 );

		color_picker( "[m] hitmarker", &g_cfg.misc.hitmarker_clr, 5, true );
		color_picker( "[m] damage indicator", &g_cfg.misc.damage_clr, 8, true );

		combo_box( "hitsound", &g_cfg.misc.sound, 6, { "disabled", "metallic", "tap" } );

		bool world_modu2 = multi_combo_box( "hitmarker", &g_cfg.misc.hitmarker, 4,
										   { "on enemy", "on screen" } );

		bool world_modu = multi_combo_box( "event type", &g_cfg.visuals.eventlog.logs, 3,
										   { "hits", "misses", "debug", "purchases", "bomb plant" } );
	}
	break;
	case 4:
	{
		checkbox( "buy bot", &g_cfg.misc.buybot.enable, 1 );

		checkbox( "force radar", &g_cfg.misc.force_radar, 5 );
		checkbox( "preserve killfeed", &g_cfg.misc.preverse_killfeed, 6 );
		checkbox( "unlock inventory", &g_cfg.misc.unlock_inventory, 7 );
		checkbox( "remove server ads", &g_cfg.misc.remove_ads, 8 );

		checkbox( "spoof sv-cheats", &g_cfg.misc.spoof_sv_cheats, 9 );
		checkbox( "unlock hidden cvars", &g_cfg.misc.unlock_hidden_cvars, 10 );
		checkbox( "bybass sv-pure", &g_cfg.misc.bypass_sv_pure, 11 );
		checkbox( "clantag", &g_cfg.misc.clantag, 12 );

		bool world_modu2 = multi_combo_box( "items", &g_cfg.misc.buybot.other_items, 4,
											{ "head armor", "armor", "hegrenade", "molotov", "smoke", "taser", "defuse kit" } );

		combo_box( "secondary", &g_cfg.misc.buybot.second_weapon, 3, { "duals", "p250", "cz75", "deagle" } );
		combo_box( "primary", &g_cfg.misc.buybot.main_weapon, 2, { "auto", "scout", "awp", "negev", "m249", "ak47", "aug" } );
	}
	break;
	case 5:
	{
		combo_box( "rodgoll gravity", &g_cfg.misc.ragdoll_gravity, 1, { "none", "away", "fly up"} );
		bool world_modu2 = multi_combo_box( "animation changers", &g_cfg.misc.animation_changes, 2,
											{ "zero pitch on land", "break leg movement", "movelean" } );
	}
	break;
	}
}

int cfg_index = -1;

std::vector<std::string> config_names = { "main", "additional", "misc", "custom 1", "custom 2" };

void configs_tab()
{
	bool draw_combo = combo_box("config slot", &cfg_index, 0, { "main", "additional", "misc", "custom 1", "custom 2" });
	button("save", 1, 0, []()
		{
			if (cfg_index != -1)
				config::save(config_names[cfg_index]);
		});

	button("load", 1, 1, []()
		{
			if (cfg_index != -1)
				config::load(config_names[cfg_index]);
		}, !draw_combo);

	button("uninject", 2, 2, []()
		{
			g_ctx.uninject = true;
		}, !draw_combo);
}

void draw_tabs()
{
	switch (selector)
	{
	case 0:
		ragebot_tab();
		break;
	case 1:
		anti_hit_tab();
		break;
	case 2:
		visuals_tab();
		break;
	case 3:
		misc_tab();
		break;
	case 4:
		configs_tab();
		break;
	}
}

void draw_cursor()
{
	const int size = 15;
	int x = cursor_pos[0];
	int y = cursor_pos[1];

	if (g_cfg.misc.menu)
	{
		render::triangle_filled(vector2d(x, y),
			vector2d(x + size + 1, y + size),
			vector2d(x, y + size),
			color(255, 133, 139));

		render::triangle_filled(vector2d(x, y + size),
			vector2d(x + size / 2, y + size),
			vector2d(x, y + size + 8),
			color(255, 133, 139));

		SetCursor(NULL);
	}
}

void draw_menu()
{
	if (!g_cfg.misc.menu)
		return;

	interfaces::surface->surface_get_cursor_pos(cursor_pos[0], cursor_pos[1]);

	int center_x = render::screen.w / 2;
	int center_y = render::screen.h / 2;

	// window header
	render::rect_filled(center_x - 168, center_y - 165, 337, 20, color(46, 46, 46));
	render::rect(center_x - 168, center_y - 165, 337, 20, color(66, 66, 66));
	render::text(center_x, center_y - 163, color(), render::fonts::menu, centered_x, "superfly");

	// main window
	render::rect_filled(center_x - 168, center_y - 145, 337, 330, color(36, 36, 36));
	render::rect(center_x - 168, center_y - 145, 337, 330, color(46, 46, 46));

	// tabs
	menu_info.write_size(337, 330);
	menu_info.write_pos(center_x - 168, center_y - 125);

	for (int i = 0; i < array_cnt; i++)
	{
		auto w = render::text_width(render::fonts::menu, tabs[i].c_str());

		int tab_size = 336 / array_cnt;
		int button_pos = (center_x - 168) + tab_size * i;

		bool hovered = item_hovered(button_pos, center_y - 144, tab_size, 20);

		if (hovered && g::keys[VK_LBUTTON].pressed)
			selector = i;

		const color clr = hovered ? color(60, 60, 60) : color(50, 50, 50);
		const color text_clr = selector == i ? color(255, 133, 139) : color();

		render::rect_filled(button_pos + 1, center_y - 144, tab_size, 20, clr);
		render::rect(button_pos + 1, center_y - 144, tab_size, 20, color(70, 70, 70));
		render::text(button_pos + tab_size / 2, center_y - 141, text_clr, render::fonts::menu, centered_x, tabs[i]);
	}

	draw_tabs();
}