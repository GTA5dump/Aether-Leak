#pragma once
#include <cstdint>

namespace rage {
#pragma pack(push, 8)
	class netSyncNodeBase : public atRTTI<netSyncNodeBase> {
	public:
		virtual ~netSyncNodeBase() = 0;
		virtual bool IsDataNode() = 0;
		virtual bool IsParentNode() = 0;
		netSyncNodeBase* m_next_sibling; //0x0000
		netSyncNodeBase* m_prev_sibling; //0x0008
		netSyncNodeBase* m_root; //0x0010
		netSyncNodeBase* m_parent; //0x0018
		uint32_t m_flag_1; //0x0020
		uint32_t m_flag_2; //0x0024
		uint32_t m_flag_3; //0x0028
		uint32_t unk_002C; //0x002C
		netSyncNodeBase* m_first_child; //0x0030
	}; //Size: 0x0040
	static_assert(sizeof(netSyncNodeBase) == 0x40);
#pragma pack(pop)
#pragma pack(push, 8)
	class netSyncDataNodeBase : public netSyncNodeBase {
	public:
		uint32_t m_flags; //0x0040
		uint32_t unk_0044; //0x0044
		uint64_t unk_0048; //0x0048
		class netSyncDataNodeBase* m_parent_data; //0x0050
		uint32_t m_child_count; //0x58
		class netSyncDataNodeBase* m_children[8]; //0x005C
		uint8_t m_sync_frequencies[8]; //0x009C
		void* nodeBuffer; //0x00A4
	}; //Size: 0x00B0
	static_assert(sizeof(netSyncDataNodeBase) == 0xB0);
#pragma pack(pop)
#pragma pack(push, 8)
	class netSyncTree {
	public:
		virtual ~netSyncTree() = 0;

		char pad_0008[8]; //0x0008
		netSyncNodeBase* m_sync_node; //0x0010
		char pad_0018[8]; //0x0018
		uint32_t m_child_node_count; //0x0020
		char pad_0024[12]; //0x0024
	}; //Size: 0x0030
	static_assert(sizeof(netSyncTree) == 0x30);
#pragma pack(pop)
}