#include "pch.h"
#include "features.h"
#include "features/manager/manager.h"
#include "rage/invoker/natives.h"
#include "util/fiber.h"
namespace features {

	void suicide(features::actionFeature* feature) {
		ENTITY::SET_ENTITY_HEALTH(PLAYER::PLAYER_PED_ID(), 0, 0);
	}

	void godmode(features::toggleFeature* feature, bool& value) {
		if (value) {
			ENTITY::SET_ENTITY_INVINCIBLE(PLAYER::PLAYER_PED_ID(), true);
		}
	}

	void never_wanted(features::toggleFeature* feature, bool& value) {
		if (value) {
			PLAYER::SET_MAX_WANTED_LEVEL(0);
			PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_PED_ID());
			PLAYER::SET_PLAYER_WANTED_LEVEL_NOW(PLAYER::PLAYER_PED_ID(), 0);
		}
		else {
			PLAYER::SET_MAX_WANTED_LEVEL(5);
		}
	}

	void run() {
		features::g_manager.add(features::actionFeature("suicide", "Suicide", suicide));	
		features::g_manager.add(features::toggleFeature("player_godmode", "Godmode", godmode));
		features::g_manager.add(features::toggleFeature("player_disable_police", "Disable Police", never_wanted));

		features::g_manager.init();

		while (true) {
			features::g_manager.tick();
			util::fiber::go_to_main();
		}
	}
}