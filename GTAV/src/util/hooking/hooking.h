#pragma once
#include "pch.h"
#include "minhook.h"
#include "util/Log.h"
namespace hooking {
#define HOOK_SIZE 20
	class DetourContext {
	public:
		std::pair<std::string, size_t> m_name;
		uint64_t m_address;
		uint8_t m_original[HOOK_SIZE];
		uint8_t m_hooked[HOOK_SIZE];
		uint8_t m_encryption;
	};

	class Detour {
	public:
		template<typename T>
		__declspec(noinline) bool detour(const char* name, uint64_t address, void* function, T** trampoline) {
			MH_STATUS init_status = MH_Initialize();
			if (init_status == MH_OK || init_status == MH_ERROR_ALREADY_INITIALIZED) {
				if (address && function) {
					uint8_t original[HOOK_SIZE];
					memcpy(original, (void*)address, HOOK_SIZE);//save the original hook

					MH_STATUS hook_status = MH_CreateHook((void*)address, function, (void**)trampoline);
					if (hook_status == MH_OK || hook_status == MH_ERROR_ALREADY_CREATED) {
						//LOG_CUSTOM_WARN("Hook", "Hooking %s", name)
						hook_status = MH_EnableHook((void*)address);
						if (hook_status == MH_OK) {
							
							DetourContext detour;
							detour.m_name = std::make_pair(std::string(name), strlen(name));
							detour.m_address = address;

							memcpy(detour.m_original, original, HOOK_SIZE);//set the original
							memcpy(detour.m_hooked, (void*)address, HOOK_SIZE);

							m_detours.push_back(detour);//update detour vector

							//LOG_CUSTOM_SUCCESS("Hook", "Hooked %s", name);
							return true;
						}
					}
				}
			}

			return false;
		};

	
		void remove_detour();
	private:
		std::vector<DetourContext> m_detours;
	};

	static Detour* getDetour() {
		static Detour instance;
		return &instance;
	}

	inline void cleanup() {
		getDetour()->remove_detour();
	}

	template<typename T>
	inline bool detour(const char* name, uint64_t address, void* function, T** trampoline) {
		return getDetour()->detour<T>(name, address, function, trampoline);
	}

	class vmt_hook
	{
	public:
		explicit vmt_hook(void* obj, std::size_t num_funcs);
		~vmt_hook();

		vmt_hook(vmt_hook&& that) = delete;
		vmt_hook& operator=(vmt_hook&& that) = delete;
		vmt_hook(vmt_hook const&) = delete;
		vmt_hook& operator=(vmt_hook const&) = delete;

		void hook(std::size_t index, void* func);
		void unhook(std::size_t index);

		template<typename T>
		T get_original(std::size_t index);

		void enable();
		void disable();

	private:
		void*** m_object;
		std::size_t m_num_funcs;

		void** m_original_table;
		std::unique_ptr<void* []> m_new_table;
	};

	template<typename T>
	inline T vmt_hook::get_original(std::size_t index)
	{
		return static_cast<T>(m_original_table[index]);
	}


}