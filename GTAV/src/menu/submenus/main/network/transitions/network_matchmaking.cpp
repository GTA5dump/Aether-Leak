#include "pch.h"
#include "network_matchmaking.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"
#include "rage/classes/enums.h"
#include "menu/util/scr_global.h"
#include "gui/render.h"

using namespace base::gui;
using namespace menu::network::matchmaking::vars;

namespace menu::network::matchmaking::vars {
	variables m_vars;
	struct RegionType {
		uint32_t id;
		const char name[22];
	};

	inline const RegionType regions[] = {
		{0, "CIS"},
		{1, "Africa"},
		{2, "East"},
		{3, "Europe"},
		{4, "China"},
		{5, "Australia"},
		{6, "West"},
		{7, "Japan"},
		{8, "Unknown"},
	};


}


namespace menu {

	void network_matchmaking_menu::render() {
		renderer::addSubmenu("Matchmaking", "Transitions Matchmaking", [](core* core) {
			core->addOption(submenuOption("Region Switcher")
				.setTarget("region_switcher"));

			core->addOption(submenuOption("Pool Switcher")
				.setTarget("pool_switcher"));

			core->addOption(toggleNumberOption<int>("Player Magnet")
				.addToggle(&m_vars.m_toggle_player_magnet)
				.addNumber(&m_vars.m_player_magnet_count)
				.addMin(0).addMax(32).addStep(1));

			core->addOption(toggleOption("Open Extra Slot")
				.addTranslate().addHotkey()
				.addToggle(&m_vars.m_open_extra_slot)
				.addClick([] { NETWORK::NETWORK_SESSION_SET_MATCHMAKING_GROUP(m_vars.m_open_extra_slot ? 4 : 0); }));
		});

		renderer::addSubmenu("Pool Switcher", "pool_switcher", [](core* core) {

			core->addOption(buttonOption("Normal")
				.addClick([] {}));

			core->addOption(buttonOption("Cheater")
				.addClick([] {}));

			core->addOption(buttonOption("Bad Sport")
				.addClick([] {}));

			core->addOption(buttonOption("Custom")
				.addClick([] {}));

			});

		renderer::addSubmenu("Region Switcher", "region_switcher", [](core* core) {
			for (const auto& region_type : regions) {
				core->addOption(buttonOption(region_type.name)
					.addClick([] {}));
			}
			});
	}


	void network_matchmaking_menu::update() {
		render();
		
	}

}