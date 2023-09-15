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
	class network_spoofing_menu {
	public:
		void render();
		void update();
	};

	static network_spoofing_menu* getNetworkSpoofingMenu() {
		static network_spoofing_menu instance;
		return &instance;
	}

	namespace network::spoofing::vars {
		struct variables {
			bool m_toggle_ping;
			bool m_ped_spinbot;
			bool m_ped_jitter;
			bool m_ped_face_direction;
			bool m_ped_relative;

			int m_ped_face_direction_val;
			float m_ped_spinbot_speed = 10.f;

			math::vector2<float> m_ped_hook_value;

			bool m_vehicle_spinbot;
			bool m_vehicle_jitter;
			bool m_vehicle_static;
			bool m_vehicle_relative;

			float m_vehicle_spinbot_speed = 10.f;

			int m_vehicle_spinbot_axis;
			int m_vehicle_jitter_axis;

			math::vector3_<float> m_vehicle_static_value;
			math::vector3_<bool> m_vehicle_static_toggle;
			math::vector3_<float> m_vehicle_hook_value;			
		};

		extern variables m_vars;
	}
}