#include "pch.h"
#include "vehicle_parachute.h"
#include "menu/util/globals.h"
#include "gui/options/option.h"
#include "gui/util/panels.h"
#include "rage/classes/CPed.h"
#include "rage/classes/CPlayerInfo.h"
#include "menu/util/control.h"
#include "menu/util/raycast.h"
#include "../../vehicle.h"

using namespace base::gui;
using namespace menu::vehicle::parachute::vars;

namespace menu::vehicle::parachute::vars {
	variables m_vars;

	void deploy() {
		Entity vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
			if (ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(vehicle) > 5.f) {
				if (VEHICLE::IS_THIS_MODEL_A_HELI(ENTITY::GET_ENTITY_MODEL(vehicle))) return;

				vehicle::vars::run_task(vehicle, [=](Vehicle veh) {
					uint64_t address = rage::engine::get_entity_address(veh);
					if (address) {
						uint64_t model_info = *(uint64_t*)(address + 0x20);
						if (model_info) {
							*(uint8_t*)(model_info + 0x58C) |= 1;
						}

						*(bool*)(address + 0x1A5D) = true;
						VEHICLE::VEHICLE_START_PARACHUTING(veh, true);
					}
					});
			}
		}
	}
}



namespace menu {

	void vehicle_parachute_menu::render() {
		renderer::addSubmenu("Vehicle Parachute", "Vehicle Parachute", [](core* core) {
			const auto vehicle = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);

			core->addOption(buttonOption("Deploy")
				.addTranslate().addHotkey()
				.addClick(deploy));

			core->addOption(toggleOption("Auto Deploy")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_auto_deploy));
		});
	}

	void vehicle_parachute_menu::update() {
		render();

		if (m_vars.m_auto_deploy) {
			deploy();
		}
	}
}