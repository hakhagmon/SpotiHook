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

/*void __declspec(naked) CmdAddText_stub(int a1, int a2, const char* fmt, const char* dummy0, int dummy1, int dummy2, int dummy3, int dummy4, int dummy5)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		push    esi
		mov     esi, [ebp + 8]
		lea     eax, [ebp + 20]
		push    CmdAddText_back
		retn
	}
}

void CmdAddText_hk(int a1, int a2, const char* fmt, const char* dummy0, int dummy1, int dummy2, int dummy3, int dummy4, int dummy5)
{
	if (fmt[8] == char(116) && fmt[9] == char(114) && fmt[10] == char(97) && fmt[11] == char(99) && fmt[12] == char(107) && fmt[13] == char(95) && fmt[14] == char(117) && fmt[15] == char(114) && fmt[16] == char(105))
	{
		if (dummy0[8] == char(97) && dummy0[9] == char(100)) //ad
		{
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
			std::cout << "[Ad detected]: " << dummy0 << std::endl;
			SetConsoleTextAttribute(hConsole, 10);
			__position = 29000;
		}
		else if (dummy0[8] == char(116) && dummy0[9] == char(114)) //tr
		{
			std::cout << "Song: " << dummy0 << std::endl;

			__position = 0;
		}
	}

	CmdAddText_stub(a1, a2, fmt, dummy0, dummy1, dummy2, dummy3, dummy4, dummy5);
} */

void __declspec(naked) __fastcall OpenTrack_stub(void* _this, DWORD edx, int a2, int a3, int a4, __int64 position, int a6, int a7)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		push - 1
		push    119762Ah
		push    OpenTrack_back
		retn
	}
}

void __fastcall OpenTrack_hk(void* _this, DWORD edx, int a2, int a3, int a4, __int64 position, int a6, int a7)
{
	OpenTrack_stub(_this, edx, a2, a3, a4, __position, a6, a7);
}

void __declspec(naked) __fastcall CreateTrack_stub(void* _this, DWORD edx, int a2, int a3, double speed, int a5, int a6, int flag, int a8, int a9)
{
	__asm
	{
		push    ebp
		mov     ebp, esp
		push - 1
		push    11971E1h
		push    CreateTrack_back
		retn
	}
}

void __fastcall CreateTrack_hk(void* _this, DWORD edx, int a2, int a3, double speed, int a5, int a6, int flag, int a8, int a9)
{
	double newspeed = 8;
	CreateTrack_stub(_this, edx, a2, a3, newspeed, a5, a6, flag, a8, a9);
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