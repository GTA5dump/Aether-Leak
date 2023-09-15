#pragma once
#include "gui/options/core.h"
#include "gui/options/submenu.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"
#include "menu/util/players.h"

namespace menu {
	class network_selected_player_menu {
	public:
		void render();
		void update();
	};

	static network_selected_player_menu* getNetworkSelectedPlayerMenu() {
		static network_selected_player_menu instance;
		return &instance;
	}

	namespace players::selected::vars {
		struct variables {
			bool m_spectate;
		};

		extern variables m_vars;
	}
}