#include "pch.h"
#include "main.h"
#include "gui/renderer.h"
#include "rage/classes/joaat.h"
#include "util/log.h"
#include "gui/util/fonts.h"
#include "gui/util/texture.h"
#include "gui/util/panels.h"
#include "gui/util/notify.h"
using namespace base::gui;

namespace menu {

	std::string toLower(std::string data) {
		std::transform(data.begin(), data.end(), data.begin(), ::tolower);
		return data;
	}

	std::vector<base::gui::abstract_option*> SearchOptions(const std::string& str) {
		std::vector<base::gui::abstract_option*> results;
		renderer::getRenderer()->get_submenu_stack().top()->reset();

		for (auto& sub : renderer::getRenderer()->get_all_subs()) {
			for (std::size_t i = 0; i < sub->get_options_size(); i++) {
				auto option = sub->get_option(i);
				std::string optionName = toLower(option->get_left_text());
				if (optionName.find(str) != std::string::npos) {
					results.insert(results.end(), option);
				}
			}
		}

		return results;
	}

	void main_menu::render() {	

		renderer::addSubmenu("Main Menu", "Main Menu", [](core* core) {

			core->addOption(submenuOption("Player")
				.addTranslate()
				.setTarget("Player"));

			core->addOption(submenuOption("Network")
				.addTranslate()
				.setTarget("Network"));

			core->addOption(submenuOption("Protections")
				.addTranslate()
				.setTarget("Protections"));

			core->addOption(submenuOption("Vehicle")
				.addTranslate()
				.setTarget("Vehicle"));

			core->addOption(submenuOption("Weapons")
				.addTranslate()
				.setTarget("Weapons"));

			core->addOption(submenuOption("Teleport")
				.addTranslate()
				.setTarget("Teleport"));

			core->addOption(submenuOption("Spawner")
				.addTranslate()
				.setTarget("Spawner"));

			core->addOption(submenuOption("World")
				.addTranslate()
				.setTarget("World"));

			core->addOption(submenuOption("Recovery")
				.addTranslate()
				.setTarget("Recovery"));

			core->addOption(submenuOption("Miscellaneous")
				.addTranslate()
				.setTarget("Miscellaneous"));

			core->addOption(submenuOption("Settings")
				.addTranslate()
				.setTarget("Settings"));		
			
		});

	}

	void main_menu::update() {
		fonts::update_queue();
		menu::textures::tick();
		renderer::getRenderer()->render();
		render();
	}
}