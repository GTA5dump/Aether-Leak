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
	class vehicle_parachute_menu {
	public:
		void render();
		void update();
	};

	static vehicle_parachute_menu* getVehicleParachuteMenu() {
		static vehicle_parachute_menu instance;
		return &instance;
	}


	namespace vehicle::parachute::vars {
		struct variables {
			bool m_auto_deploy;
		};

		extern variables m_vars;
	}
}