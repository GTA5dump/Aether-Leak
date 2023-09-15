#pragma once
#include "gui/options/core.h"
#include "gui/options/submenu.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"

namespace menu {
	class network_menu {
	public:
		void render();
		void update();
	};

	static network_menu* getNetworkMenu() {
		static network_menu instance;
		return &instance;
	}

	namespace network::vars {
		struct variables {
			bool m_bst;
			bool m_toggle_ping;
		};

		extern variables m_vars;
	}

}