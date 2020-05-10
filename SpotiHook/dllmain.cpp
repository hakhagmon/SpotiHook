// dllmain.cpp : Definisce il punto di ingresso per l'applicazione DLL.
#pragma once
#include "imports.h"
#include "window_hook.h"
#include "render_proxy.h"
#include "overlay.h"
#include "menu.h"
#include "patchAds.h"
#include "patchAutoUpdates.h"
#include "patchDownloads.h"


void Init() {
    HWND hWnd = NULL;
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    printf("[<] Initializing...");
    while (!hWnd)
    {
        printf(".");
        hWnd = WindowHook();
        Sleep(5);
    }
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
    printf("\n[+] Found nVidia hWnd!\n");
    initD3D(hWnd);
    printf("[+] DirectX initialized!\n");
    zgui::functions.draw_line = line;
    zgui::functions.draw_rect = rect;
    zgui::functions.draw_filled_rect = filled_rect;
    zgui::functions.draw_text = text;
    zgui::functions.get_text_size = get_text_size;
    zgui::functions.get_frametime = get_frametime;
    printf("[+] GUI render proxy initialized!\n");
    //CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MsgLoop, 0, 0, 0);
    XPatchDownloads();
    XPatchAutoUpdates();
    XPatchAds();
    while (true)
        render_scene();
}

void render_scene()
{
    // clear the window alpha
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
    d3ddev->BeginScene();    // begins the 3D scene
    GUI::renderMenu();
    d3ddev->EndScene();    // ends the 3D scene
    d3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
    //Sleep(16);
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, 0, 0, 0);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

