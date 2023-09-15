#include "pch.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"
#include "rage/engine.h"
#include "gui/util/notify.h"
#include "rage/classes/rlSessionByGamerTaskResult.h"
#include "rage/classes/rlTaskStatus.h"
#include "util/caller.h"
#include "rage/classes/CPlayerInfo.h"
#include "rage/classes/CPed.h"

#include "network_spoofing.h"
#include "spoofing/network_spoofing_host_token.h"
#include "spoofing/network_spoofing_name.h"

#include "rage/classes/CNetworkPlayerMgr.h"
#include "rage/classes/CNetGamePlayer.h"
#include "rage/classes/NetObject.h"


using namespace base::gui;
using namespace menu::network::spoofing::vars;

namespace menu::network::spoofing::vars {
	variables m_vars;

	const char* ping[] = {
		"Very Low", "Low", "Medium", "High", "Very High"
	}; int ping_id = 0;

	const char* face_direction[] = {
		"Left",
		"Right",
		"Back"
	};

	const char* static_type[] = {
		"Relative",
		"Static"
	};

	const char* axis[] = {
		"Pitch",
		"Roll",
		"Yaw"
	}; int axis_id = 0;
}


namespace menu {

	void network_spoofing_menu::render() {
		renderer::addSubmenu("Spoofing", "Network Spoofing", [](core* core) {
			core->addOption(submenuOption("Name")
				.setTarget("Spoofing Name"));

			core->addOption(submenuOption("Host Token")
				.setTarget("Spoofing Host Token"));

			core->addOption(submenuOption("Player Rotation")
				.setTarget("Spoofing Player Rotation"));

			core->addOption(submenuOption("Vehicle Rotation")
				.setTarget("Spoofing Vehicle Rotation"));

			core->addOption(toggleScrollOption<const char*, int>("Ping")
				.addToggle(&m_vars.m_toggle_ping)
				.addScroll(&ping).setPosition(&ping_id));
		});

		renderer::addSubmenu("Vehicle Rotation", "Spoofing Vehicle Rotation", [](core* core) {
			core->addOption(toggleNumberOption<float>("Spinbot")
				.addToggle(&m_vars.m_vehicle_spinbot)
				.addNumber(&m_vars.m_vehicle_spinbot_speed).addMin(0.f).addMax(100.f).setPrecision(0).addStep(1));

			core->addOption(scrollOption<const char*, int>("Spinbot Axis")
				.addScroll(&axis).setPosition(&axis_id));

			core->addOption(toggleScrollOption<const char*, int>("Jitter")
				.addToggle(&m_vars.m_vehicle_jitter)
				.addScroll(&axis).setPosition(&m_vars.m_vehicle_jitter_axis));

			core->addOption(toggleOption("Relative")
				.addToggle(&m_vars.m_vehicle_relative));

			core->addOption(toggleOption("Static")
				.addToggle(&m_vars.m_vehicle_static));

			if (m_vars.m_vehicle_static) {
				core->addOption(breakOption("Static"));

				core->addOption(toggleNumberOption<float>("Pitch")
					.addToggle(&m_vars.m_vehicle_static_toggle.x)
					.addNumber(&m_vars.m_vehicle_static_value.x).addMin(0.f).addMax(360.f).setPrecision(0).addStep(1));

				core->addOption(toggleNumberOption<float>("Roll")
					.addToggle(&m_vars.m_vehicle_static_toggle.y)
					.addNumber(&m_vars.m_vehicle_static_value.y).addMin(0.f).addMax(360.f).setPrecision(0).addStep(1));

				core->addOption(toggleNumberOption<float>("Yaw")
					.addToggle(&m_vars.m_vehicle_static_toggle.z)
					.addNumber(&m_vars.m_vehicle_static_value.z).addMin(0.f).addMax(360.f).setPrecision(0).addStep(1));
			}
		});

		renderer::addSubmenu("Player Rotation", "Spoofing Player Rotation", [](core* core) {
			core->addOption(toggleNumberOption<float>("Spinbot")
				.addToggle(&m_vars.m_ped_spinbot)
				.addNumber(&m_vars.m_ped_spinbot_speed).addMin(0.f).addMax(14.f).setPrecision(0).addStep(1)
				.addClick([] { m_vars.m_ped_face_direction = false; m_vars.m_ped_jitter = false; }));

			core->addOption(toggleScrollOption<const char*, int>("Face Direction")
				.addToggle(&m_vars.m_ped_face_direction)
				.addScroll(&face_direction).setPosition(&m_vars.m_ped_face_direction_val)
				.addClick([] { m_vars.m_ped_spinbot = false; m_vars.m_ped_jitter = false; }));

			core->addOption(toggleOption("Jitter")
				.addToggle(&m_vars.m_ped_jitter)
				.addClick([] { m_vars.m_ped_spinbot = false; m_vars.m_ped_face_direction = false; }));

			core->addOption(toggleOption("Relative")
				.addToggle(&m_vars.m_ped_relative));
		});

	}

	void network_spoofing_menu::update() {
		render();
		getNetworkSpoofingHostTokenMenu()->update();
		getNetworkSpoofingNameMenu()->update();

		if (m_vars.m_ped_spinbot) {
			static float value = 0.f;
			value += m_vars.m_ped_spinbot_speed;

			if (value > 180.f) value = -180.f + (value - 180.f);
			m_vars.m_ped_hook_value = { value * 0.0174532924f, value * 0.0174532924f };
			return;
		}

		if (m_vars.m_ped_jitter) {
			static float value = 0.f;
			value += 40.f;
			if (value > 180.f) value = -180.f + (value - 180.f);

			m_vars.m_ped_hook_value = { value * 0.0174532924f, value * 0.0174532924f };
		}

		if (PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), false)) {
			m_vars.m_vehicle_hook_value.x = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).x;
			m_vars.m_vehicle_hook_value.y = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).y;
			m_vars.m_vehicle_hook_value.z = ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).z;

			if (m_vars.m_vehicle_static) {
				if (m_vars.m_vehicle_static_toggle.x) m_vars.m_vehicle_hook_value.x = m_vars.m_vehicle_static_value.x + (m_vars.m_vehicle_relative ? ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).x : 0.f);
				if (m_vars.m_vehicle_static_toggle.y) m_vars.m_vehicle_hook_value.y = m_vars.m_vehicle_static_value.y + (m_vars.m_vehicle_relative ? ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).y : 0.f);
				if (m_vars.m_vehicle_static_toggle.z) m_vars.m_vehicle_hook_value.z = m_vars.m_vehicle_static_value.z + (m_vars.m_vehicle_relative ? ENTITY::GET_ENTITY_ROTATION(PLAYER::PLAYER_PED_ID(), 2).z : 0.f);
			}

			if (m_vars.m_vehicle_spinbot) {
				static float value = 0.f;
				value += m_vars.m_vehicle_spinbot_speed;
				if (value > 360.f) value = (value - 360.f);

				switch (m_vars.m_vehicle_spinbot_axis) {
				case 0:
					m_vars.m_vehicle_hook_value.x = value;
					break;

				case 1:
					m_vars.m_vehicle_hook_value.y = value;
					break;

				case 2:
					m_vars.m_vehicle_hook_value.z = value;
					break;
				}
			}

			if (m_vars.m_vehicle_jitter) {
				static float value = 0.f;
				value += MISC::GET_RANDOM_FLOAT_IN_RANGE(5.f, 100.f);
				if (value > 360.f) value = (value - 360.f);

				switch (m_vars.m_vehicle_jitter_axis) {
				case 0:
					m_vars.m_vehicle_hook_value.x = value;
					break;

				case 1:
					m_vars.m_vehicle_hook_value.y = value;
					break;

				case 2:
					m_vars.m_vehicle_hook_value.z = value;
					break;
				}
			}

			m_vars.m_vehicle_hook_value.x *= 0.0174532924f;
			m_vars.m_vehicle_hook_value.y *= 0.0174532924f;
			m_vars.m_vehicle_hook_value.z *= 0.0174532924f;
		}

		if (m_vars.m_toggle_ping) {
			for (int i = 0; i < (*patterns::network_player_mgr)->m_player_count; i++) {
				auto player = (*patterns::network_player_mgr)->m_player_list[i];
				if (*patterns::ped_factory && *patterns::network_object_mgr) {
					if ((*patterns::ped_factory)->m_local_ped && (*patterns::ped_factory)->m_local_ped->m_net_object && *patterns::network_object_mgr) {
						ping_id = (*patterns::network_object_mgr)->m_handler->m_sequences[player->m_player_id];
						(*patterns::network_object_mgr)->m_handler->m_sequences[player->m_player_id] = ping_id;

					}
				}

			}
		}
	}

}