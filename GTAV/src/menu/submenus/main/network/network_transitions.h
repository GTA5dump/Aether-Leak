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
	class network_transitions_menu {
	public:
		void render();
		void update();
	};

	static network_transitions_menu* getNetworkTransitionsMenu() {
		static network_transitions_menu instance;
		return &instance;
	}

	namespace network::transitions::vars {
		struct variables {
			bool m_seamless_session_switch;
			bool m_skip_swoop_down;
			bool m_disable_dlc_intro_movie;
			bool m_transition_helper = true;
			bool m_show_transition_state;
		};

		extern variables m_vars;
	}
}