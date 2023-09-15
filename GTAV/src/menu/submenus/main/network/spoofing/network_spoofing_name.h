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
	class network_spoofing_name_menu {
	public:
		void render();
		void update();
	};

	static network_spoofing_name_menu* getNetworkSpoofingNameMenu() {
		static network_spoofing_name_menu instance;
		return &instance;
	}

	namespace network::spoofing::name::vars {
		struct variables {
			bool m_spoof;
			std::string m_name = "aether.lol";
			int m_color;
			int m_style;
			int m_icon;

		};

		extern variables m_vars;
	}
}