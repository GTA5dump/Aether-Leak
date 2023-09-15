#include "pch.h"
#include "network_new_session.h"
#include "rage/invoker/natives.h"
#include "gui/util/timer.h"
#include "menu/util/globals.h"

using namespace base::gui;
using namespace menu::network::new_session::vars;

namespace menu::network::new_session::vars {
	variables m_vars;

	enum class eSessionType {
		JOIN_PUBLIC,
		NEW_PUBLIC,
		CLOSED_CREW,
		CREW,
		CLOSED_FRIENDS = 6,
		FIND_FRIEND = 9,
		SOLO,
		INVITE_ONLY,
		JOIN_CREW,
		SC_TV,
		LEAVE_ONLINE = -1
	};

	struct SessionType {
		eSessionType id;
		const char name[29];
	};

	const SessionType sessions[] = {
		{eSessionType::JOIN_PUBLIC, "Find Public Session"},
		{eSessionType::NEW_PUBLIC, "Find New Public Session"},
		{eSessionType::CLOSED_CREW, "Create Closed Crew Session"},
		{eSessionType::CREW, "Create Crew Session"},
		{eSessionType::CLOSED_FRIENDS, "Create Closed Friend Session"},
		{eSessionType::FIND_FRIEND, "Find Friend Session"},
		{eSessionType::SOLO, "Create Solo Session"},
		{eSessionType::INVITE_ONLY, "Create Invite Only Session"},
		{eSessionType::JOIN_CREW, "Find Crew Session"},
		{eSessionType::SC_TV, "Social Club TV"},
		{eSessionType::LEAVE_ONLINE, "Leave GTA Online"},
	};

	void join_type(eSessionType session) {
		util::fiber::pool::add([=] {
			*script_global(2695915).as<int*>() = (session == eSessionType::SC_TV ? 1 : 0);// If SC TV Then Enable Spectator Mode

			if (session == eSessionType::LEAVE_ONLINE)
				*script_global(1574589).at(2).as<int*>() = -1;
			else
				*script_global(1575017).as<int*>() = (int)session;

			*script_global(1574589).as<int*>() = 1;
			util::fiber::go_to_main();
			*script_global(1574589).as<int*>() = 0;
		});
	}
}


namespace menu {

	void network_new_session_menu::render() {
		renderer::addSubmenu("New Session", "Network New Session", [](core* core) {
			for (const auto& session_type : sessions) {
				core->addOption(buttonOption(session_type.name)
					.addClick([=] { join_type(session_type.id); }));
			}
		});
	}


	void network_new_session_menu::update() {
		render();
	}

}