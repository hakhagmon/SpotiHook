// ==========================================================
// Project: XSpotify
// 
// Component: XSpotify.dll
//
// Purpose: Blocking ads
//          
// Initial author: Meik Wochnik
//
// Started: 15.10.2019
// ==========================================================

#pragma once

#include "hooks.h"
#include "hooking.h"

__int64 __position;
DWORD CmdAddText_back = CMDADDTEXT + 10;
DWORD OpenTrack_back = OPENTRACK + 10;
DWORD CreateTrack_back = CREATETRACK + 10;

void __declspec(naked) __fastcall OpenTrack_stub(void* _this, DWORD edx, int a2, int a3, int a4, __int64 position, int a6, int a7)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		push - 1
		push    119CF38h
		push    OpenTrack_back
		retn
	}
}

void __fastcall OpenTrack_hk(void* _this, DWORD edx, int a2, int a3, int a4, __int64 position, int a6, int a7)
{
	OpenTrack_stub(_this, edx, a2, a3, a4, __position, a6, a7);
}

//todo:
void __declspec(naked) __fastcall CreateTrack_stub(float a1f, int a2, int a3, int a4, double speed, int a6, int a7, int flag, int a8, int a9)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		push - 1
		push    11A7D62h
		push    CreateTrack_back
		retn
	}
}

void __fastcall CreateTrack_hk(float a1f, int a2, int a3, int a4, double speed, int a6, int a7, int flag, int a8, int a9)
{
	double newspeed = 8;
	CreateTrack_stub(a1f, a2, a3, a4, newspeed, a6, a7, flag, a8, a9);
}

void __declspec(naked) __fastcall EnableSkips_hk(void* _this, DWORD edx)
{
	__asm
	{
		mov     eax, 1
		retn
	}
}

void XPatchAds()
{

	//Hook::InstallJmp(CmdAddText, CmdAddText_hk);
	Hook::InstallJmp(OpenTrack, OpenTrack_hk);
	Hook::InstallJmp(EnableSkips, EnableSkips_hk);
}