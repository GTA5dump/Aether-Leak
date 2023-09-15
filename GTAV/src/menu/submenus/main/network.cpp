#include "pch.h"
#include "network.h"
#include "network/network_session.h"
#include "network/network_players.h"
#include "network/network_new_session.h"
#include "network/network_transitions.h"
#include "network/network_spoofing.h"
using namespace base::gui;
using namespace menu::network::vars;

namespace menu::network::vars {
	variables m_vars;
	const char* targeting_mode[] = {
		"Traditional", "Free Aim", "Assisted Aim", "Assisted Aim - Full", "Assisted Aim - Partial"
	};

	int targeting_mode_id = 0;
}

namespace menu {

	void network_menu::render() {
		renderer::addSubmenu("Network", "Network", [](core* core) {
			core->addOption(submenuOption("Players")
				.setTarget("Network Players"));

			core->addOption(submenuOption("New Session")
				.setTarget("Network New Session"));

			core->addOption(submenuOption("Session")
				.setTarget("Network Session"));

			core->addOption(submenuOption("Transitions")
				.setTarget("Network Transitions"));

			core->addOption(submenuOption("Spoofing")
				.setTarget("Network Spoofing"));

			core->addOption(submenuOption("Request Services")
				.setTarget("Request Services"));

			core->addOption(submenuOption("Set Targeting Mode")
				.setTarget("Set Targeting Mode"));

			core->addOption(toggleOption("Bull Shark Testosterone (BST)")
				.addToggle(&m_vars.m_bst)
				.addClick([] { if (m_vars.m_bst) rage::engine::request_bullshark_testosterone(); }));

			core->addOption(buttonOption("Get BST")
				.addClick([] { rage::engine::request_bullshark_testosterone(); }));
		});

		renderer::addSubmenu("Set Targeting Mode", "Set Targeting Mode", [](core* core) {
			for (const auto& mode : targeting_mode) {
				core->addOption(buttonOption(mode)
					.addClick([=] {
						PLAYER::SET_PLAYER_TARGETING_MODE(mode[targeting_mode_id]);
				}));
			}
		});

		renderer::addSubmenu("Request Services", "Request Services", [](core* core) {
			core->addOption(buttonOption("Request Airstrike")
				.addClick([] { rage::engine::request_airstrike(); }));

			core->addOption(buttonOption("Request Ammo Drop")
				.addClick([] { rage::engine::request_ammo_drop(); }));

			core->addOption(buttonOption("Request Boat Pickup")
				.addClick([] { rage::engine::request_boat_pickup(); }));

			core->addOption(buttonOption("Request Ballistic Armour")
				.addClick([] { rage::engine::request_ballistic_armor(); }));

			core->addOption(buttonOption("Request Helicopter Pickup")
				.addClick([] { rage::engine::request_helicopter_pickup(); }));

			core->addOption(buttonOption("Request Helicopter Backup")
				.addClick([] { rage::engine::request_backup_helicopter(); }));
		});
	}

	void network_menu::update() {
		render();
		getNetworkPlayersMenu()->update();
		getNetworkSessionMenu()->update();
		getNetworkNewSessionMenu()->update();
		getNetworkTransitionsMenu()->update();
		getNetworkSpoofingMenu()->update();
	}
}