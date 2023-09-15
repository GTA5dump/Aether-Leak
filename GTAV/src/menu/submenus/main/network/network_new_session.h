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
	class network_new_session_menu {
	public:
		void render();
		void update();
	};

	static network_new_session_menu* getNetworkNewSessionMenu() {
		static network_new_session_menu instance;
		return &instance;
	}

	namespace network::new_session::vars {
		struct variables {
		};

		extern variables m_vars;
	}
}