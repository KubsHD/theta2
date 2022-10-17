#include "network.h"

#include "log.h"

#include <eos_init.h>
#include <eos_base.h>
#include <eos_common.h>
#include <eos_integratedplatform.h>
#include <eos_platform_prereqs.h>
#include <eos_stats.h>
#include <eos_auth.h>
#include <eos_ui.h>
#include <eos_integratedplatform.h>
#include <eos_stats.h>
#include <eos_logging.h>
#include <eos_connect.h>
#include <eos_sdk.h>

EOS_HPlatform platform = NULL;
EOS_HStats stats = NULL;
EOS_HAuth auth = NULL;

int net_ready = 0;

EOS_ProductUserId user_id;

void EOS_CALL auth_cb(const EOS_Auth_LoginCallbackInfo* Data);

void prompt_epic_login()
{
	EOS_Auth_LoginOptions log = {};
	EOS_Auth_Credentials creds = {};


	// try login from cache

	creds.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	creds.Type = EOS_ELoginCredentialType::EOS_LCT_AccountPortal;
	creds.ExternalType = EOS_EExternalCredentialType::EOS_ECT_EPIC;

	log.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	log.Credentials = &creds;
	log.ScopeFlags = (EOS_EAuthScopeFlags)(EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence);


	EOS_Auth_Login(auth, &log, NULL, &auth_cb);
}

void EOS_CALL connect_login_cb(const EOS_Connect_LoginCallbackInfo* data)
{
	//log_info("%s", EOS_EResult_ToString(data->ResultCode));
	user_id = data->LocalUserId;
}


void EOS_CALL auth_cb(const EOS_Auth_LoginCallbackInfo* Data)
{
	//todo: may be broken
	user_id = (EOS_ProductUserId)Data->LocalUserId;

	if (Data->ResultCode != EOS_EResult::EOS_Success)
	{
		prompt_epic_login();
	}

	//log_info("%s", EOS_EResult_ToString(Data->ResultCode));

	EOS_Auth_CopyUserAuthTokenOptions token_opts;
	token_opts.ApiVersion = EOS_AUTH_COPYIDTOKEN_API_LATEST;

	EOS_Auth_Token* tok;

	EOS_Auth_CopyUserAuthToken(auth, &token_opts, Data->LocalUserId, &tok);

	// credentials creation 

	EOS_Connect_Credentials cred;

	cred.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
	cred.Type = EOS_EExternalCredentialType::EOS_ECT_EPIC;
	cred.Token = tok->AccessToken;

	// login options

	EOS_Connect_LoginOptions opts_login;

	opts_login.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
	opts_login.UserLoginInfo = NULL;
	opts_login.Credentials = &cred;


	// login
	EOS_Connect_Login(EOS_Platform_GetConnectInterface(platform), &opts_login, NULL, &connect_login_cb);
}



void EOS_CALL token_cb(const EOS_Connect_CreateDeviceIdCallbackInfo* data)
{
	log_info("%s", EOS_EResult_ToString(data->ResultCode));

	if (data->ResultCode == EOS_EResult::EOS_DuplicateNotAllowed)
	{

	}


	// user info

	EOS_Connect_UserLoginInfo user;

	user.ApiVersion = EOS_CONNECT_USERLOGININFO_API_LATEST;
	user.DisplayName = "player1";



	// credentials creation 

	EOS_Connect_Credentials cred;

	cred.ApiVersion = EOS_CONNECT_CREDENTIALS_API_LATEST;
	cred.Type = EOS_EExternalCredentialType::EOS_ECT_DEVICEID_ACCESS_TOKEN;
	cred.Token = NULL;


	// login options

	EOS_Connect_LoginOptions opts_login;

	opts_login.ApiVersion = EOS_CONNECT_LOGIN_API_LATEST;
	opts_login.UserLoginInfo = &user;
	opts_login.Credentials = &cred;


	// login
	EOS_Connect_Login(EOS_Platform_GetConnectInterface(platform), &opts_login, NULL, &connect_login_cb);
}


void EOS_CALL log_cb(const EOS_LogMessage* Data)
{
	log_info("%s", Data->Message);
}

void net_init()
{
	EOS_EResult res;

	{
		EOS_InitializeOptions opts = {};

		opts.ApiVersion = EOS_INITIALIZE_API_LATEST;
		opts.ProductName = "Theta";
		opts.ProductVersion = "1.0.0";


		res = EOS_Initialize(&opts);
		if (res != EOS_EResult::EOS_Success)
		{
			log_info("[EOS SDK] Init Failed!");
			return;
		}

		EOS_Logging_SetCallback(&log_cb);
	}

	EOS_Platform_ClientCredentials cred;
	cred.ClientId = "xyza7891FiQnJw3EfxfC8jIVGtHmBHg7";
	cred.ClientSecret = "ods/eMm0oYE6/FlhIQVlQkKAWm+KTZgEzFZKjpo51/M";


	EOS_Platform_Options opts = {};


	opts.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	opts.Flags = EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D9 | EOS_PF_WINDOWS_ENABLE_OVERLAY_D3D10 | EOS_PF_WINDOWS_ENABLE_OVERLAY_OPENGL;
	opts.ProductId = "819a6d1d8b674c6daa38f5e119e528c0";
	opts.DeploymentId = "dd843eb27f9441a69fcf407e12762604";
	opts.SandboxId = "3e5765eca3974df087ffbd20b02a33c4";
	opts.bIsServer = EOS_FALSE;
	opts.ClientCredentials = cred;

	platform = EOS_Platform_Create(&opts);

	if (platform == NULL)
	{
		log_info("[EOS SDK] platform Failed!");
	}

	auth = EOS_Platform_GetAuthInterface(platform);
	stats = EOS_Platform_GetStatsInterface(platform);
}

void net_login()
{
	EOS_Auth_LoginOptions log = {};
	EOS_Auth_Credentials creds = {};



	// try login from cache

	creds.ApiVersion = EOS_AUTH_CREDENTIALS_API_LATEST;
	creds.Type = EOS_ELoginCredentialType::EOS_LCT_PersistentAuth;
	creds.ExternalType = EOS_EExternalCredentialType::EOS_ECT_EPIC;

	log.ApiVersion = EOS_AUTH_LOGIN_API_LATEST;
	log.Credentials = &creds;
	log.ScopeFlags = (EOS_EAuthScopeFlags)(EOS_EAuthScopeFlags::EOS_AS_BasicProfile | EOS_EAuthScopeFlags::EOS_AS_FriendsList | EOS_EAuthScopeFlags::EOS_AS_Presence);

	EOS_Auth_Login(auth, &log, NULL, &auth_cb);
}

void net_update()
{
	EOS_Platform_Tick(platform);
}

void EOS_CALL cb_stat(const EOS_Stats_IngestStatCompleteCallbackInfo* Data)
{
	log_info("%s", EOS_EResult_ToString(Data->ResultCode));
}

void net_stat_send_endscore(int val)
{

	EOS_Stats_IngestData dat;
	dat.ApiVersion = EOS_STATS_INGESTDATA_API_LATEST;
	dat.StatName = "GameScore";
	dat.IngestAmount = val;

	EOS_Stats_IngestStatOptions opts;
	opts.ApiVersion = EOS_STATS_INGESTSTAT_API_LATEST;
	opts.StatsCount = 1;
	opts.Stats = &dat;
	opts.TargetUserId = user_id;
	opts.LocalUserId = user_id;


	EOS_Stats_IngestStat(stats, &opts, NULL, &cb_stat);
}

void net_shutdown()
{
	EOS_Platform_Release(platform);
	EOS_Shutdown();
}