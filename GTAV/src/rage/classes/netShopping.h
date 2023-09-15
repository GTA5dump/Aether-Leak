#pragma once
#include "pch.h"

namespace rage::netShopping {
    struct CNetworkShoppingMgr  {
        char _0x0000[32];
        void* m_first;
        void* m_last;
        char _0x0030[9];
        bool m_is_busy1;
        char _0x003A[14];
        bool m_is_busy2;
        char _0x0049[7];
        bool m_is_busy3;
    };

    struct netTransaction {
        int m_id;
        int m_variation;
        int m_price;
        int m_multiplier;
        int m_value;
    };
}