#pragma once
#include "pch.h"
#include <unordered_map>

#undef GetTempPath

namespace util::dirs {
	class directories {
	public:
		bool load();
		std::string create_string(std::string base, std::string folder);
		std::vector<std::string> get_files_in_directory(std::string dir, std::string extension);

		const char* get_log() { return get_path(PATH_LOG); }
		const char* get_gfx() { return get_path(PATH_GFX); }
		const char* get_config() { return get_path(PATH_CONFIG); }
		const char* get_aether() { return get_path(PATH_AETHER); }
		const char* get_tmp() { return get_path(PATH_TMP); }
		const char* get_textures() { return get_path(PATH_TEXTURES); }


		enum Paths {
			PATH_DOCUMENTS,
			PATH_AETHER,
			PATH_LOG,
			PATH_GFX,
			PATH_CONFIG,
			PATH_TMP,
			PATH_TEXTURES
		};
	private:
		std::unordered_map<Paths, char[MAX_PATH]> m_paths;

		const char* get_path(Paths path) {
			return m_paths[path];
		}
	};

	directories* get_directories();

	inline std::vector<std::string> get_files_in_directory(std::string dir, std::string extension = ".txt") {
		return get_directories()->get_files_in_directory(dir, extension);
	}

	inline bool load() {
		return get_directories()->load();
	}

	inline std::string create_string(std::string base, std::string folder) {
		return get_directories()->create_string(base, folder);
	}

#define get_path(name) get_directories()->get_##name##()
}