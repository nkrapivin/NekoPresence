/*
	It's the NekoPresence source itself, yay.
*/

// TO BUILD THIS EXTENSION ON LINUX REPLACE #include "pch.h" WITH
#include <cstddef>
#include <cstring>
//#include "pch.h"
#include "discord_rpc.h"
#include "discord_register.h"

#if !defined( _MSC_VER)
#define EXPORTED_FN __attribute__((visibility("default")))
#else
#define EXPORTED_FN __declspec(dllexport)
#define snprintf sprintf_s
#define strdup _strdup
#endif

// For GM:S bool -> double
#define GM_TRUE (1.0)
#define GM_FALSE (0.0)

extern "C" {
	static bool np_Initialized = false;

	static char* np_small_image_text = NULL;
	static char* np_large_image_text = NULL;
	static double np_instance = 0;

	void (*CreateAsynEventWithDSMap)(int, int) = NULL;
	int (*CreateDsMap)(int _num, ...) = NULL;
	bool (*DsMapAddDouble)(int _index, char* _pKey, double value) = NULL;
	bool (*DsMapAddString)(int _index, char* _pKey, char* pVal) = NULL;
	const int EVENT_OTHER_SOCIAL = 70;

	EXPORTED_FN void RegisterCallbacks(char* arg1, char* arg2, char* arg3, char* arg4)
	{
		void (*CreateAsynEventWithDSMapPtr)(int, int) = (void (*)(int, int))(arg1);
		int(*CreateDsMapPtr)(int _num, ...) = (int(*)(int _num, ...)) (arg2);
		CreateAsynEventWithDSMap = CreateAsynEventWithDSMapPtr;
		CreateDsMap = CreateDsMapPtr;

		bool (*DsMapAddDoublePtr)(int _index, char* _pKey, double value) = (bool(*)(int, char*, double))(arg3);
		bool (*DsMapAddStringPtr)(int _index, char* _pKey, char* pVal) = (bool(*)(int, char*, char*))(arg4);

		DsMapAddDouble = DsMapAddDoublePtr;
		DsMapAddString = DsMapAddStringPtr;
	}

	// Init function.
	EXPORTED_FN double np_initdll(void) {
		np_Initialized = true;
		return GM_TRUE;
	}

	// Shutdown function.
	EXPORTED_FN double np_shutdown(void) {
		if (!np_Initialized) return GM_FALSE;
		Discord_Shutdown();
		return GM_TRUE;
	}

#pragma region // Callbacks.
	void handleDiscordReady(const DiscordUser *request) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, (char*)"event_type", (char*)"DiscordReady");
		DsMapAddString(ds_map, (char*)"user_id", (char*)request->userId);
		DsMapAddString(ds_map, (char*)"username", (char*)request->username);
		DsMapAddString(ds_map, (char*)"discriminator", (char*)request->discriminator);
		DsMapAddString(ds_map, (char*)"avatar", (char*)request->avatar);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordError(int errorCode, const char *message) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, (char*)"event_type", (char*)"DiscordError");
		DsMapAddString(ds_map, (char*)"error_message", (char*)message);
		DsMapAddDouble(ds_map, (char*)"error_code", errorCode);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordDisconnected(int errorCode, const char* message) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, (char*)"event_type", (char*)"DiscordDisconnected");
		DsMapAddDouble(ds_map, (char*)"error_code", errorCode);
		DsMapAddString(ds_map, (char*)"error_message", (char*)message);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordJoinGame(const char *joinSecret) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, (char*)"event_type", (char*)"DiscordJoinGame");
		DsMapAddString(ds_map, (char*)"join_secret", (char*)joinSecret);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordSpectateGame(const char *spectateSecret) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, (char*)"event_type", (char*)"DiscordSpectateGame");
		DsMapAddString(ds_map, (char*)"spectate_secret", (char*)spectateSecret);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordJoinRequest(const DiscordUser *request) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, (char*)"event_type", (char*)"DiscordJoinRequest");
		DsMapAddString(ds_map, (char*)"user_id", (char*)request->userId);
		DsMapAddString(ds_map, (char*)"username", (char*)request->username);
		DsMapAddString(ds_map, (char*)"discriminator", (char*)request->discriminator);
		DsMapAddString(ds_map, (char*)"avatar", (char*)request->avatar);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}
#pragma endregion

	EXPORTED_FN double np_initdiscord(char* client_id, double autoRegister, char* steam_id) {
		if (!np_Initialized) return GM_FALSE;

		DiscordEventHandlers handlers;
		memset(&handlers, 0, sizeof(handlers));

		// Define callbacks.
		handlers.ready = handleDiscordReady;
		handlers.errored = handleDiscordError;
		handlers.disconnected = handleDiscordDisconnected;
		handlers.joinGame = handleDiscordJoinGame;
		handlers.spectateGame = handleDiscordSpectateGame;
		handlers.joinRequest = handleDiscordJoinRequest;

		// Do the stuff...
		Discord_Initialize(client_id, &handlers, (int)autoRegister, steam_id);

		return GM_TRUE;
	}

	EXPORTED_FN double np_update() {
		if (!np_Initialized) return GM_FALSE;

		Discord_RunCallbacks();
		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence_more(char* small_image_text, char* large_image_text, double instance) {
		if (!np_Initialized) return GM_FALSE;

		// For some reason pointers to *_image_text screw up, so I'm using strdup to copy them.
		np_small_image_text = strdup(small_image_text);
		np_large_image_text = strdup(large_image_text);
		np_instance = instance;

		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence(char* state, char* details, char* large_image_key, char* small_image_key) {
		if (!np_Initialized) return GM_FALSE;

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));
		discordPresence.state = state;
		discordPresence.details = details;
		discordPresence.largeImageKey = large_image_key;
		discordPresence.smallImageKey = small_image_key;
		discordPresence.largeImageText = np_large_image_text;
		discordPresence.smallImageText = np_small_image_text;
		discordPresence.instance = (int8_t)np_instance;
		Discord_UpdatePresence(&discordPresence);
		return GM_TRUE;
	}
}