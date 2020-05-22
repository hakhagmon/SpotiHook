#pragma once
#include "imports.h"

//1.1.33.569.gced9e0f5
DWORD CREATETRACK = 0xC76EB0;
DWORD CLOSETRACK = 0xC6F230;
DWORD OPENTRACK = 0xC77B30;
DWORD CMDADDTEXT = 0x109ED50;
DWORD SETBITRATE = 0x5AD870;
DWORD AES_SET_ENCRYPT_KEY = 0x107E6D0;
DWORD ENABLESKIPS = 0x72BB80;
DWORD GETFILEID = 0x6E8420;
DWORD SIGNAL = 0xB69E80;


//CreateTrack
//typedef void(__fastcall* CreateTrack_t)(int a2, int a3, float a1, int a4, double speed, int a6, int a7, int flag, int a8, int a9);
typedef void(__fastcall* CreateTrack_t)(float a1f, int a2, int a3, int a4, double speed, int a6, int a7, int flag, int a8, int a9);
extern CreateTrack_t CreateTrack;
CreateTrack_t CreateTrack = (CreateTrack_t)CREATETRACK;

//CloseTrack
typedef void(__thiscall* CloseTrack_t)(void* _this, int a2, int a3, int a4, int a5, int a6);
extern CloseTrack_t CloseTrack;
CloseTrack_t CloseTrack = (CloseTrack_t)CLOSETRACK;

//OpenTrack
//todo: thiscall could be wrong
typedef  void(__thiscall* OpenTrack_t)(void* _this, int a2, int a3, int a4, __int64 position, int a6, int a7);
extern OpenTrack_t OpenTrack;
OpenTrack_t OpenTrack = (OpenTrack_t)OPENTRACK;

//CmdAddText
typedef void(__cdecl* CmdAddText_t)(int a1, int a2, int a3, int a4, int a5, const char* a6, const char* dummy0, int dummy1, int dummy2, int dummy3, int dummy4, int dummy5);
extern CmdAddText_t CmdAddText;
CmdAddText_t CmdAddText = (CmdAddText_t)CMDADDTEXT;

//SetBitrate
typedef void(__cdecl* SetBitrate_t)(int a1, int a2, int a3, int a4, int a5);
extern SetBitrate_t SetBitrate;
SetBitrate_t SetBitrate = (SetBitrate_t)SETBITRATE;

//EnableSkips
typedef void(__thiscall* EnableSkips_t)(void* _this);
extern EnableSkips_t EnableSkips;
EnableSkips_t EnableSkips = (EnableSkips_t)ENABLESKIPS;

//GetFileID
typedef void(__thiscall* GetFileID_t)(void* _this, int* a2, int a3);
extern GetFileID_t GetFileID;
GetFileID_t GetFileID = (GetFileID_t)GETFILEID;

//todo: broken?
//aes_set_encrypt_key
typedef void(__cdecl* aes_set_encrypt_key_t)(unsigned int* key, DWORD* userKey, int bits, const char a4); // char a4 ??
extern aes_set_encrypt_key_t aes_set_encrypt_key;
aes_set_encrypt_key_t aes_set_encrypt_key = (aes_set_encrypt_key_t)AES_SET_ENCRYPT_KEY;

//Signal
typedef void(__stdcall* Signal_t)(int a2, int a3);
extern Signal_t Signal;
Signal_t Signal = (Signal_t)SIGNAL;