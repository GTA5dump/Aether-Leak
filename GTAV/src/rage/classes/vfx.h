#pragma once
#include "pch.h"


struct CWheelVfx {
    float m_slip_min; //0x0004
    float m_slip_max; //0x0008
    float m_pressure_min; //0x000C
    float m_pressure_max; //0x0010
    int m_type1; //0x0014
    BOOL m_unk1; //0x0018
    int m_type2; //0x001c
    BOOL m_unk2; //0x0010
    int m_type3; //0x0024
    BOOL m_unk3; //0x0028
    int m_type4; //0x002c
    BOOL m_unk4; //0x0030
    uint8_t m_r; //0x0034
    uint8_t m_g; //0x0035
    uint8_t m_b; //0x0036
};

class CVehicleStreamRender {
public:
    char _0x0000[0x8]; // unk
    float m_tire_height; // unk
    char _0x000C[0xB94]; // unk
    float m_tire_width; // 0x08
};  
static_assert(sizeof(CVehicleStreamRender) == 0xBA4);