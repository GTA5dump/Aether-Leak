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
	class session_scripts_menu {
	public:
		void render();
		void update();
	};

	static session_scripts_menu* getSessionScriptsMenu() {
		static session_scripts_menu instance;
		return &instance;
	}

	namespace network::session::scripts::vars {
		struct variables {
		};

		extern variables m_vars;
	}
}