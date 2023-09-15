#pragma once
#include "gui/options/core.h"
#include "gui/options/submenu.h"
#include "gui/options/option.h"
#include "gui/options/button.h"
#include "gui/options/submenu_option.h"
#include "gui/options/number.h"
#include "gui/options/toggle.h"
#include "gui/options/scroll.h"
#include "gui/options/break.h"
namespace menu {
	class session_block_join_menu {
	public:
		void render();
		void update();
	};

	static session_block_join_menu* getSessionBlockJoinMenu() {
		static session_block_join_menu instance;
		return &instance;
	}

	namespace network::session::block_join::vars {
		struct variables {
			bool m_from_everyone;
			bool m_from_advertisers;
			bool m_rid_joins;
			bool m_targeted_joins;

			int m_message_id = 0;
		};

		extern variables m_vars;
	}
}