#include "pch.h"
#include "renderer.h"
#include "rage/invoker/natives.h"
#include "render.h"
#include "rage/classes/enums.h"
#include "util/timer.h"
#include "gui/util/panels.h"
#include <gui/util/notify.h>
#include <menu/submenus/main.h>
#include "util/json.h"

namespace menu::renderer {
	bool pushed_options = false;
	int get_id_from_name(const char* name) {
		for (std::uint32_t i = 0; i < 32; ++i) {
			if (!strcmp(PLAYER::GET_PLAYER_NAME(i), name))
				return i;
		}
		return 0;
	}

	bool is_key_pressed(std::uint16_t key) {
		if (GetForegroundWindow() == g_window) {
			if (GetAsyncKeyState(key) & 0x8000) {
				return true;
			}
		}
		return false;
	}

	bool g_is_mouse_enabled;
	bool g_is_moving_menu;
	std::vector<int> m_disabled_mouse_keys = {
		0, 1, 2, 3, 4, 5, 6, 14, 15, 16, 17, 24, 69, 70, 84, 85, 92, 99, 100, 106, 114, 115, 121, 142, 241, 257, 261, 262, 329, 330, 331
	};

	math::vector2<float> get_mouse_pos() {
		return math::vector2<float>{
			PAD::GET_DISABLED_CONTROL_NORMAL(2, static_cast<int>(eControllerInputs::INPUT_CURSOR_X)),
				PAD::GET_DISABLED_CONTROL_NORMAL(2, static_cast<int>(eControllerInputs::INPUT_CURSOR_Y))
		};
	}

	bool is_point_in_rect(math::vector2<float> point, math::vector2<float> rectPos, math::vector2<float> rectSize) {
		auto left = rectPos.x - (rectSize.x / 2.f);
		auto right = rectPos.x + (rectSize.x / 2.f);
		auto top = rectPos.y + (rectSize.y / 2.f);
		auto bottom = rectPos.y - (rectSize.y / 2.f);

		return point.x <= right && point.x >= left && point.y <= top && point.y >= bottom;
	}

	void move_menu_with_mouse(math::vector2<float> mousePos, math::vector2<float> initialMenuPosition) {
		auto offsets = initialMenuPosition - mousePos;
		auto currentMousePos = get_mouse_pos();

		menu::renderer::getRenderer()->m_position = currentMousePos + offsets;
	}

	void tick() {
		static bool hasMouseClicked = false;
		static math::vector2<float> menuPosInit = { 0.f, 0.f };
		static math::vector2<float> mousePosInit = { 0.f, 0.f };
		if (g_is_mouse_enabled) {
			for (auto id : m_disabled_mouse_keys) {
				PAD::DISABLE_CONTROL_ACTION(2, id, true);
			}

			g_is_moving_menu = is_point_in_rect(get_mouse_pos(), { menu::renderer::getRenderer()->m_position.x, menu::renderer::getRenderer()->m_position.y + (menu::renderer::getRenderer()->m_header.m_height / 2) }, { menu::renderer::getRenderer()->m_width, menu::renderer::getRenderer()->m_header.m_height });

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && g_is_moving_menu) {
				if (!hasMouseClicked) {
					menuPosInit = menu::renderer::getRenderer()->m_position;
					mousePosInit = get_mouse_pos();
					hasMouseClicked = true;
				}
				move_menu_with_mouse(mousePosInit, menuPosInit);
			}
			else {
				hasMouseClicked = false;
			}

			// 1 = normal cursor
			// 2 = low opacity normal cursor
			// 3 = hand icon
			// 4 = grab icon
			// 5 = click icon
			// 6 = left arrow
			// 7 = right arrow
			// 8 = up arrow
			// 9 = down arrow
			// 10 = extend horizontally icon
			// 11 = normal mouse with plus next to it
			// 12 = normal mouse with negative next to it
			// any more than 12 is just taking 12's icon so it stops there

			if (g_is_moving_menu && GetAsyncKeyState(VK_LBUTTON))
				HUD::SET_MOUSE_CURSOR_STYLE(4);
			else if (g_is_moving_menu)
				HUD::SET_MOUSE_CURSOR_STYLE(3);
			else
				HUD::SET_MOUSE_CURSOR_STYLE(1);

			HUD::SET_MOUSE_CURSOR_THIS_FRAME();
		}
		else {
			hasMouseClicked = false;
		}
	}

	void update_last_key_pressed() {
		for (int i = 0; i > 255; i++) {
			if (GetAsyncKeyState(i)) {
				m_last_key_pressed = i;
			}
		}
	}

	bool is_key_held(bool keyState, bool& keyHeld, Timer& holdTimer, Timer& scrollTimer, std::chrono::milliseconds initialDelay, std::chrono::milliseconds accelerationDelay)
	{
		if (keyState) {
			if (!keyHeld) {
				if (holdTimer.Update()) {
					keyHeld = true;
					scrollTimer.SetDelay(initialDelay);
					return true;
				}
			}
			else {
				if (scrollTimer.Update()) {
					scrollTimer.SetDelay(accelerationDelay);
					return true;
				}
			}
		}
		else {
			keyHeld = false;
			holdTimer.Reset();
			scrollTimer.SetDelay(initialDelay);
		}

		return false;
	}


	void renderer::render() {




		//fonts::gfx::load();
		update_hotkeys();
		check_keys();
		handle_keys();
		//drawOverlay();
		update_last_key_pressed();
		if (m_opened) {
			if (!pushed_options) {
				util::fiber::sleep(50);
				push_options();
				pushed_options = true;
				//LOG_SUCCESS("Pushed Options");
			}
			PAD::DISABLE_CONTROL_ACTION(0, 27, true);
			m_draw_base_y = m_position.y;
			if (!m_submenu_stack.empty()) {
				auto sub = m_submenu_stack.top();
				sub->reset();
				sub->execute();
				tick();
				draw_header();
				draw_title(sub);
				draw_background(sub);
				if (sub->get_options_size() != 0) {
					std::size_t startPoint = 0;
					std::size_t endPoint = sub->get_options_size() > m_max_vis_options ? m_max_vis_options : sub->get_options_size();
					if (sub->get_options_size() > m_max_vis_options && sub->get_selected_option() >= m_max_vis_options) {
						startPoint = sub->get_selected_option() - m_max_vis_options + 1;
						endPoint = sub->get_selected_option() + 1;
					}
					for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j) {
						draw_option(sub->get_option(i), i == sub->get_selected_option());
					}
				}
				update_scroller();
				draw_footer();
				//render_color_preview(sub->get_option(sub->get_selected_option()));
				render_tooltip();

			}
		}
	}



	void renderer::update_scroller() {
		if (m_current != m_target)
			m_current = m_current * (1 - m_speed) + m_target * m_speed;
		if ((m_current > m_target - 0.0005) && (m_current < m_target + 0.0005))
			m_current = m_target;
	}



	void renderer::reset_keys() {
		m_open_key = false, m_back_key = false, m_enter_key = false, m_up_key = false; m_down_key = false,
			m_left_key = false, m_right_key = false; m_hotkey = false;
	}
	void renderer::check_keys() {
		reset_keys();
		m_open_key = is_key_pressed(VK_F4) || (PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlScriptRB) && PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendX)), m_back_key = is_key_pressed(VK_BACK) || PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendCancel), m_enter_key = is_key_pressed(VK_RETURN) || PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendAccept),
			m_up_key = is_key_pressed(VK_UP) || PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendUp), m_down_key = is_key_pressed(VK_DOWN) || PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendDown), m_left_key = is_key_pressed(VK_LEFT) || PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendLeft),
			m_right_key = is_key_pressed(VK_RIGHT) || PAD::IS_DISABLED_CONTROL_PRESSED(0, ControlFrontendRight); m_hotkey = is_key_pressed(VK_F8);
	}
	void renderer::handle_keys() {
		if (is_key_pressed(VK_F12)) {
			g_is_mouse_enabled ^= true;
		}

		static auto openTicker = GetTickCount();
		if (m_open_key && GetTickCount() - openTicker >= static_cast<std::uint32_t>(m_open_delay)) {
			openTicker = GetTickCount();
			m_opened ^= true;

			AUDIO::PLAY_SOUND_FRONTEND(-1, m_opened ? "SELECT" : "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
		}

		static Timer backTimer(0ms);
		backTimer.SetDelay(std::chrono::milliseconds(m_back_delay));
		if (m_opened && m_back_key && backTimer.Update()) {
			AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);

			if (m_submenu_stack.size() <= 1) {
				m_opened = false;
			}
			else {
				m_submenu_stack.pop();
			}
		}

		static Timer hotkeyTimer(0ms);
		hotkeyTimer.SetDelay(std::chrono::milliseconds(m_back_delay));
		if (m_opened && m_hotkey && hotkeyTimer.Update()) {
			AUDIO::PLAY_SOUND_FRONTEND(-1, "BACK", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
			m_hotkey_pressed = !m_hotkey_pressed;
		}

		if (m_opened && !m_submenu_stack.empty()) {
			auto sub = m_submenu_stack.top();

			static Timer enterTimer(0ms);
			enterTimer.SetDelay(std::chrono::milliseconds(m_enter_delay));
			if (m_enter_key && sub->get_options_size() != 0 && enterTimer.Update()) {
				AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				if (auto opt = sub->get_option(sub->get_selected_option())) {
					opt->handle_action(eOptionAction::click);
				}
			}

			static Timer upTimer(0ms);
			upTimer.SetDelay(std::chrono::milliseconds(m_vertical_delay));
			if (m_up_key && sub->get_options_size() != 0 && upTimer.Update()) {
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				sub->scroll_backward();
			}

			static Timer downTimer(0ms);
			downTimer.SetDelay(std::chrono::milliseconds(m_vertical_delay));
			if (m_down_key && sub->get_options_size() != 0 && downTimer.Update()) {
				AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_UP_DOWN", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
				sub->scroll_forward();
			}

			static Timer leftTimer(0ms);
			leftTimer.SetDelay(std::chrono::milliseconds(m_horizontal_delay));
			if (m_left_key && sub->get_options_size() != 0) {
				if (leftTimer.Update()) {
					AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
					if (auto opt = sub->get_option(sub->get_selected_option())) {
						opt->handle_action(eOptionAction::left_click);
					}

					// Decrease delay based on key hold duration
					if (leftTimer.GetElapsedTime() > std::chrono::milliseconds(500)) {
						m_horizontal_delay -= 100;  // Decrease the delay by 50 milliseconds
					}
					else if (leftTimer.GetElapsedTime() > std::chrono::milliseconds(100)) {
						m_horizontal_delay -= 50;  // Decrease the delay by 20 milliseconds
					}
				}
			}
			else {
				leftTimer.Reset();  // Reset the left timer when the key is released
			}

			static Timer rightTimer(0ms);
			rightTimer.SetDelay(std::chrono::milliseconds(m_horizontal_delay));
			if (m_right_key && sub->get_options_size() != 0) {
				if (rightTimer.Update()) {
					AUDIO::PLAY_SOUND_FRONTEND(-1, "NAV_LEFT_RIGHT", "HUD_FRONTEND_DEFAULT_SOUNDSET", false);
					if (auto opt = sub->get_option(sub->get_selected_option())) {
						opt->handle_action(eOptionAction::right_click);
					}

					// Decrease delay based on key hold duration
					if (rightTimer.GetElapsedTime() > std::chrono::milliseconds(500)) {
						m_horizontal_delay -= 100;  // Decrease the delay by 50 milliseconds
					}
					else if (rightTimer.GetElapsedTime() > std::chrono::milliseconds(100)) {
						m_horizontal_delay -= 50;  // Decrease the delay by 20 milliseconds
					}
				}
			}
			else {
				rightTimer.Reset();  // Reset the right timer when the key is released
			}
		}
	}

	void renderer::draw_scroller() {
		m_target = render::get_rect_base(m_option.m_height);

		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(2);
		//::draw_rect({ m_position.x, m_current }, { m_width, m_option.m_height }, m_scroller_color);
		render::draw_sprite({ "textures", "scroller" }, { m_position.x, m_current }, { m_width, m_option.m_height }, m_scroller_color, 0.f);
	}

	void renderer::draw_header() {
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(1);
		//render::draw_rect({ m_position.x, render::get_rect_base(m_header.m_height) }, { m_width, m_header.m_height }, m_header.m_color);
		render::draw_sprite({ "textures", "header" }, { m_position.x,  render::get_rect_base(m_header.m_height)}, { m_width, m_header.m_height }, m_header.m_color, 0.f);
		render::draw_text("AETHER", JUSTIFY_CENTER, { m_position.x, render::get_rect_base(m_header.m_height) }, m_header.m_font_scale, m_header.m_font, { 2.1f, 2.1f }, m_header.m_text_color, true);
		m_draw_base_y += m_header.m_height;
	}

	void renderer::draw_title(base::gui::abstract_submenu* sub) {
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(1);
		std::string name = "~bold~";
		name.append(sub->get_name());
		render::draw_rect({ m_position.x, render::get_rect_base(m_title.m_height) }, { m_width, m_title.m_height }, m_title.m_color);
		render::draw_text(name.c_str(), JUSTIFY_LEFT, {m_position.x, render::get_rect_base(m_title.m_height)}, m_title.m_font_scale, m_title.m_font, m_title.m_padding);
		char rightText[32] = {};
		std::snprintf(rightText, sizeof(rightText) - 1, "%zu / %zu", sub->get_selected_option() + 1, sub->get_options_size());

		std::string name2 = "~bold~";
		name2.append(rightText);
		render::draw_text(name2.c_str(), JUSTIFY_RIGHT, {m_position.x + (m_width / m_title.m_padding.x), render::get_rect_base(m_title.m_height)}, m_title.m_font_scale, m_title.m_font, m_title.m_padding);

		m_draw_base_y += m_title.m_height;
	}

	void renderer::draw_background(base::gui::abstract_submenu* sub) {
		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(1);
		int visible_options = sub->get_options_size() > m_max_vis_options ? m_max_vis_options : sub->get_options_size();

		float bg_height = m_option.m_height * visible_options;
		m_bg_target = bg_height;

		if (m_bg_current != m_bg_target)
			m_bg_current = m_bg_current * (1 - m_speed) + m_bg_target * m_speed;
		if ((m_bg_current > m_bg_target - 0.0005) && (m_bg_current < m_bg_target + 0.0005))
			m_bg_current = m_bg_target;

		render::draw_rect({ m_position.x, m_draw_base_y + (m_bg_current / 2.f) }, { m_width, m_bg_current }, m_option.m_color);
	}

	void renderer::draw_option(base::gui::abstract_option* option, bool selected) {
		if (selected) {
			draw_scroller();
		}

		auto sub = m_submenu_stack.top();

		GRAPHICS::SET_SCRIPT_GFX_DRAW_ORDER(4);
		auto y_pos = selected ? m_current : render::get_rect_base(m_option.m_height);
		
		if (selected) {
			if (!strcmp(sub->get_name(), "Players")) {
				if (sub->get_selected_option() != 0 && sub->get_selected_option() != 1) {
					g_panel_spacing = 0.06f;
					menu::player_info_panel(get_id_from_name(option->get_left_text()));
				}
			}
		}


		if (g_is_mouse_enabled && is_point_in_rect(get_mouse_pos(), { m_position.x, render::get_rect_base(m_option.m_height) }, { m_width, m_option.m_height })) {
			auto sub = m_submenu_stack.top();
			sub->set_selected_option(sub->get_id());
		}
		if (g_is_mouse_enabled && is_point_in_rect(get_mouse_pos(), { m_position.x, render::get_rect_base(m_option.m_height) }, { m_width, m_option.m_height }) && is_key_pressed(VK_LBUTTON)) {
			option->handle_action(eOptionAction::click);
		}

		if (option->get_flag(eOptionFlag::simple_option)) {
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
			render::draw_text(option->get_right_text(), JUSTIFY_RIGHT, { m_position.x + (m_width / m_option.m_padding.x), y_pos}, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
		}

		if (option->get_flag(eOptionFlag::indicator)) {
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
			render::draw_right_text(">", m_position.x + (m_width / m_option.m_padding.x), m_draw_base_y + (m_option.m_height / 2.f)
				- (render::get_text_height(2, 0.4) / 1.725f), 0.4, 2,
				selected ? m_option.m_selected_text_color : m_option.m_text_color, false, false);
		}

		if (option->get_flag(eOptionFlag::toggle_option)) {
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
			auto size = render::get_sprite_scale(0.045);
			if (m_toggled_on) {
				render::draw_sprite({ "textures", "toggle_on" }, { m_position.x + (m_width / m_option.m_padding.x - 0.0045f),
					m_draw_base_y + (m_option.m_height / 2.f), }, { size.x, size.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color, 0.0f);
			}
			else {
				render::draw_sprite({ "textures", "toggle_off" }, { m_position.x + (m_width / m_option.m_padding.x - 0.0045f),
					m_draw_base_y + (m_option.m_height / 2.f), }, { size.x, size.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color, 0.0f);
			}
		}

		if (option->get_flag(eOptionFlag::toggle_number_option)) {
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
			auto size = render::get_sprite_scale(0.045);
			if (m_toggled_on) {
				render::draw_sprite({ "textures", "toggle_on" }, { m_position.x + (m_width / m_option.m_padding.x - 0.0045f),
					m_draw_base_y + (m_option.m_height / 2.f), }, { size.x, size.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color, 0.0f);
			}
			else {
				render::draw_sprite({ "textures", "toggle_off" }, { m_position.x + (m_width / m_option.m_padding.x - 0.0045f),
					m_draw_base_y + (m_option.m_height / 2.f), }, { size.x, size.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color, 0.0f);
			}

			render::draw_text(option->get_right_text(), JUSTIFY_RIGHT, { m_position.x + test, y_pos }, m_option.m_font_scale, m_option.m_font, { m_option.m_padding.x - test, m_option.m_padding.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color);
		}

		if (option->get_flag(eOptionFlag::toggle_scroll_option)) {
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
			auto size = render::get_sprite_scale(0.045);
			if (m_toggled_on) {
				render::draw_sprite({ "textures", "toggle_on" }, { m_position.x + (m_width / m_option.m_padding.x - 0.0045f),
					m_draw_base_y + (m_option.m_height / 2.f), }, { size.x, size.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color, 0.0f);
			}
			else {
				render::draw_sprite({ "textures", "toggle_off" }, { m_position.x + (m_width / m_option.m_padding.x - 0.0045f),
					m_draw_base_y + (m_option.m_height / 2.f), }, { size.x, size.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color, 0.0f);
			}

			render::draw_text(option->get_right_text(), JUSTIFY_RIGHT, { m_position.x + test, y_pos }, m_option.m_font_scale, m_option.m_font, { m_option.m_padding.x - test, m_option.m_padding.y }, selected ? m_option.m_selected_text_color : m_option.m_text_color);
		}

		if (option->get_flag(eOptionFlag::color_option)) {
			m_color_opt = true;
			auto size = render::get_sprite_scale(0.015);

			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
			
			render::draw_sprite({ "textures", "color" }, { m_position.x + (m_width / m_option.m_padding.x - 0.006f),
					m_draw_base_y + (m_option.m_height / 2.f), }, { size.x, size.y }, option->get_color(), 0.0f);
		}

		if (option->get_flag(eOptionFlag::number_option)) {
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);
			render::draw_right_text(option->get_right_text(), m_position.x + (m_width / m_option.m_padding.x),
				m_draw_base_y + (m_option.m_height / 2.f) - (render::get_text_height(m_option.m_font, m_option.m_font_scale) / 1.5f) - 0.00007,
				m_option.m_font_scale, m_option.m_font, selected ? m_option.m_selected_text_color : m_option.m_text_color, false, false);
		}

		if (option->get_flag(eOptionFlag::keyboard_option)) {
			auto size = render::get_sprite_scale(0.03);
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);

			render::draw_sprite({ "textures", "keyboard" }, { m_position.x + (m_width / m_option.m_padding.x - 0.004f),
				m_draw_base_y + (m_option.m_height / 2.f), }, size, selected ? m_option.m_selected_text_color : m_option.m_text_color, 0.f);

			render::draw_right_text(option->get_right_text(), m_position.x + (m_width / m_option.m_padding.x) - 0.012f,
				m_draw_base_y + (m_option.m_height / 2.f) - (render::get_text_height(m_option.m_font, m_option.m_font_scale) / 1.5f) - 0.00007,
				m_option.m_font_scale, m_option.m_font, selected ? m_option.m_selected_text_color : m_option.m_text_color, false, false);
		}
		if (option->get_flag(eOptionFlag::scroll_option)) {
			render::draw_text(option->get_left_text(), JUSTIFY_LEFT, { m_position.x, y_pos }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, selected ? m_option.m_selected_text_color : m_option.m_text_color);

			render::draw_right_text(option->get_right_text(), m_position.x + (m_width / m_option.m_padding.x),
				m_draw_base_y + (m_option.m_height / 2.f) - (render::get_text_height(m_option.m_font, m_option.m_font_scale) / 1.5f) - 0.00007,
				m_option.m_font_scale, m_option.m_font, selected ? m_option.m_selected_text_color : m_option.m_text_color, false, false);
		}
	
		if (option->get_flag(eOptionFlag::break_option)) {
			if (m_down_key) {
				while (sub->get_option(sub->get_selected_option())->get_flag(eOptionFlag::break_option)) {
					sub->set_selected_option(sub->get_selected_option() + 1);
				}
			}
			else if (m_up_key) {
				while (sub->get_option(sub->get_selected_option())->get_flag(eOptionFlag::break_option)) {
					sub->set_selected_option(sub->get_selected_option() - 1);
				}
			}

			render::draw_text(option->get_left_text(), JUSTIFY_CENTER, { m_position.x, m_draw_base_y + (m_option.m_height / 2.f) }, m_option.m_font_scale, m_option.m_font, m_option.m_padding, m_option.m_text_color);
		}

		if (sub->get_selected_option() >= sub->get_options_size()) {
			sub->set_selected_option(sub->get_options_size() - 1);
		}

		if (m_hotkey_pressed && selected && option->get_bool_pointer()) {
			render::draw_text("Press any key to set hotkey...", JUSTIFY_CENTER, { 0.5f, 0.5f }, 0.5f, 0, { 0, 0}, { 255, 255, 255, 255 }, false, true);

			static int delay_tick;
			static bool waiting_for_key = true;

			if (GetTickCount64() - delay_tick > 150 && waiting_for_key) {
				for (int key = 0; key < 256; key++) {
					if (is_key_pressed(key) && key != VK_F8) {
						m_last_key_pressed = key;
						waiting_for_key = false;
						break;
					}
				}
			}

			if (!waiting_for_key) {
				m_bool_hotkeys.insert({ rage::joaat(option->get_left_text()), { m_last_key_pressed, option->get_bool_pointer(), option->get_left_text() } });

				menu::notify::stacked(std::format("Assigned Option {} To Hotkey {}", option->get_left_text(), g_key_names[m_last_key_pressed]));

				m_hotkey_pressed = false;
				delay_tick = GetTickCount64();
				waiting_for_key = true;
			}
		}

		m_draw_base_y += m_option.m_height;
	}

	void renderer::update_hotkeys() {
		if (m_bool_hotkeys.empty()) {
			return; // Or handle the case appropriately
		}

		static int delay_tick;
		for (auto pair : m_bool_hotkeys) {
			if (GetTickCount64() - delay_tick > 500) {
				if (is_key_pressed(pair.second.m_key)) {
					*pair.second.m_pointer = !*pair.second.m_pointer;

					AUDIO::PLAY_SOUND_FRONTEND(-1, "SELECT", "HUD_FRONTEND_DEFAULT_SOUNDSET", FALSE);

					if (*pair.second.m_pointer) {
						menu::notify::stacked(std::format("Pressed {}, Enabled {},", g_key_names[pair.second.m_key], pair.second.m_name).c_str());
					}
					else {
						menu::notify::stacked(std::format("Pressed {}, Disabled {},", g_key_names[pair.second.m_key], pair.second.m_name).c_str());
					}

					delay_tick = GetTickCount64();
				}
			}
		}
	}

	void renderer::load_hotkeys() {
		using jsonf = nlohmann::json;
		static jsonf loader;
		try {
			std::filesystem::path cheat_path;
			cheat_path.append(Util::GetDocumentsPath());
			cheat_path.append("neon");
			cheat_path.append("hotkeys.json");
			std::ifstream file(cheat_path);
			if (!file.good()) {
				return;
			}
			file >> loader;
			LOG("Found %d Hotkeys", loader.size());
			m_bool_hotkeys;
			for (auto& item : loader.items()) {
				const std::string& key = item.key();
				u32 id = std::stoul(key.c_str());
				int hotkey = item.value()["Hotkey"].get<int>();
				if (get_option_pointer(id)) {
					if (get_option_pointer(id)->get_bool_pointer()) {
						m_bool_hotkeys.insert({ id, { hotkey, get_option_pointer(id)->get_bool_pointer(), get_option_pointer(id)->get_left_text()} });
					}
				}
			}
		}

		catch (const std::exception& e) { // catch any exception derived from std::exception
			LOG_ERROR(e.what());
		}
	}

	void renderer::save_hotkeys() {
		if (m_bool_hotkeys.empty()) {
			return; 
		}

		using jsonf = nlohmann::json;
		static jsonf save;
		for (auto hotkey : m_bool_hotkeys) {
			std::filesystem::path cheat_path;
			cheat_path.append(Util::GetDocumentsPath());
			cheat_path.append("neon");
			cheat_path.append("hotkeys.json");
			std::ofstream file(cheat_path, std::ios::out | std::ios::trunc);
			save[std::to_string(hotkey.first).c_str()]["Hotkey"] = hotkey.second.m_key;
			std::ofstream json_file(cheat_path);
			json_file << std::setw(4) << save << std::endl;
			json_file.close();
		}
	}

	void renderer::render_color_preview(base::gui::abstract_option* option) {
		m_draw_base_y += 0.005f;
		if (m_color_opt) {
			render::draw_rect({ m_position.x, m_draw_base_y + (0.033f / 2.f) }, { m_width, 0.033f }, option->get_color());
			m_draw_base_y += 0.033f;
		}	
	}

	void renderer::render_tooltip() {
		if (!m_submenu_stack.empty())
		{
			auto sub = m_submenu_stack.top();
			if (sub->get_options_size())
			{
				if (auto opt = sub->get_option(sub->get_selected_option()))
				{
					tooltip = opt->get_description();
				}
			}
		}

		if (!tooltip.c_str() || !*tooltip.c_str())
			return;

		m_draw_base_y += 0.001f;
		float y = 0.f;
		y += m_draw_base_y;

		float scaled_body_height = render::get_normalized_font_scale(m_option.m_font, m_option.m_font_scale);
		HUD::SET_TEXT_FONT(m_option.m_font);
		HUD::SET_TEXT_SCALE(0.f, scaled_body_height);
		HUD::SET_TEXT_WRAP(m_position.x - (m_width / 2.f) + 0.004f, (1.0f - (1.0f - (m_position.x + 0.1575f - (0.23f - m_option.m_font_scale)) - m_wrap)));
		HUD::BEGIN_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING("STRING");
		HUD::ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME(tooltip.c_str());

		float height = m_option.m_height;

		int lines = HUD::END_TEXT_COMMAND_GET_NUMBER_OF_LINES_FOR_STRING({ m_position.x - (m_width / 2.f) + 0.004f, y + 0.005f });
		if (lines > 1) {
			height = (lines * HUD::GET_RENDERED_CHARACTER_HEIGHT(scaled_body_height, m_option.m_font)) + (0.005f * lines) + 0.005f;
		}

		std::pair<std::string, std::string> texture = { "textures", "scroller" };
		render::draw_sprite_aligned(texture, { m_position.x - (m_width / 2.f), y}, { m_width, height }, 0.f, m_option.m_color);
		render::draw_text2(tooltip.c_str(), {m_position.x - (m_width / 2.f) + 0.004f, y + 0.005f }, scaled_body_height, m_option.m_font, { 255, 255, 255, 255 }, JUSTIFY_LEFT, { m_position.x - (m_width / 2.f) + 0.004f, (1.0f - (1.0f - (m_position.x + 0.1575f - (0.23f - m_option.m_font_scale)) - m_wrap)) });

		m_draw_base_y += m_option.m_height;
	}

	void renderer::draw_footer() {
	//	render::draw_rect({ m_position.x, m_draw_base_y + (m_footer.m_height / 2.f) }, { m_width, m_footer.m_height }, m_footer.m_color);;
		
		render::draw_sprite({ "textures", "footer" }, { m_position.x, render::get_rect_base(m_footer.m_height, m_position.y + m_header.m_height + m_title.m_height + m_bg_current) }, { m_width, m_footer.m_height }, m_footer.m_color, 0.f); // so the footer animates like the option bg

		//render::draw_sprite({ "textures", "footer" }, { m_position.x,  m_draw_base_y + (m_footer.m_height / 2.f) }, { m_width, m_footer.m_height }, m_footer.m_color, 0.f);
		float size = 0.02f;
		auto sizee = render::get_sprite_scale(size);
		
		render::draw_text("1.0", JUSTIFY_RIGHT, { m_position.x + (m_width / m_title.m_padding.x), render::get_rect_base(m_footer.m_height, m_position.y + m_header.m_height + m_title.m_height + m_bg_current) }, m_title.m_font_scale, m_title.m_font, m_title.m_padding);
		render::draw_sprite({ "textures", "dots" }, { m_position.x, render::get_rect_base(m_footer.m_height, m_position.y + m_header.m_height + m_title.m_height + m_bg_current) }, { sizee.x, sizee.y }, { 255, 255, 255, 255 }, 0.f);
		m_draw_base_y += m_footer.m_height;
	}

	void renderer::push_options() {
		m_all_options.clear();
		for (auto&& sub : m_all_subs) {
			for (int i = 0; i < sub->get_options_size(); i++) {
				m_all_options.insert({ rage::joaat(sub->get_option(i)->get_left_text()), sub->get_option(i) });
			}
		}
	}

	base::gui::abstract_option* renderer::get_option_pointer(u32 name) {
		if (auto it = m_all_options.find(name);  it != m_all_options.end()) {
			return &(it->second)[0];
		}
		else {
			LOG(std::format("Failed To Find Option {}", name).c_str());
			return nullptr;
		}
	}

	void renderer::drawOverlay()
	{
		if (!m_IsOverlayEnabled) {
			return;
		}

		static int FPS;
		static char Buffer[80];
		static Timer updateTimer(0ms);
		updateTimer.SetDelay(std::chrono::seconds(1));
		if (updateTimer.Update()) {
			FPS = 60;

			std::time_t CurrentTime = time(0); struct std::tm tstruct;
			tstruct = *localtime(&CurrentTime);
			strftime(Buffer, sizeof(Buffer), "%c", &tstruct);
		}

		std::string Format = "Aether" + std::string(" | FPS: ") + std::to_string(FPS) + std::string(" | ") + (std::string)Buffer;

		render::draw_left_text(
			Format.c_str(),
			m_OverlayX - (m_OverlayWidth / m_OverlayPadding),
			m_OverlayY - (render::get_text_height(m_option.m_font, m_option.m_font_scale)) + 0.001f,
			m_option.m_font_scale,
			m_option.m_font,
			m_OverlayTextColor, false, false);

		render::draw_rect({
			m_OverlayX,
			m_OverlayY, },
			{	m_OverlayWidth,
			m_OverlayHeight},
			m_OverlayColor);

		render::draw_rect({
			m_OverlayX,
			m_OverlayY - (m_OverlayLineHeight / 2.f) - (m_OverlayHeight / 2.f) },
			{ m_OverlayWidth,
			m_OverlayLineHeight },
			m_OverlayLineColor);
	}


}

