// ==========================================================
// Project: XSpotify
// 
// Component: XSpotify.dll
//
// Purpose: Disable auto updates
//          
// Initial author: Meik Wochnik
//
// Started: 09.02.2020
// ==========================================================

#pragma once

#include "hooking.h"
#include "hooks.h"

bool ParseUpdateUrl_hk(int a1, int a2)
{
	std::cout << "ParseUpdateUrl called!" << std::endl;
	return 0;
}

void XPatchAutoUpdates()
{
	Hook::InstallCall((void*)0x56CE1A, ParseUpdateUrl_hk);
}