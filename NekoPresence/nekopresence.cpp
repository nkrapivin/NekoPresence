/*
	It's the NekoPresence source itself, yay.
*/

#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <string>
#include <cmath>
#include "discord_rpc.h"
#include "discord_register.h"

#if !defined( _MSC_VER)
#define EXPORTED_FN __attribute__((visibility("default")))
#else
#define EXPORTED_FN __declspec(dllexport)
#define WIN32_LEAN_AND_MEAN
#endif

// For GM:S bool -> double
#define GM_TRUE (1.0)
#define GM_FALSE (0.0)

extern "C" {
	static bool np_Initialized = false;

	static std::string np_small_image_text{};
	static std::string np_large_image_text{};

	static std::string np_join_secret{};
	static std::string np_spectate_secret{};
	static std::string np_match_secret{};

	static int np_party_size = 0;
	static int np_party_max = 0;
	static int np_party_privacy = 0;
	static std::string np_party_id{};

	static int64_t np_start_timestamp = 0;
	static int64_t np_end_timestamp = 0;

	static int8_t np_instance = 0;

	void (*CreateAsynEventWithDSMap)(int, int) = NULL;
	int (*CreateDsMap)(int _num, ...) = NULL;
	bool (*DsMapAddDouble)(int _index, const char* _pKey, double value) = NULL;
	bool (*DsMapAddString)(int _index, const char* _pKey, const char* pVal) = NULL;
	const int EVENT_OTHER_SOCIAL = 70;

	EXPORTED_FN void RegisterCallbacks(char* arg1, char* arg2, char* arg3, char* arg4)
	{
		void (*CreateAsynEventWithDSMapPtr)(int, int) = (void (*)(int, int))(arg1);
		int(*CreateDsMapPtr)(int _num, ...) = (int(*)(int _num, ...)) (arg2);
		CreateAsynEventWithDSMap = CreateAsynEventWithDSMapPtr;
		CreateDsMap = CreateDsMapPtr;

		bool (*DsMapAddDoublePtr)(int _index, const char* _pKey, double value) = (bool(*)(int, const char*, double))(arg3);
		bool (*DsMapAddStringPtr)(int _index, const char* _pKey, const char* pVal) = (bool(*)(int, const char*, const char*))(arg4);

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
		DsMapAddString(ds_map, "event_type", "DiscordReady");
		DsMapAddString(ds_map, "user_id", request->userId);
		DsMapAddString(ds_map, "username", request->username);
		DsMapAddString(ds_map, "discriminator", request->discriminator);
		DsMapAddString(ds_map, "avatar", request->avatar);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordError(int errorCode, const char *message) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, "event_type", "DiscordError");
		DsMapAddString(ds_map, "error_message", message);
		DsMapAddDouble(ds_map, "error_code", errorCode);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordDisconnected(int errorCode, const char* message) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, "event_type", "DiscordDisconnected");
		DsMapAddDouble(ds_map, "error_code", errorCode);
		DsMapAddString(ds_map, "error_message", message);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordJoinGame(const char *joinSecret) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, "event_type", "DiscordJoinGame");
		DsMapAddString(ds_map, "join_secret", joinSecret);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordSpectateGame(const char *spectateSecret) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, "event_type", "DiscordSpectateGame");
		DsMapAddString(ds_map, "spectate_secret", spectateSecret);
		CreateAsynEventWithDSMap(ds_map, EVENT_OTHER_SOCIAL);
	}

	void handleDiscordJoinRequest(const DiscordUser *request) {
		int ds_map = CreateDsMap(0);
		DsMapAddString(ds_map, "event_type", "DiscordJoinRequest");
		DsMapAddString(ds_map, "user_id", request->userId);
		DsMapAddString(ds_map, "username", request->username);
		DsMapAddString(ds_map, "discriminator", request->discriminator);
		DsMapAddString(ds_map, "avatar", request->avatar);
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
		Discord_Initialize(client_id, &handlers, static_cast<int>(std::floor(autoRegister)), steam_id);

		return GM_TRUE;
	}

	EXPORTED_FN double np_update() {
		if (!np_Initialized) return GM_FALSE;

		Discord_RunCallbacks();
		return GM_TRUE;
	}

	EXPORTED_FN double np_registergame(char* client_id, char* command) {
		if (!np_Initialized) return GM_FALSE;
		Discord_Register(client_id, command);
		return GM_TRUE;
	}

	EXPORTED_FN double np_registergame_steam(char* client_id, char* steam_id) {
		if (!np_Initialized) return GM_FALSE;
		Discord_RegisterSteamGame(client_id, steam_id);
		return GM_TRUE;
	}

	EXPORTED_FN double np_respond(char* user_id, double reply) {
		if (!np_Initialized) return GM_FALSE;
		Discord_Respond(user_id, static_cast<int>(std::floor(reply)));
		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence_secrets(char* matchSecret, char* spectateSecret, char* joinSecret) {
		if (!np_Initialized) return GM_FALSE;
		np_join_secret = joinSecret;
		np_spectate_secret = spectateSecret;
		np_match_secret = matchSecret;
		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence_clearsecrets(void) {
		if (!np_Initialized) return GM_FALSE;
		np_join_secret.clear();
		np_match_secret.clear();
		np_spectate_secret.clear();
		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence_partyparams(double partySize, double partyMax, char* partyId, double partyPrivacy) {
		if (!np_Initialized) return GM_FALSE;
		np_party_size = static_cast<int>(std::floor(partySize));
		np_party_max = static_cast<int>(std::floor(partyMax));
		np_party_id = partyId;
		np_party_privacy = static_cast<int>(std::floor(partyPrivacy));
		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence_more(char* small_image_text, char* large_image_text, double instance) {
		if (!np_Initialized) return GM_FALSE;

		np_small_image_text = small_image_text;
		np_large_image_text = large_image_text;
		np_instance = (instance > 0.5) ? 1 : 0;

		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence_timestamps(double startTimestamp, double endTimestamp, double is_unix_ts) {
		if (!np_Initialized) return GM_FALSE;

		if (is_unix_ts > 0.5) {
			np_start_timestamp = static_cast<int64_t>(startTimestamp);
			np_end_timestamp = static_cast<int64_t>(endTimestamp);
		}
		else {
			np_start_timestamp = static_cast<int64_t>((startTimestamp - 25569) * 86400);
			np_end_timestamp = static_cast<int64_t>((endTimestamp - 25569) * 86400);
		}

		return GM_TRUE;
	}

	EXPORTED_FN double np_setpresence(char* state, char* details, char* large_image_key, char* small_image_key) {
		if (!np_Initialized) return GM_FALSE;

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));

		discordPresence.state = state;
		discordPresence.details = details;
		if (np_start_timestamp > 0) discordPresence.startTimestamp = np_start_timestamp;
		if (np_end_timestamp > 0) discordPresence.endTimestamp = np_end_timestamp;
		discordPresence.largeImageKey = large_image_key;
		discordPresence.largeImageText = np_large_image_text.c_str();
		discordPresence.smallImageKey = small_image_key;
		discordPresence.smallImageText = np_small_image_text.c_str();
		discordPresence.partyId = np_party_id.c_str();
		discordPresence.partySize = np_party_size;
		discordPresence.partyMax = np_party_max;
		discordPresence.partyPrivacy = np_party_privacy;
		discordPresence.matchSecret = np_match_secret.c_str();
		discordPresence.joinSecret = np_join_secret.c_str();
		discordPresence.spectateSecret = np_spectate_secret.c_str();
		discordPresence.instance = np_instance;

		Discord_UpdatePresence(&discordPresence);
		return GM_TRUE;
	}

	EXPORTED_FN double np_clearpresence(void) {
		if (!np_Initialized) return GM_FALSE;
		Discord_ClearPresence();
		np_join_secret.clear();
		np_match_secret.clear();
		np_spectate_secret.clear();
		np_small_image_text.clear();
		np_large_image_text.clear();
		np_party_id.clear();
		np_party_size = 0;
		np_party_max = 0;
		np_party_privacy = 0;
		np_instance = 0;
		np_start_timestamp = 0;
		np_end_timestamp = 0;
		return GM_TRUE;
	}
}