#pragma once
#include <Windows.h>
#include "genCode.h"


EXTERN_C HMODULE hModule;
EXTERN_C ULONG_PTR exeBaseAddress;
EXTERN_C Player* pPlayer;
EXTERN_C EntityList** ppEntityList;
EXTERN_C int playerNums;
EXTERN_C float* viewMatrix;

typedef BOOL(__stdcall* _wglSwapBuffers)(HDC hdc);
EXTERN_C _wglSwapBuffers o_wglSwapBuffers;

typedef int(__fastcall* _SDL_SetRelativeMouseMode)(int mode);
EXTERN_C _SDL_SetRelativeMouseMode o_SDL_SetRelativeMouseMode;
void updateConstants();

