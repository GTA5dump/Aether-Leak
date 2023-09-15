#include "pch.h"
#include "panels.h"
#include "gui/render.h"
#include "gui/renderer.h"

namespace menu {

    void panel::start_panel(std::string title) {
        auto menu = renderer::getRenderer();

        g_panel_base_2 = g_panel_pos.y;

        if (!m_next_to) {
            if (menu->m_position.x > 0.5f) {
                g_panel_pos.x = menu->m_position.x - (menu->m_width / 2.f) + (m_width - g_panel_spacing);
            }
            else {
                g_panel_pos.x = menu->m_position.x + (menu->m_width / 2.f) - (m_width - g_panel_spacing);
            }
        }
        else {
            if (menu->m_position.x > 0.5f) {
                g_panel_pos.x = menu->m_position.x - (menu->m_width / 2.f) - (m_width - g_panel_spacing);
            }
            else {
                g_panel_pos.x = menu->m_position.x + (menu->m_width / 2.f) + (m_width - g_panel_spacing);
            }
            g_panel_base = g_panel_base_2;
        }

        g_panel_base = g_panel_pos.y;


        g_panel_pos.y = renderer::getRenderer()->m_position.y - 0.006f;

        render::draw_rect({ g_panel_pos.x, render::get_rect_base(m_title_height, g_panel_base) }, { m_width, 0.004f }, menu->m_header.m_color);
        g_panel_base += 0.004f;

        render::draw_rect({ g_panel_pos.x, render::get_rect_base(m_title_height, g_panel_base) }, { m_width, m_title_height }, m_back_ground);
     
        render::draw_text(title.c_str(), eJustify::JUSTIFY_LEFT, { g_panel_pos.x, render::get_rect_base(m_title_height, g_panel_base) }, m_title_font_scale, (int)m_title_font, m_padding);
        g_panel_base += m_title_height;
    }

    void panel::push(std::string left, std::string right) {
        render::draw_rect({ g_panel_pos.x, render::get_rect_base(m_item_height, g_panel_base) }, { m_width, m_title_height }, m_back_ground);
        render::draw_text(left.c_str(), eJustify::JUSTIFY_LEFT, { g_panel_pos.x, render::get_rect_base(m_item_height, g_panel_base ) }, m_font_scale, (int)m_item_font, m_padding);
        render::draw_text(right.c_str(), eJustify::JUSTIFY_RIGHT, { g_panel_pos.x + (m_width / 2.f) - 0.003f, render::get_rect_base(m_item_height, g_panel_base) }, m_font_scale, (int)m_item_font, m_padding);
        g_panel_base += m_item_height;
    }

    void draw_text(std::string text, eJustify justify, Vector2 position, float size, int font, Vector2 padding, color color, float width, bool half, bool outline, bool shadow) {
        if (justify == eJustify::JUSTIFY_LEFT) {
            position.x -= (width / padding.x);
        }
        if (justify == eJustify::JUSTIFY_RIGHT) {
            HUD::SET_TEXT_RIGHT_JUSTIFY(true);
            position.x += width / padding.y;
            HUD::SET_TEXT_WRAP(0.f, position.x);
        }
        if (justify == eJustify::JUSTIFY_CENTER) {
            HUD::SET_TEXT_CENTRE(true);
        }

        if (half) {
            position.y -= render::get_text_height(font, size) / 2.f;
        }
        else {
            position.y -= render::get_text_height(font, size) / 1.5f;
        }

        HUD::SET_TEXT_SCALE(size, size);
        HUD::SET_TEXT_FONT(static_cast<int>(font));
        HUD::SET_TEXT_COLOUR(color.r, color.g, color.b, color.a);
        if (outline)
            HUD::SET_TEXT_OUTLINE();
        if (shadow)
            HUD::SET_TEXT_DROP_SHADOW();
        HUD::BEGIN_TEXT_COMMAND_DISPLAY_TEXT("STRING");
        HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(text.c_str());
        HUD::END_TEXT_COMMAND_DISPLAY_TEXT({ position.x, position.y }, 0);
    }

    void panel::push_double(std::string left_left, std::string left_right, std::string right_left, std::string right_right) {

        color m_text_color = color(255, 255, 255, 255);

        render::draw_rect({ g_panel_pos.x, render::get_rect_base(m_item_height, g_panel_base) }, { m_width, m_title_height }, m_back_ground);
        draw_text(left_left.c_str(), JUSTIFY_LEFT, { (g_panel_pos.x), render::get_rect_base(m_item_height, g_panel_base) }, m_font_scale, (int)m_item_font, { m_padding.x, m_padding.y }, m_text_color, m_width, false, false, false);
        draw_text(left_right.c_str(), JUSTIFY_RIGHT, { (g_panel_pos.x - (m_width / 2.f)), render::get_rect_base(m_item_height, g_panel_base) }, m_font_scale, (int)m_item_font, { m_padding.x, m_padding.y }, m_text_color, m_width, false, false, false);
        draw_text(right_left.c_str(), JUSTIFY_LEFT, { (g_panel_pos.x + (m_width / 2.f)), render::get_rect_base(m_item_height, g_panel_base) }, m_font_scale, (int)m_item_font, { m_padding.x, m_padding.y }, m_text_color, m_width, false, false, false);
        draw_text(right_right.c_str(), JUSTIFY_RIGHT, { (g_panel_pos.x), render::get_rect_base(m_item_height,g_panel_base) }, m_font_scale, (int)m_item_font, { m_padding.x, m_padding.y }, m_text_color, m_width, false, false, false);
        draw_text("~c~|", JUSTIFY_CENTER, { (g_panel_pos.x), render::get_rect_base(m_item_height, g_panel_base) }, m_font_scale * 2.f, (int)m_item_font, { m_padding.x, m_padding.y }, m_text_color, m_width, false, false, false);
        g_panel_base += m_item_height;
    }

    void panel::end_panel() {
        g_panel_base += g_panel_spacing;
    }

    void panel::set_next() {
        m_next_to = true;
    }

}