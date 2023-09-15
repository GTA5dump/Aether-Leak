#pragma once
#include "gui/options/core.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"
#include "gui/options/color.h"

namespace menu {
	class main_menu {
	public:
		void render();
		void update();
	};

	static main_menu* getMainMenu() {
		static main_menu instance;
		return &instance;
	}
}