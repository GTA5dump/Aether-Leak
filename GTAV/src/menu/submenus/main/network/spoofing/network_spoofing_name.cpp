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

#include "network_spoofing_name.h"

using namespace base::gui;
using namespace menu::network::spoofing::name::vars;

namespace menu::network::spoofing::name::vars {
	variables m_vars;

    const char* textSettingsLabels[] = {
     "Normal",
     "Bold",
     "Italic"
    };

    const char* textSettingsValues[] = {
        "",
        "~h~",
        "~italic~"
    }; int textSettingsId = 0;

    const char* textColorsLabels[] = {
        "Default",
        "Black",
        "Blue",
        "Gold",
        "Green",
        "Grey",
        "Light Blue",
        "Orange",
        "Purple",
        "Red"
    };

    const char* textColorsValues[] = {
        "",
        "~v~",
        "~b~",
        "~y~",
        "~g~",
        "~c~",
        "~f~",
        "~o~",
        "~p~",
        "~r~"
    }; int textColorsId = 0;

    const char* textIconsLabels[] = {
        "None",
        "Wanted Star",
        "Rockstar Verified",
        "Rockstar"
    };

    const char* textIconsValues[] = {
        "",
        "~ws~",
        "~¦~",
        "~÷~"
    }; int textIconsId = 0;



    static bool active;
    void do_spoof() {
            MISC::DISPLAY_ONSCREEN_KEYBOARD(true, (char*)"Input", (char*)"", (char*)"", (char*)"", (char*)"", (char*)"", 20);
            while (MISC::UPDATE_ONSCREEN_KEYBOARD() == 0) {
                active = true;
                renderer::getRenderer()->reset_keys();
                util::fiber::go_to_main();
            }
            active = false;
            if (!MISC::GET_ONSCREEN_KEYBOARD_RESULT())
                return;
            m_vars.m_name = MISC::GET_ONSCREEN_KEYBOARD_RESULT();
    }
}


namespace menu {

	void network_spoofing_name_menu::render() {
		renderer::addSubmenu("Name", "Spoofing Name", [](core* core) {
            const char* og_name = PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID());

            core->addOption(keyboardOption("Input Name")
                .addClick([=] { do_spoof(); }));

            core->addOption(buttonOption("Reset to Default")
                .addClick([=] { m_vars.m_spoof = false; m_vars.m_name = og_name; }));

            std::string preview = std::format("Apply: {}{}{}{}", textSettingsValues[textSettingsId], textColorsValues[textColorsId], textIconsValues[textIconsId], m_vars.m_name.c_str());
            std::string name_to_apply = std::format("{}{}{}{}", textSettingsValues[textSettingsId], textColorsValues[textColorsId], textIconsValues[textIconsId], m_vars.m_name.c_str());

            core->addOption(buttonOption(preview.c_str())
                .addClick([=] { m_vars.m_spoof = true; m_vars.m_name = name_to_apply; }));

            core->addOption(breakOption("Settings"));

            core->addOption(scrollOption<const char*, int>("Color")
                .addScroll(&textColorsLabels).setPosition(&textColorsId));

            core->addOption(scrollOption<const char*, int>("Style")
                .addScroll(&textSettingsLabels).setPosition(&textSettingsId));

            core->addOption(scrollOption<const char*, int>("Icon")
                .addScroll(&textIconsLabels).setPosition(&textIconsId));

		});
	}

	void network_spoofing_name_menu::update() {
		render();

	}

}