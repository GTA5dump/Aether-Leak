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
	class network_matchmaking_menu {
	public:
		void render();
		void update();
	};

	static network_matchmaking_menu* getNetworkMatchMakingMenu() {
		static network_matchmaking_menu instance;
		return &instance;
	}

	namespace network::matchmaking::vars {
		struct variables {
			bool m_toggle_player_magnet;
			bool m_open_extra_slot;
			int m_player_magnet_count;
		};

		extern variables m_vars;
	}
}