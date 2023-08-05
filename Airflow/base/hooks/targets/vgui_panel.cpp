#include "../hooks.h"
#include "../../../includes.h"

#include "../../sdk.h"
#include "../../global_context.h"

#include "../../../base/tools/render.h"

#include "../../../functions/config_vars.h"
#include "../../../functions/features.h"
#include "../../../functions/menu/menu.h"

void update_buttons( ) {
	for ( int i = 0; i < 256; ++i ) {
		key_t* key = &g::keys[ i ];

		key->pressed = false;

		if ( key->down && key->tick > key->old_tick ) {
			key->old_tick = key->tick;
			key->pressed = true;
		}
	}
}

void proceed_binds( ) {
	for ( auto& b : g_cfg.binds ) {
		if ( b.key == -1 )
			continue;

		switch ( b.type ) {
			case 0:
			b.toggled = true;
			break;
			case 1:
			b.toggled = g::keys[ b.key ].down;
			break;
			case 2:
			if ( g::keys[ b.key ].pressed )
				b.toggled = !b.toggled;
			break;
		}
	}
}

namespace tr::panel {
	enum paneltypes_t {
		focus_overlay_panel = 0,
		mat_sys_top_panel,
		hud_zoom,
	};

	struct panels_t {
		std::string		name{};
		unsigned int	id{};

		panels_t() : name(""), id{ 0 } {}
		panels_t(const std::string& name) : name(name), id{ 0 } {}

		__forceinline bool is_valid(unsigned int id) {
			return this->id == id;
		}
	};

	std::vector<panels_t> panels = {
		panels_t{ xor_str("FocusOverlayPanel") },
		panels_t{ xor_str("MatSystemTopPanel") },
		panels_t{ xor_str("HudZoom") },
	};

	__forceinline panels_t& get_panel(int idx) {
		return panels[idx];
	}

	void __fastcall paint_traverse(c_panel* ecx, void* edx, unsigned int panel, bool a, bool b) {
		static auto original = vtables[vtables_t::panel].original<paint_traverse_fn>(xor_int(41));

		auto get_new_panel = [&](const char* panel_name) {
			unsigned int new_panel = 0;
			if (!std::strcmp(interfaces::panel->get_name(panel), panel_name))
				new_panel = panel;

			return new_panel;
		};

		// there is no sense to get panels every tick
		// it eats fps alot
		int panels_count = 0;
		for (auto& _panel : panels) {
			// all panels got, don't get anything here
			if (panels_count >= panels.size())
				break;

			// we already found a panel
			if (_panel.id > 0) {
				panels_count++;
				continue;
			}

			if (!_panel.id)
				_panel.id = get_new_panel(_panel.name.c_str());
		}

		if (get_panel(hud_zoom).is_valid(panel) && (g_cfg.misc.removals & scope))
			return;

		original(ecx, panel, a, b);

		if (get_panel(focus_overlay_panel).is_valid(panel)) {
			// ghetto fix
			// reupdate screen values when game was collapsed
			if (!g_render->screen_size.w || !g_render->screen_size.h)
				g_render->update_screen_size();

			// fixes some memes with d3dx render
			interfaces::surface->draw_set_color(color(0, 0, 0, 255));
			interfaces::surface->draw_filled_rect(0, 0, 1, 1);

			g_visuals_wrapper->on_paint_traverse();

			ecx->set_mouse_input_enabled(panel, g_ctx.uninject ? false : g_cfg.misc.menu);
		}

		/* yeah fps drops shit framework, i wont even spend time on this shit to fix it */
		update_buttons( );
		proceed_binds( );

		draw_menu( );
	}
}