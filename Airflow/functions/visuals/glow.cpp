#include "glow.h"
#include "../config_vars.h"

void c_glow_esp::on_post_screen_effects() {
	if (!g_ctx.in_game || !g_ctx.local)
		return;

	auto& enemy_esp = g_cfg.visuals.esp[esp_enemy];
	auto& weapon_esp = g_cfg.visuals.esp[esp_weapon];

	for (int i = 0; i < interfaces::glow_object_manager->glow_objects.count(); i++) {
		auto object = &interfaces::glow_object_manager->glow_objects[i];
		if (!object->entity || object->is_unused())
			continue;

		auto is_projectile = [&](int class_id) {
			switch (class_id) {
			case (int)CBaseCSGrenadeProjectile:
			case (int)CDecoyProjectile:
			case (int)CMolotovProjectile:
			case (int)CSensorGrenadeProjectile:
			case (int)CSmokeGrenadeProjectile:
			case (int)CInferno:
				return true;
				break;
			}
			return false;
		};

		auto draw_glow = [&](color& clr, bool ignore = false) {
			object->color = vector3d(clr[0] / 255.f,
				clr[1] / 255.f,
				clr[2] / 255.f );

			object->alpha = clr[3] / 255.f;

			if (ignore)
				object->occlued_render = true;

			object->unocclued_render = false;
		};

		if (enemy_esp.enable && (enemy_esp.elements & 256) && object->entity->is_player()) {
			c_csplayer* player = (c_csplayer*)object->entity;

			if (!player->is_alive() || player->dormant())
				continue;

			if (g_ctx.local->is_alive()) {
				if (g_ctx.local->team() == player->team())
					continue;

				if (g_ctx.local == player)
					continue;
			}
			else {
				auto spectator = g_ctx.local->get_observer_target();
				if (spectator) {
					if (player == spectator || player->team() == g_ctx.local->team())
						continue;
				}
				else continue;
			}

			draw_glow(enemy_esp.colors.glow, true);
		}

		auto class_id = object->entity->get_client_class()->class_id;
		if ((object->entity->is_weapon() || class_id == CC4 || class_id == CPlantedC4 || is_projectile(class_id))) {
			object->occlued_render = weapon_esp.enable && (weapon_esp.elements & 64);
			draw_glow(weapon_esp.colors.glow);
		}
	}
}
