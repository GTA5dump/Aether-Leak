#pragma once
#include "pch.h"
#include "joaat.h"
#include "base.h"
#include "atMap.h"

namespace rage {
	//Program
	class scrProgram : public pgBase {
	public:
		uint8_t** m_code_blocks; //0x0010
		uint32_t m_hash; //0x0018
		uint32_t m_code_size; //0x001C
		uint32_t m_arg_count; //0x0020
		uint32_t m_local_count; //0x0024
		uint32_t m_global_count; //0x0028
		uint32_t m_native_count; //0x002C
		void* m_local_data; //0x0030
		int64_t** m_global_data; //0x0038
		void** m_native_entrypoints; //0x0040
		uint32_t m_proc_count; //0x0048
		uint32_t unk_004C; //0x004C
		const char** m_proc_names; //0x0050
		uint32_t m_name_hash; //0x0058
		uint32_t m_ref_count; //0x005C
		const char* m_name; //0x0060
		const char** m_strings_data; //0x0068
		uint32_t m_strings_count; //0x0070
		char m_breakpoints[12]; //0x0074 - rage::atMap<uint8_t>

		bool is_valid() const {
			return m_code_size != 0;
		}
		uint32_t get_num_code_pages() const {
			return (m_code_size + 0x3FFF) >> 14;
		}
		uint32_t get_code_page_size(uint32_t page) const {
			auto num = get_num_code_pages();
			if (page < num) {
				if (page == num - 1)
					return m_code_size & 0x3FFF;
				return 0x4000;
			}
			return 0;
		}
		uint32_t get_full_code_size() const {
			auto numPages = get_num_code_pages();
			if (!numPages)
				return 0;
			if (numPages == 1)
				--numPages;
			return (numPages * 0x4000) + (m_code_size & 0x3FFF);
		}
		uint8_t* get_code_page(std::uint32_t page) const {
			return m_code_blocks[page];
		}
		uint64_t get_string_page(uint64_t index) const {
			return *reinterpret_cast<uint64_t*>(uintptr_t(this) + offsetof(scrProgram, m_strings_data)) + index * 8;
		}
		uint8_t* get_code_address(uint32_t index) const {
			if (index < m_code_size)
				return &m_code_blocks[index >> 14][index & 0x3FFF];
			return nullptr;
		}
		const char* get_string(uint32_t index) const {
			if (index < m_strings_count)
				return &m_strings_data[index >> 14][index & 0x3FFF];
			return nullptr;
		}
		void** get_address_of_native_entrypoint(void* entrypoint) {
			for (uint32_t i{}; i != m_native_count; ++i) {
				if (m_native_entrypoints[i] == entrypoint) {
					return m_native_entrypoints + i;
				}
			}
			return nullptr;
		}
		uint64_t* get_native_table() {
			return *reinterpret_cast<uint64_t**>(uintptr_t(this) + offsetof(scrProgram, m_native_entrypoints));
		}
	}; //Size: 0x0080
	static_assert(sizeof(scrProgram) == 0x80);
	//Program Table Entry
	class scrProgramTableEntry {
	public:
		scrProgram* m_program; //0x0000
		char pad_0008[4]; //0x0008
		joaat_t m_hash; //0x000C
	}; //Size: 0x0010
	static_assert(sizeof(scrProgramTableEntry) == 0x10);
	//Program Table
	class scrProgramTable {
	public:
		scrProgramTableEntry* m_data; //0x0000
		char pad_0008[16]; //0x0008
		uint32_t m_size; //0x0018

		scrProgram* find_script(joaat_t hash) {
			for (uint32_t i{}; i != m_size; ++i) {
				if (m_data[i].m_hash == hash) {
					return m_data[i].m_program;
				}
			}
			return nullptr;
		}
		scrProgramTableEntry* begin() {
			return m_data;
		}
		scrProgramTableEntry* end() {
			return m_data + m_size;
		}
	}; //Size: 0x001C
}