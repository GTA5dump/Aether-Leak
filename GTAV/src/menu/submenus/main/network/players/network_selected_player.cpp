#include "pch.h"
#include "network_selected_player.h"
#include "rage/invoker/natives.h"
#include "../network_players.h"
#include "gui/util/panels.h"
using namespace base::gui;
using namespace menu::players::selected::vars;

namespace menu::players::selected::vars {
	variables m_vars;
}

namespace menu {
	void network_selected_player_menu::render() {
		renderer::addPlayerSubmenu(&players::vars::m_vars.m_selected_player, rage::joaat("selected_player"), [](pcore* core) {
			player_info_panel(players::vars::m_vars.m_selected_player);


			core->addOption(buttonOption(PLAYER::GET_PLAYER_NAME(players::vars::m_vars.m_selected_player)));

			core->addOption(toggleOption("Spectate")
				.addToggle(&m_vars.m_spectate));

			core->addOption(numberOption<float>("Wrap")
				.addTooltip("Hotkeys Supported. Press F12 to bind. Test test estsetssssssss testestes tse setse tsetes tse test sets etset estset setsetsetests").addNumber(&g_panel_spacing).addMin(-1).addMax(1)
				.addStep(0.001f).setPrecision(3));
		});
	}

	void network_selected_player_menu::update() {
		render();

		if (m_vars.m_spectate) {
			const auto target = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(players::vars::m_vars.m_selected_player);

			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(true, target);
			HUD::SET_MINIMAP_IN_SPECTATOR_MODE(true, target);
		}
		else {
			NETWORK::NETWORK_SET_IN_SPECTATOR_MODE(false, -1);
			HUD::SET_MINIMAP_IN_SPECTATOR_MODE(false, -1);

		}
	}
}