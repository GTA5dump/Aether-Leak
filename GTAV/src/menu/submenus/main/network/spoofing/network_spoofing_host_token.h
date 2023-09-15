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
	class network_spoofing_host_token_menu {
	public:
		void render();
		void update();
	};

	static network_spoofing_host_token_menu* getNetworkSpoofingHostTokenMenu() {
		static network_spoofing_host_token_menu instance;
		return &instance;
	}

	namespace network::spoofing::host_token::vars {
		struct variables {
			bool m_spoof;
			uint64_t host_token = 0;

		};

		extern variables m_vars;
	}
}