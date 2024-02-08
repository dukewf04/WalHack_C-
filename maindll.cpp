#define _CRT_NONSTDC_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <d3dx9.h>
#include "BaseClasses.h"
#include "Functions.h"
#include "cHook.h"

#include "MinHook.h"
#pragma comment (lib, "MinHook.x86.lib")
#pragma comment (lib, "MinHook.x64.lib")

typedef INT(WINAPI* EndSceenFn)(LPDIRECT3DDEVICE9);
EndSceenFn pEndSceen;

//typedef bool (WINAPI *BITBLT)(HDC, int, int, int, int, HDC, int, int, DWORD);
//BITBLT OrigBitBlt = NULL;

//BOOL WINAPI DetourBitBlt(HDC hdc, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop)
//{
	//Beep(900, 100);
	//Beep(900, 100);
	//return (*OrigBitBlt)(hdc, x, y, cx, cy, hdcSrc, x1, y1, rop);
//	return false;
//}

HRESULT WINAPI myEndSceen(LPDIRECT3DDEVICE9 m_pDevice)
{
  // Снимок экрана
//	if (GetAsyncKeyState(VK_INSERT) && 1) {
//	    screenCapturePart(1, 1, 1919, 1079, "Kurva.png");
//	}
	bool f = 1;
	// Активация чита
	if (GetAsyncKeyState(VK_NUMPAD0) && 1) { Beep(600, 100); HERASE = true; }

	// Силуэты
	if (GetAsyncKeyState(VK_NUMPAD1) && 1) { Beep(600, 100); func.bWH = true; }
	if (GetAsyncKeyState(VK_NUMPAD4) && 1) { Beep(400, 100); func.bWH = false; }



	if (HERASE)	create_silhouettes();
	return pEndSceen(m_pDevice);
}
cVmtHook* vmthook = new cVmtHook;

void hooking()
{
	for (;;)
	{
		Sleep(31000);
		if (SSystemGlobalEnvironment::GetInstance()->pRenderer()->GetDirectDevice())
		{
			vmthook->bInitialize((PDWORD64*)SSystemGlobalEnvironment::GetInstance()->pRenderer()->GetDirectDevice());
			pEndSceen = (EndSceenFn)vmthook->dwHookMethod((DWORD64)myEndSceen, 42); 

			Beep(600, 100);
			Beep(600, 100);
			Beep(600, 100);
			break;
		}
	}
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserv)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
	//	while (MH_Initialize() != MH_OK);
	//	while (MH_CreateHook(&BitBlt, &DetourBitBlt, reinterpret_cast<LPVOID*>(&OrigBitBlt)) != MH_OK);
	//	while (MH_EnableHook(MH_ALL_HOOKS) != MH_OK);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hooking, 0, 0, 0);
		return TRUE;

	case DLL_PROCESS_DETACH:
		FreeLibraryAndExitThread(hModule, 0);
		return TRUE;
	}
	return FALSE;
}

/*
BOOL APIPRIVATE DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserv)
{
	UNREFERENCED_PARAMETER(lpReserv);
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		while (MH_Initialize() != MH_OK);
		while (MH_CreateHook(&BitBlt, &DetourBitBlt, reinterpret_cast<LPVOID*>(&OrigBitBlt)) != MH_OK);
		while (MH_EnableHook(MH_ALL_HOOKS) != MH_OK);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)hooking, 0, 0, 0);
		return TRUE;

	case DLL_PROCESS_DETACH:
		FreeLibraryAndExitThread(hModule, 0);
		return TRUE;
	}
	return FALSE;
}
*/