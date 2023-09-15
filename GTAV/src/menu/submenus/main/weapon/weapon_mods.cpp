#include "pch.h"
#include "weapon_mods.h"
#include "menu/util/globals.h"
#include "gui/options/option.h"
#include "gui/util/panels.h"
#include "rage/classes/CPed.h"
#include "rage/classes/CPlayerInfo.h"
#include "menu/util/control.h"
#include "menu/util/raycast.h"

#include "paint_gun.h"
#include "aimbot.h"
#include "weapon_disables.h"
#include "weapon_multipliers.h"
#include "explosions.h"
#include "entity_gun.h"
#include "particle_gun.h"
#include "gravity_gun.h"
#include "weapon_tracers.h"

using namespace base::gui;
using namespace menu::weapon::mods::vars;

namespace menu::weapon::mods::vars {
	variables m_vars;


	Vector3 get_direction(Vector3 rotation) {
		Vector3 tmp;
		tmp.y = rotation.z * 0.0174532924f;
		tmp.x = rotation.x * 0.0174532924f;
		tmp.z = abs(cos(tmp.x));

		return Vector3((-sin(tmp.y)) * tmp.z, (cos(tmp.y)) * tmp.z, sin(tmp.x));
	}

	bool isPedShooting(Ped ped) {
		Vector3 coords = ENTITY::GET_ENTITY_COORDS(ped, 1);
		return PED::IS_PED_SHOOTING_IN_AREA(ped, coords, coords, true, true);
	}
}



namespace menu {

	void weapon_mods_menu::render() {
		renderer::addSubmenu("Weapon Mods", "Weapon Mods", [](core* core) {
			core->addOption(submenuOption("Aimbot")
				.setTarget("Aimbot"));

			core->addOption(submenuOption("Disables")
				.setTarget("Weapon Disables"));

			core->addOption(submenuOption("Multipliers")
				.setTarget("Weapon Multipliers"));

			core->addOption(submenuOption("Tracers")
				.setTarget("Weapon Tracers"));

			core->addOption(submenuOption("Explosion Gun")
				.setTarget("Weapon Explosions"));

			core->addOption(submenuOption("Entity Gun")
				.setTarget("Weapon Entity Gun"));

			core->addOption(submenuOption("Particle Gun")
				.setTarget("Weapon Particle Gun"));

			core->addOption(submenuOption("Paint Gun")
				.setTarget("Paint Gun"));

			core->addOption(submenuOption("Gravity Gun")
				.setTarget("Weapon Gravity Gun"));

			core->addOption(toggleOption("Unlimited Ammo")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_unlimited_ammo));

			core->addOption(toggleOption("Instant Kill")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_instant_kill));

			core->addOption(toggleOption("Rapid Fire")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_rapid_fire));

			core->addOption(toggleOption("Explosive Bullets")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_explosive_bullets));

			core->addOption(toggleOption("Incendiary Bullets")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_incendiary_bullets));

			core->addOption(toggleOption("Teleport")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_teleport));

			core->addOption(toggleOption("Airstrike")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_airstrike));

			core->addOption(toggleOption("Defibrillator")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_defibrillator));

			core->addOption(toggleOption("Apply Force")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_apply_force));

			core->addOption(toggleOption("Delete Entity")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_delete_entity));

			core->addOption(toggleOption("Shrink Entity")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_shrink_entity));

			core->addOption(toggleOption("Spawn Money")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_spawn_money));

			core->addOption(toggleOption("Vehicle Hijack")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_vehicle_hijack));

			core->addOption(toggleOption("Clown VFX")
				.addHotkey().addTranslate()
				.addToggle(&m_vars.m_clown_vfx));
		});		
	}

	void weapon_mods_menu::update() {
		render();	
		raycast::raycastUpdate();

		getPaintGunMenu()->update();
		getAimBotMenu()->update();
		getWeaponDisablesMenu()->update();
		getWeaponMultipliersMenu()->update();
		getWeaponExplosionsMenu()->update();
		getEntityGunMenu()->update();
		getParticleGunMenu()->update();
		getGravityGunMenu()->update();
		getWeaponTracersMenu()->update();

		uint32_t m_weapon_hash = 0;
		WEAPON::GET_CURRENT_PED_WEAPON(PLAYER::PLAYER_PED_ID(), &m_weapon_hash, false);

		if (m_vars.m_unlimited_ammo) {
			WEAPON::SET_PED_INFINITE_AMMO_CLIP(PLAYER::PLAYER_PED_ID(), true);
		}

		if (m_vars.m_instant_kill) {
			PLAYER::SET_PLAYER_MELEE_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_PED_ID(), 99999.f, 1);
			PLAYER::SET_PLAYER_WEAPON_DAMAGE_MODIFIER(PLAYER::PLAYER_PED_ID(), 99999.f);
		}

		if (m_vars.m_rapid_fire) {
			if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
				if (PAD::IS_CONTROL_PRESSED(0, ControlAttack)) {
					Vector3 start = CAM::GET_GAMEPLAY_CAM_COORD() + (get_direction(CAM::GET_GAMEPLAY_CAM_ROT(0)));
					Vector3 end = CAM::GET_GAMEPLAY_CAM_COORD() + (get_direction(CAM::GET_GAMEPLAY_CAM_ROT(0)) * 200.f);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(start, end, WEAPON::GET_WEAPON_DAMAGE_TYPE(m_weapon_hash), true, m_weapon_hash, PLAYER::PLAYER_PED_ID(), true, false, 500.0f, PLAYER::PLAYER_PED_ID(), 0);
				}
			}
		}

		if (m_vars.m_explosive_bullets) {
			if (patterns::ped_factory) {
				if ((*patterns::ped_factory)->m_local_ped) {
					if ((*patterns::ped_factory)->m_local_ped->m_player_info) {
						(*patterns::ped_factory)->m_local_ped->m_player_info->m_frame_flags |= 0x800;
					}
				}
			}
		}

		if (m_vars.m_incendiary_bullets) {
			if (patterns::ped_factory) {
				if ((*patterns::ped_factory)->m_local_ped) {
					if ((*patterns::ped_factory)->m_local_ped->m_player_info) {
						(*patterns::ped_factory)->m_local_ped->m_player_info->m_frame_flags |= 0x1000;
					}
				}
			}
		}


		if (m_vars.m_teleport) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			if (isPedShooting(playerPed)) {
				auto BulletCoord = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(BulletCoord))
					ENTITY::SET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), BulletCoord, FALSE, TRUE, TRUE, FALSE);
			}
		}

		if (m_vars.m_airstrike) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			if (isPedShooting(playerPed)) {
				auto BulletCoord = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(BulletCoord)){
					control::requestWeapon(0x47757124);
					control::requestWeapon(0xf8a3939f);

					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY(BulletCoord, BulletCoord, WEAPON::GET_WEAPON_DAMAGE_TYPE(0x47757124), true, 0x47757124, PLAYER::PLAYER_PED_ID(), true, false, 500.0f, PLAYER::PLAYER_PED_ID(), 0);
					MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS_IGNORE_ENTITY({ BulletCoord.x, BulletCoord.y, BulletCoord.z + 60.0f }, BulletCoord, WEAPON::GET_WEAPON_DAMAGE_TYPE(0xf8a3939f), true, 0xf8a3939f,PLAYER::PLAYER_PED_ID(), true, false, 500.0f, PLAYER::PLAYER_PED_ID(), 0);
				}
			}
		}

		if (m_vars.m_apply_force) {
			Ped playerPed = PLAYER::PLAYER_PED_ID();
			if (isPedShooting(playerPed)) {
				auto BulletCoord = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(BulletCoord)) {
					if (control::request_control(raycast::m_ShapeTestEntity)) {
						Vector3 velocity = ((ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false) + (get_direction(CAM::GET_GAMEPLAY_CAM_ROT(0)) * 9999.f)) - ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false)) * 4.f;
						ENTITY::SET_ENTITY_VELOCITY(raycast::m_ShapeTestEntity, velocity);
					}

				}
			}
		}

		if (m_vars.m_delete_entity) {
			Entity entity;
			if (isPedShooting(PLAYER::PLAYER_PED_ID())) {
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_PED_ID(), &entity)) {
					if (ENTITY::IS_ENTITY_A_PED(entity)) {
						entity = PED::IS_PED_IN_ANY_VEHICLE(entity, true) ? PED::GET_VEHICLE_PED_IS_IN(entity, false) : entity;
					}

					if (ENTITY::DOES_ENTITY_EXIST(entity)) {
						if (control::request_control(entity)) {
							ENTITY::SET_ENTITY_AS_MISSION_ENTITY(entity, true, true);
							ENTITY::DELETE_ENTITY(&entity);
						}
					}
				}
			}
		}

		if (m_vars.m_spawn_money) {
			if (isPedShooting(PLAYER::PLAYER_PED_ID())) {
				auto bulletCoords = raycast::m_ShapeTestDestination;
				if (!raycast::IsEmpty(bulletCoords)) {
					control::requestModel(0x113fd533);
					OBJECT::CREATE_AMBIENT_PICKUP(0x1e9a99f8, bulletCoords, 1, 2500, 0x113fd533, 1, 1);
				}
			}
		}

		if (m_vars.m_vehicle_hijack) {
			Entity ent;
			if (PAD::IS_DISABLED_CONTROL_PRESSED(0, (int)eControllerInputs::INPUT_AIM)) {
				if (PAD::IS_DISABLED_CONTROL_JUST_RELEASED(0, (int)eControllerInputs::INPUT_ATTACK)) {
					if (raycast::raycast(&ent)) {
						if (ENTITY::IS_ENTITY_A_VEHICLE(ent)) {
							for (size_t i = 0; i < 8 && !VEHICLE::IS_VEHICLE_SEAT_FREE(ent, -1, 0); i++) {
								const auto ped = VEHICLE::GET_PED_IN_VEHICLE_SEAT(ent, -1, 0);
								TASK::CLEAR_PED_TASKS_IMMEDIATELY(ped);

								util::fiber::go_to_main();
							}
							PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), ent, -1);
						}
					}
				}
			}
		}

		if (m_vars.m_clown_vfx) {
			if (isPedShooting(PLAYER::PLAYER_PED_ID())) {
				STREAMING::REQUEST_NAMED_PTFX_ASSET("scr_rcbarry2");
				GRAPHICS::USE_PARTICLE_FX_ASSET("scr_rcbarry2");
				auto weaponIndexed = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(PLAYER::PLAYER_PED_ID(), 1);

				Vector3 v0, v1;
				MISC::GET_MODEL_DIMENSIONS(WEAPON::GET_SELECTED_PED_WEAPON(PLAYER::PLAYER_PED_ID()), &v0, &v1);
				GRAPHICS::START_NETWORKED_PARTICLE_FX_NON_LOOPED_ON_ENTITY("muz_clown", weaponIndexed, { (v0.x - v1.x) / 2.0f + 0.7f, 0.f, 0.f }, { 0.f, 180.f, 0.f }, 1.f, 1, 1, 1);
			}
		}

		if (m_vars.m_defibrillator) {
			if (m_weapon_hash == 0x3656c8c1) {
				Entity aimed;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &aimed)) {
					if (ENTITY::IS_ENTITY_A_PED(aimed) && ENTITY::IS_ENTITY_DEAD(aimed, 0)) {
						PED::RESURRECT_PED(aimed);
						PED::REVIVE_INJURED_PED(aimed);
						ENTITY::SET_ENTITY_HEALTH(aimed, 100, 0);
					}
				}
			}
		}

		if (m_vars.m_shrink_entity) {
			Entity aimed;
			if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &aimed)) {
				if (ENTITY::IS_ENTITY_A_PED(aimed)) {
					PED::SET_PED_CONFIG_FLAG(aimed, 223, true);
				}
			}
		}
	}
}