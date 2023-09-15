#include "pch.h"
#include "network_transitions.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"
#include "rage/classes/enums.h"
#include "menu/util/scr_global.h"
#include "gui/render.h"
#include "transitions/network_matchmaking.h"
using namespace base::gui;
using namespace menu::network::transitions::vars;

namespace menu::network::transitions::vars {
	variables m_vars;

}


namespace menu {

	void network_transitions_menu::render() {
		renderer::addSubmenu("Transitions", "Network Transitions", [](core* core) {
			core->addOption(submenuOption("Matchmaking")
				.setTarget("Transitions Matchmaking"));

			core->addOption(toggleOption("Seamless Session Switch")
				.addToggle(&m_vars.m_seamless_session_switch));

			core->addOption(toggleOption("Skip Swoop Down")
				.addToggle(&m_vars.m_skip_swoop_down));

			core->addOption(toggleOption("Disable DLC Intro Movie")
				.addToggle(&m_vars.m_disable_dlc_intro_movie));

			core->addOption(toggleOption("Transition Helper")
				.addToggle(&m_vars.m_transition_helper));

			core->addOption(toggleOption("Show Transition State")
				.addToggle(&m_vars.m_show_transition_state));
		});
	}

	eTransitionState last_state = eTransitionState::TRANSITION_STATE_EMPTY;
	void network_transitions_menu::update() {
		render();
		getNetworkMatchMakingMenu()->update();
		constexpr char transition_states[][48] = { "TRANSITION_STATE_EMPTY", "Singleplayer Swoop Up", "Multiplayer Swoop Up", "Creator Swoop Up", "Pre-HUD Checks", "Wait HUD Exit", "Wait For Summon", "Singleplayer Swoop Down", "Multiplayer Swoop Down", "Cancel Joining", "Retry Loading", "Retry Loading Slot 1", "Retry Loading Slot 2", "Retry Loading Slot 3", "Retry Loading Slot 4", "Wait On Invite", "Prejoining Freemode Session Checks", "Look For Fresh Join Freemode", "Look To Join Another Session Freemode", "Confirm Freemode Session Joining", "Wait Join Freemode Session", "Creation Enter Session", "Pre-Freemode Launch Script", "Freemode Teamfull Check", "Start Freemode Launch Script", "Freemode Transition Create Player", "Is Freemode And Transition Ready", "Freemode Swoop Down", "Post Bink Video Warp", "Freemode Final Setup Player", "Move Freemode To Running State", "Freemode How To Terminate", "Start Creator Pre-Launch Script Check", "Start Creator Launch Script", "Creator Transition Create Player", "Is Creator And Transition Ready", "Creator Swoop Down", "Creator Final Setup Player", "Move Creator To Running State", "Prejoining Testbed Session Checks", "Look For Fresh Join Testbed", "Look For Fresh Host Testbed", "Look To Join Another Session Testbed", "Look To Host Session Testbed", "Confirm Testbed Session Joining", "Wait Join Testbed Session", "Start Testbed Launch Script", "Testbed Transition Create Player", "Is Testbed And Transition Ready", "Testbed Swoop Down", "Testbed Final Setup Player", "Move Testbed To Running State", "Testbed How To Terminate", "Quit Current Session Prompt", "Wait For Transition Session To Setup", "Terminate Singleplayer", "Wait Terminate Singleplayer", "Kick Terminate Session", "Terminate Session", "Wait Terminate Session", "Terminate Session And Hold", "Terminate Session And Move Into Holding State", "Team Swapping Checks", "Return To Singleplayer", "Wait For Singleplayer To Start", "Waiting For External Termination Call", "Terminate Maintransition", "Wait For Dirty Load Confirm", "DLC Intro Bink", "Spawn Into Personal Vehicle" };

		if (m_vars.m_transition_helper) {
			const auto state = *scr_globals::transition_state.as<eTransitionState*>();

			if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0)
				return;

			// When freemode script loaded remove loading screen.
			if (state == eTransitionState::TRANSITION_STATE_WAIT_JOIN_FM_SESSION && DLC::GET_IS_LOADING_SCREEN_ACTIVE())
			{
				SCRIPT::SHUTDOWN_LOADING_SCREEN();
			}

			if (last_state == state || state == eTransitionState::TRANSITION_STATE_EMPTY || state > eTransitionState::TRANSITION_STATE_SPAWN_INTO_PERSONAL_VEHICLE)
			{
				return;
			}

			if (HUD::BUSYSPINNER_IS_ON())
			{
				HUD::BUSYSPINNER_OFF();
			}

			last_state = state;
		}

		if (m_vars.m_show_transition_state) {
			const auto state = *scr_globals::transition_state.as<eTransitionState*>();

			if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0)
				return;

			if (last_state == state || state == eTransitionState::TRANSITION_STATE_EMPTY || state > eTransitionState::TRANSITION_STATE_SPAWN_INTO_PERSONAL_VEHICLE) {
				return;
			}

			if (HUD::BUSYSPINNER_IS_ON()) {
				HUD::BUSYSPINNER_OFF();
			}

			if ((int)state > 0 && (int)state < std::size(transition_states)) {
				auto const spinner_text = std::format("{} | {}", transition_states[(int)state], static_cast<int>(state));

				render::draw_text(spinner_text.c_str(), JUSTIFY_CENTER, {0.5f, 0.5f}, 0.5f, 0, {0, 0}, {255, 255, 255, 255}, false, true);
			}
			last_state = state;
		}

		if (m_vars.m_seamless_session_switch) {
			const auto state = *scr_globals::transition_state.as<eTransitionState*>();

			if (state <= eTransitionState::TRANSITION_STATE_FM_FINAL_SETUP_PLAYER)	{
				PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), true, 0);
				if (STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS()) {
					STREAMING::STOP_PLAYER_SWITCH();
					GRAPHICS::ANIMPOSTFX_STOP_ALL();
				}
			}
		}
	}

}