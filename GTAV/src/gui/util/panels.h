#pragma once
#include "util/math.h"
#include "gui/util/flags.h"
#include "gui/types.h"
#include "gui/renderer.h"
#include "menu/util/scr_global.h"

namespace menu {
    class panel {
    public:
        panel(float width, float title_height, float item_height) :
            m_width(width), m_title_height(title_height), m_item_height(item_height)
        {
        }
    public:
        void start_panel(std::string title);
        void push(std::string left, std::string right);
        void push_double(std::string left_left, std::string left_right, std::string right_left, std::string right_right);
        void end_panel();
        void set_next();

        void setRenderTitle(bool value) { m_render_title = value; }


    private:
        float m_width;
        float m_title_height;
        float m_item_height;
        bool m_next_to;
        eFont m_title_font = eFont::HouseScript;
        eFont m_item_font = eFont::ChaletLondon;
        color m_back_ground = { 0, 0, 0, 180 };
        float m_title_font_scale = 0.3f;
        float m_font_scale = 0.25f;
        math::vector2<float> m_padding = { 3.1f, 2.1f };
        bool m_render_title;
    }; inline panel* g_panel;




    inline math::vector2<float> g_panel_pos = { renderer::getRenderer()->m_position.x, renderer::getRenderer()->m_position.y - 0.006f };
    inline float g_panel_base = 0.f;
    inline float g_panel_base_2 = 0.f;
    inline float g_panel_spacing = 0.06f;

    inline void teleport_info_panel() {
        panel panel(0.17f, 0.03f, 0.03f);
        panel.start_panel("Teleport Location Info");
        panel.push("Txl", "Gay");
        panel.push_double("Stash", "Mega Gay", "Andy", "little Gay");
        panel.push("Scarface", "No Gay");
        panel.end_panel();
    }

    inline void player_info_panel(Player player) {
        if (*patterns::is_session_started) {
            panel panel(0.14f, 0.03f, 0.03f);
            panel.start_panel(PLAYER::GET_PLAYER_NAME(player));

            auto ped = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player);

            if (ped) {
                int rank = *script_global(scr_globals::gpbd_fm_1).at(player, scr_globals::size::gpbd_fm_1).at(205).at(6).as<int*>();
                int cash = *script_global(scr_globals::gpbd_fm_1).at(player, scr_globals::size::gpbd_fm_1).at(205).at(3).as<int*>();
                int bank = *script_global(scr_globals::gpbd_fm_1).at(player, scr_globals::size::gpbd_fm_1).at(205).at(56).as<int*>() - *script_global(scr_globals::gpbd_fm_1).at(player, scr_globals::size::gpbd_fm_1).at(205).at(3).as<int*>();

                int health = (int)ENTITY::GET_ENTITY_HEALTH(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player));
                int armor = (int)PED::GET_PED_ARMOUR(PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(player));
                float kd_ratio = *script_global(scr_globals::gpbd_fm_1)
                    .at(player, scr_globals::size::gpbd_fm_1)
                    .at(205)
                    .at(26)
                    .as<float*>();

                std::ostringstream oss;
                oss << std::fixed << std::setprecision(2) << kd_ratio;
                std::string kd_ratio_str = oss.str();




                std::string wanted = std::format("{}/5", PLAYER::GET_PLAYER_WANTED_LEVEL(ped));

                std::string ip = std::format("{}.{}.{}.{}", patterns::get_net_player(player)->GetGamerInfo()->m_external_ip.m_field1, patterns::get_net_player(player)->GetGamerInfo()->m_external_ip.m_field2, patterns::get_net_player(player)->GetGamerInfo()->m_external_ip.m_field3, patterns::get_net_player(player)->GetGamerInfo()->m_external_ip.m_field4);

                std::string device = *script_global(scr_globals::gpbd_fm_1).at(player, scr_globals::size::gpbd_fm_1).at(96).at(29).as<int*>() == 2 ? "Controller" : "Keyboard";

                std::ostringstream oss2;
                oss2 << "0x" << std::hex << patterns::get_net_player(player)->GetGamerInfo()->m_host_token;
                std::string hexAddress = oss2.str();

                panel.push("Rank", std::to_string(rank));
                panel.push("Health", std::to_string(health));
                panel.push("Cash", std::format("${}", std::to_string(cash)));
                panel.push("Armor", std::to_string(armor));
                panel.push("Bank", std::format("${}", std::to_string(bank)));
                panel.push("Wanted Level", wanted);
                panel.push("K/D Ratio", kd_ratio_str);
                panel.push("Rockstar ID", std::to_string(patterns::get_net_player(player)->GetGamerInfo()->m_gamer_handle.m_rockstar_id));
                panel.push("IP", ip);
                panel.push("Host Token", hexAddress);
                panel.push("Device", device);
                panel.push("Ping", "0");
            }


            panel.end_panel();
        }
    }

    inline void vehicle_info_panel() {
        panel panel(0.14f, 0.f, 0.02f);
        panel.setRenderTitle(false);

    

        panel.end_panel();

    }
}