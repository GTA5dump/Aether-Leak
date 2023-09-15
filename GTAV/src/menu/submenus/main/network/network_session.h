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

#include "rage/classes/SessionInfo.h"
namespace menu {
	class network_session_menu {
	public:
		void render();
		void update();
	};

	static network_session_menu* getNetworkSessionMenu() {
		static network_session_menu instance;
		return &instance;
	}

	namespace network::session::vars {
		struct variables {
			bool join_queued = false;
			rage::rlSessionInfo info;
			uint64_t rid_to_join;
			bool host_token_correction;
			bool peer_token_correction;

			bool m_force_session_host;
		};

		extern variables m_vars;
	}
}