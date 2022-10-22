#pragma once



//void EOS_CALL cb(const EOS_Auth_LoginCallbackInfo* Data);
void net_init();
void net_update();
void net_stat_send_endscore(int val);
void net_login();
void net_shutdown();
