#include "pch.h"
#include "vehicle_mods.h"
#include "menu/util/globals.h"
#include "gui/options/option.h"
#include "gui/util/panels.h"
#include "rage/classes/CPed.h"
#include "rage/classes/CPlayerInfo.h"
#include "menu/util/control.h"
#include "menu/util/raycast.h"

#include "vehicle_health.h"
#include "vehicle_movement.h"
#include "vehicle_boost.h"
#include "vehicle_multipliers.h"
#include "vehicle_weapons.h"
#include "vehicle_particles.h"
#include "vehicle_collision.h"
#include "vehicle_gravity.h"
#include "vehicle_ramps.h"
#include "vehicle_tyre_tracks.h"
#include "vehicle_modifiers.h"

using namespace base::gui;
using namespace menu::vehicle::mods::vars;

namespace menu::vehicle::mods::vars {
	variables m_vars;
	int r = 255, g = 0, b = 0;
	void RGBFadee()
	{
		if (r > 0 && b == 0) {
			r--;
			g++;
		}
		if (g > 0 && r == 0) {
			g--;
			b++;
		}
		if (b > 0 && g == 0) {
			r++;
			b--;
		}
	}
}

namespace menu {

	void vehicle_mods_menu::render() {
		renderer::addSubmenu("Vehicle Mods", "Vehicle Mods", [](core* core) {
			core->addOption(submenuOption("Health")
				.setTarget("Vehicle Health"));

			core->addOption(submenuOption("Movement")
				.setTarget("Vehicle Movement"));

			core->addOption(submenuOption("Boost")
				.setTarget("Vehicle Boost"));

			core->addOption(submenuOption("Multipliers")
				.setTarget("Vehicle Multipliers"));

			core->addOption(submenuOption("Modifiers")
				.setTarget("Vehicle Modifiers"));

			core->addOption(submenuOption("Weapons")
				.setTarget("Vehicle Weapons"));

			core->addOption(submenuOption("Particles")
				.setTarget("Vehicle Particles"));

			core->addOption(submenuOption("Collision")
				.setTarget("Vehicle Collision"));

			core->addOption(submenuOption("Gravity")
				.setTarget("Vehicle Gravity"));

			core->addOption(submenuOption("Ramps")
				.setTarget("Vehicle Ramps"));

			core->addOption(submenuOption("Doors")
				.setTarget("Vehicle Doors"));

			core->addOption(submenuOption("Proofs")
				.setTarget("Vehicle Proofs"));

			core->addOption(submenuOption("Tyre Tracks")
				.setTarget("Vehicle Tyre Tracks"));

			core->addOption(toggleOption("Rainbow Primary Paint")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_rainbow_prime));

			core->addOption(toggleOption("Rainbow Primary Paint")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_rainbow_second));

			core->addOption(toggleOption("Rainbow Tyre Smoke")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_rainbow_tyre));

			core->addOption(toggleOption("Instant Stop")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_instant_stop));

			core->addOption(toggleOption("Instant Enter")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_instant_enter));

			core->addOption(toggleOption("Toggle Flares")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_toggle_flares));

			core->addOption(toggleOption("Toggle Chaff")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_toggle_chaff));

			core->addOption(buttonOption("Kick Driver")
				.addClick([] {
					if (!VEHICLE::IS_VEHICLE_SEAT_FREE(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), -1, 0)) {
						TASK::CLEAR_PED_TASKS_IMMEDIATELY(VEHICLE::GET_PED_IN_VEHICLE_SEAT(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), 0), -1, 0));
					}}));

			core->addOption(buttonOption("Delete")
				.addClick([] {
					Vehicle veh = PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false);
					ENTITY::DELETE_ENTITY(&veh);
				}));
		});

		renderer::addSubmenu("Vehicle Proofs", "Vehicle Proofs", [](core* core) {
			core->addOption(toggleOption("Bullet Proof")
				.addToggle(&m_vars.bullet_proof));

			core->addOption(toggleOption("Fire Proof")
				.addToggle(&m_vars.fire_proof));

			core->addOption(toggleOption("Explosion Proof")
				.addToggle(&m_vars.explosion_proof));

			core->addOption(toggleOption("Collision Proof")
				.addToggle(&m_vars.collision_proof));

			core->addOption(toggleOption("Melee Proof")
				.addToggle(&m_vars.melee_proof));

			core->addOption(toggleOption("Drown Proof")
				.addToggle(&m_vars.drown_proof));
				
		});

		renderer::addSubmenu("Vehicle Doors", "Vehicle Doors", [](core* core) {
			core->addOption(buttonOption("Open All Doors")
				.addClick([] {
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 0, true, false);
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 1, true, false);
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 2, true, false);
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 3, true, false);
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 4, true, false);
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 5, true, false);
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 6, true, false);
					VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 7, true, false);
					}));

			core->addOption(buttonOption("Close All Doors")
				.addClick([] { VEHICLE::SET_VEHICLE_DOORS_SHUT(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), true); }));

			core->addOption(buttonOption("Open Front Left Door")
				.addClick([] { VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 0, true, false); }));

			core->addOption(buttonOption("Open Front Right Door")
				.addClick([] { VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 1, true, false); }));

			core->addOption(buttonOption("Open Back Left Door")
				.addClick([] { VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 2, true, false); }));

			core->addOption(buttonOption("Open Back Right Door")
				.addClick([] { VEHICLE::SET_VEHICLE_DOOR_OPEN(PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID()), 3, true, false); }));
		});
	}

	void vehicle_mods_menu::update() {
		render();
		getVehicleHealthMenu()->update();
		getVehicleMovementMenu()->update();
		getVehicleBoostMenu()->update();
		getVehicleMultipliersMenu()->update();
		getVehicleWeaponsMenu()->update();
		getVehicleParticlesMenu()->update();
		getVehicleCollisionMenu()->update();
		getVehicleGravityMenu()->update();
		getVehicleRampsMenu()->update();
		getVehicleTyreTracksMenu()->update();
		getVehicleModifiersMenu()->update();

		ENTITY::SET_ENTITY_PROOFS(PLAYER::PLAYER_PED_ID(), m_vars.bullet_proof, m_vars.fire_proof, m_vars.explosion_proof, m_vars.collision_proof, m_vars.melee_proof, 0, 0, m_vars.drown_proof);

		if (m_vars.m_scorched) {
			ENTITY::SET_ENTITY_RENDER_SCORCHED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), true);
		}
		else {
			ENTITY::SET_ENTITY_RENDER_SCORCHED(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), false);
		}

		Ped playerPed = PLAYER::PLAYER_PED_ID();
		Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
		DWORD model = ENTITY::GET_ENTITY_MODEL(veh);


		if (m_vars.m_rainbow_prime) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				RGBFadee();
				VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(veh, r, g, b);

			}
		}
		if (m_vars.m_rainbow_second) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				RGBFadee();
				VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(veh, r, g, b);

			}
		}
		if (m_vars.m_rainbow_tyre) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				RGBFadee();
				VEHICLE::SET_VEHICLE_TYRE_SMOKE_COLOR(veh, r, g, b);

			}
		}

		if (m_vars.m_instant_stop) {
			if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
				ENTITY::FREEZE_ENTITY_POSITION(PED::GET_VEHICLE_PED_IS_IN(PLAYER::PLAYER_PED_ID(), false), Util::is_key_pressed(VK_SPACE) ? true : false);
			}
		}

		if (m_vars.m_instant_enter) {
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_ENTERING(PLAYER::PLAYER_PED_ID()), -1);
			PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), PED::GET_VEHICLE_PED_IS_TRYING_TO_ENTER(PLAYER::PLAYER_PED_ID()), -1);
		}
	}
}