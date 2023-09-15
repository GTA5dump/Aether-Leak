#include "pch.h"
#include "protections.h"
#include "protections/protection_network_events.h"
#include "protections/protection_script_events.h"
#include "protections/protection_reports.h"
using namespace base::gui;
using namespace menu::protections::vars;

namespace menu::protections::vars {
	variables m_vars;
	
	void enable_all() {
		m_vars.m_fake_lag = true;
		m_vars.m_force_entities_visible = true;
		m_vars.m_block_all_script_events = true;
		network_events::vars::m_vars.block_network_crashes_id = 2;
		network_events::vars::m_vars.block_network_kicks_id = 2;
		network_events::vars::m_vars.block_desync_id = 2;
		network_events::vars::m_vars.block_explosion_id = 2;
		network_events::vars::m_vars.block_ptfx_id = 2;
		network_events::vars::m_vars.block_kick_vote_id = 2;
		network_events::vars::m_vars.block_clear_tasks_id = 2;
		network_events::vars::m_vars.block_ragdoll_id = 2;
		network_events::vars::m_vars.block_request_control_id = 2;
		network_events::vars::m_vars.block_remove_weapons_id = 2;
		network_events::vars::m_vars.block_give_weapons_id = 2;
		network_events::vars::m_vars.block_sound_spam_id = 2;
		script_events::vars::m_vars.block_script_kicks_id = 2;
		script_events::vars::m_vars.block_script_crashes_id = 2;
		script_events::vars::m_vars.block_bounty_id = 2;
		script_events::vars::m_vars.block_ceo_kick_id = 2;
		script_events::vars::m_vars.block_ceo_ban_id = 2;
		script_events::vars::m_vars.block_ceo_money_id = 2;
		script_events::vars::m_vars.block_clear_wanted_level_id = 2;
		script_events::vars::m_vars.block_force_to_mission_id = 2;
		script_events::vars::m_vars.block_give_collectible_id = 2;
		script_events::vars::m_vars.block_gta_banner_id = 2;
		script_events::vars::m_vars.block_network_bail_id = 2;
		script_events::vars::m_vars.block_insurance_message_id = 2;
		script_events::vars::m_vars.block_remote_off_radar_id = 2;
		script_events::vars::m_vars.block_force_to_cutscene_id = 2;
		script_events::vars::m_vars.block_force_to_cayo_id = 2;
		script_events::vars::m_vars.block_force_teleport_id = 2;
		script_events::vars::m_vars.block_script_sound_spam_id = 2;
		script_events::vars::m_vars.block_spectate_message_id = 2;
		script_events::vars::m_vars.block_transaction_error_id = 2;
		script_events::vars::m_vars.block_vehicle_kick_id = 2;
		script_events::vars::m_vars.block_force_to_activity_id = 2;
		script_events::vars::m_vars.block_kick_from_interior_id = 2;
		script_events::vars::m_vars.block_notifications_id = 2;
		script_events::vars::m_vars.block_business_raids_id = 2;
		script_events::vars::m_vars.block_rotate_cam_id = 2;
		reports::vars::m_vars.block_arxan_reports_id = 2;
		reports::vars::m_vars.block_reports_id = 2;
	}

	void disable_all() {
		m_vars.m_fake_lag = false;
		m_vars.m_force_entities_visible = false;
		m_vars.m_block_all_script_events = false;
		network_events::vars::m_vars.block_network_crashes_id = 0;
		network_events::vars::m_vars.block_network_kicks_id = 0;
		network_events::vars::m_vars.block_desync_id = 0;
		network_events::vars::m_vars.block_explosion_id = 0;
		network_events::vars::m_vars.block_ptfx_id = 0;
		network_events::vars::m_vars.block_kick_vote_id = 0;
		network_events::vars::m_vars.block_clear_tasks_id = 0;
		network_events::vars::m_vars.block_ragdoll_id = 0;
		network_events::vars::m_vars.block_request_control_id = 0;
		network_events::vars::m_vars.block_remove_weapons_id = 0;
		network_events::vars::m_vars.block_give_weapons_id = 0;
		network_events::vars::m_vars.block_sound_spam_id = 0;
		script_events::vars::m_vars.block_script_kicks_id = 0;
		script_events::vars::m_vars.block_script_crashes_id = 0;
		script_events::vars::m_vars.block_bounty_id = 0;
		script_events::vars::m_vars.block_ceo_kick_id = 0;
		script_events::vars::m_vars.block_ceo_ban_id = 0;
		script_events::vars::m_vars.block_ceo_money_id = 0;
		script_events::vars::m_vars.block_clear_wanted_level_id = 0;
		script_events::vars::m_vars.block_force_to_mission_id = 0;
		script_events::vars::m_vars.block_give_collectible_id = 0;
		script_events::vars::m_vars.block_gta_banner_id = 0;
		script_events::vars::m_vars.block_network_bail_id = 0;
		script_events::vars::m_vars.block_insurance_message_id = 0;
		script_events::vars::m_vars.block_remote_off_radar_id = 0;
		script_events::vars::m_vars.block_force_to_cutscene_id = 0;
		script_events::vars::m_vars.block_force_to_cayo_id = 0;
		script_events::vars::m_vars.block_force_teleport_id = 0;
		script_events::vars::m_vars.block_script_sound_spam_id = 0;
		script_events::vars::m_vars.block_spectate_message_id = 0;
		script_events::vars::m_vars.block_transaction_error_id = 0;
		script_events::vars::m_vars.block_vehicle_kick_id = 0;
		script_events::vars::m_vars.block_force_to_activity_id = 0;
		script_events::vars::m_vars.block_kick_from_interior_id = 0;
		script_events::vars::m_vars.block_notifications_id = 0;
		script_events::vars::m_vars.block_business_raids_id = 0;
		script_events::vars::m_vars.block_rotate_cam_id = 0;
		reports::vars::m_vars.block_arxan_reports_id = 0;
		reports::vars::m_vars.block_reports_id = 0;
	}

}

namespace menu {

	void protections_menu::render() {
		renderer::addSubmenu("Protections", "Protections", [](core* core) {
			core->addOption(submenuOption("Network Events")
				.setTarget("Protections Network Events"));

			core->addOption(submenuOption("Script Events")
				.setTarget("Protections Script Events"));

			core->addOption(submenuOption("Reports")
				.setTarget("Protections Reports"));

			core->addOption(toggleOption("Fake Lag")
				.addToggle(&m_vars.m_fake_lag));

			core->addOption(toggleOption("Block all Script Events")
				.addToggle(&m_vars.m_block_all_script_events));

			core->addOption(toggleOption("Force Entities Visible")
				.addToggle(&m_vars.m_force_entities_visible));

			core->addOption(buttonOption("Enable All")
				.addClick([] { enable_all(); }));

			core->addOption(buttonOption("Disable All")
				.addClick([] { disable_all(); }));
		});
		
	}

	void protections_menu::update() {
		render();
		getProtectionsNetworkEventsMenu()->update();
		getProtectionsScriptEventsMenu()->update();
		getProtectionsReportsMenu()->update();
	}
}