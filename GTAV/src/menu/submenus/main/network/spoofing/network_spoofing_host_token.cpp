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

#include "network_spoofing_host_token.h"

using namespace base::gui;
using namespace menu::network::spoofing::host_token::vars;

namespace menu::network::spoofing::host_token::vars {
	variables m_vars;

	void do_spoof(uint64_t token) {
		std::uint64_t host_token;
		caller::call<uint64_t>(patterns::generate_uuid, &host_token);

		host_token = m_vars.m_spoof ? token : host_token;

		*patterns::host_token = host_token;

		if ((*patterns::network)->m_game_session_ptr)
			(*patterns::network)->m_game_session_ptr->m_local_player.m_player_data.m_host_token = host_token;

		patterns::profile_gamer_info->m_host_token = host_token;
		patterns::player_info_gamer_info->m_host_token = host_token;
		(*patterns::communications)->m_voice.m_connections[0]->m_gamer_info.m_host_token = host_token;

		if ((*patterns::ped_factory)->m_local_ped && (*patterns::ped_factory)->m_local_ped->m_player_info)
			(*patterns::ped_factory)->m_local_ped->m_player_info->m_net_player_data.m_host_token = host_token;
	}
}


namespace menu {

	void network_spoofing_host_token_menu::render() {
		renderer::addSubmenu("Host Token", "Spoofing Host Token", [](core* core) {
			core->addOption(numberOption<uint64_t>("Host Token")
				.addNumber(&m_vars.host_token)
				.addMin(0).addMax(999999).addStep(1));

			std::string preview = std::format("Apply: {}", m_vars.host_token);

			core->addOption(buttonOption("Reset to default")
				.addClick([] { m_vars.m_spoof = false; }));

			core->addOption(buttonOption(preview.c_str())
				.addClick([] { m_vars.m_spoof = true; }));
		});

	}

	void network_spoofing_host_token_menu::update() {
		render();

		if (m_vars.m_spoof)
			do_spoof(m_vars.host_token);
		
	}

}