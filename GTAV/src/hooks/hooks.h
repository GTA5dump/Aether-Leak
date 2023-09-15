#pragma once
#include "util/hooking/hooking.h"
#include "rage/classes/scrNativeCallContext.h"
#include "rage/classes/scrThread.h"
#include "rage/classes/scrProgram.h"
#include "rage/classes/WebConstuction.h"
namespace base::hooks {
	bool patterns();
	bool bypass();
	bool hooks();

	void stat_get_int(rage::scrNativeCallContext* context);
	void set_current_ped_weapon(rage::scrNativeCallContext* context);
	void disable_control_action(rage::scrNativeCallContext* context);
	void network_session_host(rage::scrNativeCallContext* context);
	void set_player_name(rage::scrNativeCallContext* context);


	uint64_t scriptVmHook(void* stack, int64_t** globals, rage::scrProgram* program, rage::scrThreadContext* ctx);
	inline decltype(&scriptVmHook) ogScriptVmHook;

	void* getFontIdHook(uint64_t font, int id);
	inline decltype(&getFontIdHook) ogGetFontIdHook;

	int ntQueryVirtualMemoryHook(void* _this, HANDLE handle, PVOID base_addr, int info_class, MEMORY_BASIC_INFORMATION* info, int size, size_t* return_len);
	
	void queueDependencyHook(void* dependency);
	inline decltype(&queueDependencyHook) ogQueueDependencyHook;

	void sendNetworkEventHook(uint64_t net_table, uint64_t event);
	inline decltype(&sendNetworkEventHook) ogSendNetworkEventHook;

	uint64_t formatEngineStringHook(uint64_t rcx, uint64_t rdx, uint32_t r8d, const char* r9, uint32_t stack);
	inline decltype(&formatEngineStringHook) ogFormatEngineStringHook;

	int getEngineStringLineCountHook(uint64_t rcx, const char* rdx, float xmm2, uint64_t r9, bool stack);
	inline decltype(&getEngineStringLineCountHook) ogGetEngineStringLineCountHook;

	bool allowWeaponsInVehicleHook(int64_t unk, int weaponinfo_group);
	inline decltype(&allowWeaponsInVehicleHook) ogAllowWeaponsInVehicleHook;

	uint64_t addEventToListHook(uint64_t event_pool);
	inline decltype(&addEventToListHook) ogAddEventToListHook;

	bool sendHTTPRequestHook(CHttpRequest* request);
	inline decltype(&sendHTTPRequestHook) ogSendHTTPRequestHook;

	LPVOID convertThreadToFiberHook(LPVOID param);
	inline decltype(&convertThreadToFiberHook) ogConvertThreadToFiberHook;

	void syncPedOrientationHook(uint64_t rcx, uint64_t rdx);
	inline decltype(&syncPedOrientationHook) ogSyncPedOrientationHook;

	void syncVehicleOrientationHook(uint64_t rcx, uint64_t* rdx);
	inline decltype(&syncVehicleOrientationHook) ogsyncVehicleOrientationHook;

	void receivedNetworkEventHook(void* _this, CNetGamePlayer* sender, CNetGamePlayer* receiver, uint16_t event_id, int event_index, int event_bitset, uint32_t buffer_size, rage::datBitBuffer* buffer);
	inline decltype(&receivedNetworkEventHook) ogReceivedNetworkEventHook;
}