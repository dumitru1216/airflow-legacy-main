#include "menu.h"

#include "../config_system.h"
#include "../config_vars.h"

#include "../../base/sdk.h"

#include "../../base/tools/render.h"

#include <ShlObj.h>
#include <algorithm>

menu_info_t menu_info = {};

const int array_cnt = 5;

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
	checkbox("enable rage", &g_cfg.rage.enable, 0);

}

void anti_hit_tab()
{
	
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

	sub_tab = std::clamp(sub_tab, 1, 5);

	std::string sub_tab_str = std::to_string(sub_tab) + " / 5";

	render::text(menu_info.pos_x + menu_info.size_x / 2, menu_info.pos_y + 15,
		color(), render::fonts::menu, centered_x, sub_tab_str);

	switch (sub_tab)
	{
	case 1:
	{
		checkbox( "enable esp", &g_cfg.visuals.esp[ esp_enemy ].enable, 1, true );

		bool draw_removals = multi_combo_box( "esp elements", &g_cfg.visuals.esp[esp_enemy ].elements, 2,
											  { "box", "name", "health", "weapon icon", "weapon name", "ammo bar", "flags", "skeleton" } );

		color_picker( "box color", &g_cfg.visuals.esp[ esp_enemy ].colors.box, 3, false, true );

	}
	break;
	case 2:
	{
		
	}
	break;
	case 3:
	{
		
	}
	break;
	case 4:
	{
		
	}
	break;
	case 5:
	{
		
	}
	break;
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


	}
	break;
	case 2:
	{
		
	}
	break;
	case 3:
	{
		
	}
	break;
	case 4:
	{
		
	}
	break;
	case 5:
	{
		
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