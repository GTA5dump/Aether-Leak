#include "pch.h"
#include "settings.h"
#include "gui/renderer.h"
#include "util/log.h"
#include "settings/position.h"
#include "settings/color.h"
#include "settings/hotkeys.h"
using namespace base::gui;

namespace menu {

	void settings_menu::render() {
		renderer::addSubmenu("Settings", "Settings", [](core* core) {
			core->addOption(submenuOption("Hotkeys")
				.addTranslate()
				.setTarget("Hotkeys"));

			core->addOption(submenuOption("Position")
				.addTranslate()
				.setTarget("Settings Position"));

			core->addOption(submenuOption("Color")
				.addTranslate()
				.setTarget("Settings Color"));

			core->addOption(buttonOption("Unload")
				.addTranslate()
				.addClick([] { g_running = false; }));
		});
	}

	void settings_menu::update() {
		render();
		getPositionMenu()->update();
		getColorMenu()->update();
		getHotkeysMenu()->update();
	}
}