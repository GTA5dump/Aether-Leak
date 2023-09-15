#pragma once
#include "pch.h"
#include "hooks/patterns.h"

namespace Util {
	inline std::string GetDocumentsPath() {
		wchar_t folder[1024];
		HRESULT result = SHGetFolderPathW(0, CSIDL_MYDOCUMENTS, 0, 0, folder);
		if (SUCCEEDED(result)) {
			char string[1024];
			wcstombs(string, folder, 1023);
			return string;
		}
		else return "";
	}

	inline bool is_key_pressed(std::uint16_t key) {
		if (GetForegroundWindow() == g_window) {
			if (GetAsyncKeyState(key) & 0x8000) {
				return true;
			}
		}
		return false;
	}
}
