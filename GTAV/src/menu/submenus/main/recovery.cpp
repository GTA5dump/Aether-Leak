#include "pch.h"
#include "recovery.h"
#include "gui/util/timer.h"
#include "rage/engine.h"
#include <vector>
#include <array>

using namespace base::gui;
using namespace menu::recovery::vars;

namespace menu::recovery::vars {
	variables m_vars;
	
	void fleeca_job() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_JOB_BONUS"), 1, 15000000, 0, 1},
			});
	}

	void bend_job() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_BEND_JOB"), 1, 15000000, 0, 1},
			});
	}

	void doomsday_finale() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_DOOMSDAY_FINALE_BONUS"), 1, 200000, 0, 1},
			});
	}

	void avenger_operations() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_AVENGER_OPERATIONS"), 1, 1000000, 0, 1},
			});
	}

	void gang_award() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_GANGOPS_AWARD_ORDER"), 1, 2000000, 0, 1},
			});
	}

	void daily_objective() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_DAILY_OBJECTIVE_EVENT"), 1, 1000000, 0, 1},
			});
	}

	void music_studio() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_MUSIC_STUDIO_SHORT_TRIP"), 1, 1000000, 0, 1},
			});
	}

	void casino_heist_finale() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_CASINO_HEIST_FINALE"), 1, 3619000, 0, 1 },
			});
	}

	void gang_money() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_GANGOPS_AWARD_MASTERMIND_4"), 1, 7000000, 0, 1 },
			});
	}

	void gang_money_2() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_GANGOPS_AWARD_MASTERMIND_3"), 1, 7000000, 0, 1 },
			});
	}

	void premium_job() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_PREMIUM_JOB"), 1, 300000, 0, 1 },
			});
	}

	void juggallo_phone() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_JUGGALO_PHONE_MISSION"), 1, 100000, 0, 1 },
			});
	}

	void juggalo_story() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_WINTER_22_AWARD_JUGGALO_STORY"), 1, 500000, 0, 1 },
			});
	}

	void agency_story_finale() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_AGENCY_STORY_FINALE"), 1, 3000000, 0, 1 },
			});
	}

	void island_heist_finale() {
		rage::engine::add_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1445302971, 4,
			{
				{ (int)rage::joaat("SERVICE_EARN_ISLAND_HEIST_FINALE"), 1, 2550000, 0, 1 },
			});
	}
}

namespace menu {

	void recovery_menu::render() {
		renderer::addSubmenu("Recovery", "Recovery", [](core* core) {
			core->addOption(submenuOption("Money")
				.setTarget("Recovery Money"));
		
		});

		renderer::addSubmenu("Money", "Recovery Money", [](core* core) {

			core->addOption(toggleOption("1 Million Loop")
				.addToggle(&m_vars.m_loop));

			core->addOption(breakOption("Transactions"));

			core->addOption(buttonOption("Fleeca Job (15M)")
				.addClick(fleeca_job));

			core->addOption(buttonOption("Bend Job (15M)")
				.addClick(bend_job));

			core->addOption(buttonOption("Gang Money(7M)")
				.addClick(gang_money));

			core->addOption(buttonOption("Gang Award (7M)")
				.addClick(gang_money_2));

			core->addOption(buttonOption("Casino Heist (3.6M)")
				.addClick(casino_heist_finale));

			core->addOption(buttonOption("Gang Operations (2M)")
				.addClick(gang_award));

			core->addOption(buttonOption("Daily Objective (1M)")
				.addClick(daily_objective));

			core->addOption(buttonOption("Music Studio (1M)")
				.addClick(music_studio));

			core->addOption(buttonOption("Avenger Operations (1M)")
				.addClick(avenger_operations));

			core->addOption(buttonOption("Juggalo Story Award (500k)")
				.addClick(juggalo_story));

			core->addOption(buttonOption("Premium Job (300k)")
				.addClick(premium_job));

			core->addOption(buttonOption("Agency Story Finale (300k)")
				.addClick(agency_story_finale));

			core->addOption(buttonOption("Island Heist Finale (250k)")
				.addClick(island_heist_finale));

			core->addOption(buttonOption("Doomsday Finale (200k)")
				.addClick(doomsday_finale));

			core->addOption(buttonOption("Juggalo Phone (100k)")
				.addClick(juggallo_phone));

		});
	}

	void recovery_menu::update() {
		render();
		int m_timer = 0;
		if (m_vars.m_loop) {
			run_timed(&m_timer, 1000, [] {
				rage::engine::send_basket_transaction(rage::joaat("CATEGORY_SERVICE_WITH_THRESHOLD"), 1633116913, rage::joaat("NET_SHOP_ACTION_EARN"), 1000000, 4);
			});
		}
	}
}