#include "pch.h"
#include "network_players.h"
#include "rage/invoker/natives.h"
#include "players/network_selected_player.h"
#include "menu/util/players.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"

using namespace base::gui;
using namespace menu::players::vars;

namespace menu::players::vars {
	variables m_vars;
}

const char* sort_types[] = {
	"Player ID", "Alphabetical"
}; std::size_t sort_types_id = 0;

namespace menu {

	void network_players_menu::render() {
		renderer::addSubmenu("Players", "Network Players", [](core* core) {
			core->addOption(scrollOption<const char*, std::size_t>("Sort Players")
				.addScroll(&sort_types).setPosition(&sort_types_id));

			core->addOption(breakOption("Players"));

			for (uint32_t i = 0; i < 32; i++) {
				if (auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(i)) {
					if (ENTITY::DOES_ENTITY_EXIST(ped)) {
						std::string name = PLAYER::GET_PLAYER_NAME(i);

						core->addOption(submenuOption(PLAYER::GET_PLAYER_NAME(i))
							.addClick([=] { m_vars.m_selected_player = i; })
							.setTarget("selected_player"));
					}
				}
			}
		});
	}

	/*#define SCORE_BOARD_HEADSHOT_GLOBAL 1666668 + 2
			const char* GPic = "CHAR_MULTIPLAYER";
			const int HeadIndex = SCORE_BOARD_HEADSHOT_GLOBAL;
			for (int i = 0; i <= 150; i += 5)
			{
				uint64_t* base = script_global(HeadIndex + i).as<uint64_t*>();
				int playerId = (int)*base;
				if (playerId == patterns::get_net_player(plyr)->m_player_id)
				{
					uint64_t* logo = script_global(HeadIndex + i + 1).as<uint64_t*>();
					int logos = (int)*logo;
					GPic = (char*)PED::GET_PEDHEADSHOT_TXD_STRING(logos);
					render::draw_sprite({ GPic, GPic }, { GetRenderer()->m_posX - (GetRenderer()->m_width / 2) + 0.01f } , GetRenderer()->GetOptionY(index) + GetRenderer()->m_optionHeight / 2, 0.01125f, 0.0225f, 0, Color(255, 255, 255, 210));
					return 0.01125f;
					break;
				}
				if (playerId == -1)
					break;
			}*/

	void network_players_menu::update() {
		render();
		getNetworkSelectedPlayerMenu()->update();
	}

}