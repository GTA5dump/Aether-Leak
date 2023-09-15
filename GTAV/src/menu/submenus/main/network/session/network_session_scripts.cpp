#include "pch.h"
#include "network_session_scripts.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"
#include "rage/engine.h"

using namespace base::gui;
using namespace menu::network::session::scripts::vars;

namespace menu::network::session::scripts::vars {
	variables m_vars;
}


namespace menu {

	void session_scripts_menu::render() {
		renderer::addSubmenu("Session Scripts", "Session Scripts", [](core* core) {
			core->addOption(submenuOption("Freemode Scripts")
				.setTarget("freemode_scripts"));

			core->addOption(submenuOption("Arcade Games")
				.setTarget("arcade_games"));

			core->addOption(submenuOption("Session Breaking")
				.setTarget("session_breaking"));

			core->addOption(buttonOption("Flight School")
				.addClick([] { rage::engine::session_script(203); }));

			core->addOption(buttonOption("Darts")
				.addClick([] { rage::engine::session_script(14); }));

			core->addOption(buttonOption("Golf")
				.addClick([] { rage::engine::session_script(200); }));

			core->addOption(buttonOption("Impromptu Race")
				.addClick([] { rage::engine::session_script(16); }));

			core->addOption(buttonOption("Impromptu Deathmatch")
				.addClick([] { rage::engine::session_script(5); }));

			core->addOption(buttonOption("One on one Deathmatch")
				.addClick([] { rage::engine::session_script(204); }));

			core->addOption(buttonOption("Tutorial")
				.addClick([] { rage::engine::session_script(20); }));
		});

		renderer::addSubmenu("Session Breaking", "session_breaking", [](core* core) {
			core->addOption(buttonOption("Tennis")
				.addClick([] { rage::engine::session_script(12); }));

			});

		renderer::addSubmenu("Arcade Games", "arcade_games", [](core* core) {
			core->addOption(buttonOption("Space Monkey")
				.addClick([] { rage::engine::session_script(223); }));

			core->addOption(buttonOption("Gunslinger")
				.addClick([] { rage::engine::session_script(218); }));

			core->addOption(buttonOption("Wizard")
				.addClick([] { rage::engine::session_script(219); }));

			core->addOption(buttonOption("Camhedz")
				.addClick([] { rage::engine::session_script(225); }));

			core->addOption(buttonOption("QUB3D")
				.addClick([] { rage::engine::session_script(224); }));

			});

		renderer::addSubmenu("Freemode Scripts", "freemode_scripts", [](core* core) {
			core->addOption(buttonOption("Business Battle")
				.addClick([] { rage::engine::session_script(114); }));

			core->addOption(buttonOption("Challenges")
				.addClick([] { rage::engine::session_script(40); }));

			core->addOption(buttonOption("Checkpoints")
				.addClick([] { rage::engine::session_script(39); }));

			core->addOption(buttonOption("Hunt The Beast")
				.addClick([] { rage::engine::session_script(47); }));

			core->addOption(buttonOption("Hot Property")
				.addClick([] { rage::engine::session_script(43); }));

			core->addOption(buttonOption("Hot Target")
				.addClick([] { rage::engine::session_script(36); }));

			core->addOption(buttonOption("Kill List")
				.addClick([] { rage::engine::session_script(37); }));

			core->addOption(buttonOption("King of The Castle")
				.addClick([] { rage::engine::session_script(45); }));

			core->addOption(buttonOption("Criminal Damge")
				.addClick([] { rage::engine::session_script(46); }));

			core->addOption(buttonOption("Penned In")
				.addClick([] { rage::engine::session_script(41); }));
			});


	}


	void session_scripts_menu::update() {
		render();
	}

}