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
	class vehicle_mods_menu {
	public:
		void render();
		void update();
	};

	static vehicle_mods_menu* getVehicleModsMenu() {
		static vehicle_mods_menu instance;
		return &instance;
	}


	namespace vehicle::mods::vars {
		struct variables {
			bool m_instant_stop;
			bool m_instant_enter;
			bool bullet_proof;
			bool fire_proof;
			bool explosion_proof;
			bool collision_proof;
			bool melee_proof;
			bool drown_proof;
			bool m_toggle_chaff;
			bool m_toggle_flares;

			bool m_rainbow_prime;
			bool m_rainbow_second;
			bool m_scorched;
			bool m_rainbow_tyre;
		};

		extern variables m_vars;
	}
}