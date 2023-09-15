#pragma once
#include "gui/options/core.h"
#include "gui/options/submenu.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"
#include "gui/options/break.h"
namespace menu {
	class network_players_menu {
	public:
		void render();
		void update();
	};

	static network_players_menu* getNetworkPlayersMenu() {
		static network_players_menu instance;
		return &instance;
	}

	namespace players::vars {
		struct variables {
			uint32_t m_selected_player;
		};

		extern variables m_vars;
	}
}