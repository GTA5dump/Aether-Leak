#pragma once
#include <cstdint>
#include "CEntity.h"

namespace rage {
#pragma pack(push, 1)
	class CPhysical : public CDynamicEntity {
	public:
		char pad_00F0[144]; //0x00F0
		uint64_t unk_0180; //0x0180
		uint32_t m_damage_bits; //0x0188
		uint8_t m_hostility; //0x018C
		char pad_018D[3]; //0x018D
		uint8_t unk_0190; //0x0190
		char pad_0191[3]; //0x0191
		uint32_t unk_0194; //0x0194
		char pad_0198[232]; //0x0198
		float m_health; //0x0280
		float m_maxhealth; //0x0284
		uint64_t unk_0280; //0x0280
		char pad_02B0[72]; //0x0290
		uint64_t unk_02D8; //0x02D8
		uint64_t unk_02E0; //0x02E0
		uint32_t unk_02E8; //0x02E8
	}; //Size: 0x02EC
	static_assert(sizeof(CPhysical) == 0x2EC);
#pragma pack(pop)
}
#pragma pack(push, 2)
class CObject : public rage::CPhysical {
public:
	char pad_030C[60]; //0x030C
	uint64_t unk_0348; //0x0348
	char pad_0350[8]; //0x0350
	uint64_t unk_0358; //0x0358
	uint16_t unk_0360; //0x0360
	uint32_t unk_0362; //0x0362
	uint16_t unk_0366; //0x0366
	char pad_0368[120];//0x0368
	uint64_t unk_03E0; //0x03E0
	char pad_03E8[8]; //0x03E8
	uint64_t unk_03F0; //0x03F0
	uint64_t unk_03F8; //0x03F8
	uint64_t unk_0400; //0x0400
	uint64_t unk_0408; //0x0408
	uint64_t unk_0410; //0x0410
}; //Size: 0x03F8
static_assert(sizeof(CObject) == 0x3F8);
#pragma pack(pop)