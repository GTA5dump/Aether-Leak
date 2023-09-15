#pragma once
#include <cstdint>
#include "PeerInfo.h"

namespace rage {
	class rlSessionInfo {
	public:
		bool serialize(class rage::datBitBuffer& buf);
		bool deserialize(class rage::datBitBuffer& buf);
	public:
		char* encode();
		bool decode(char* gsinfo);
	public:
		uint64_t m_rockstar_id; //0x0000
		uint64_t m_session_token; //0x0008
		class rage::rlPeerInfo m_peer_info; //0x0010
	}; //Size: 0x00D0
	static_assert(sizeof(rage::rlSessionInfo) == 0xD0);
}