#pragma once
#include "submenu.h"
#include "rage/classes/joaat.h"
#include "rage/engine.h"
#include "rage/invoker/natives.h"

namespace base::gui {
	class core : public base_submenu<core> {
	public:
		explicit core(const char* name, const char* target, std::function<void(core*)> action) {
			set_name(name); set_id(rage::joaat(target)); set_action(std::move(action));
		}
	};

	class pcore : public base_submenu<pcore> {
	public:
		explicit pcore(uint32_t* player, uint32_t id, std::function<void(pcore*)> action) :
			m_selected_player(player)
		{
			set_id(id);
			set_action(std::move(action));
		}
		const char* get_name() override {
			return PLAYER::GET_PLAYER_NAME(*m_selected_player);
		}
	private:
		uint32_t* m_selected_player;
	};

	class vcore : public base_submenu<vcore> {
	public:
		explicit vcore(int* vehicle, uint32_t id, std::function<void(vcore*)> action) :
			m_selected_vehicle(vehicle)
		{
			set_id(id);
			set_action(std::move(action));
		}
		const char* get_name() override {
			return rage::engine::get_vehicle_class_name(*m_selected_vehicle);
		}
	private:
		int* m_selected_vehicle;
	};
}