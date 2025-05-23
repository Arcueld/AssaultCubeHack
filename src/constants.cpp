#include "constants.h"
#include "offset.h"

HMODULE hModule = GetModuleHandle(nullptr);
ULONG_PTR exeBaseAddress = (ULONG_PTR)GetModuleHandle(nullptr);
Player* pPlayer = *(Player**)(exeBaseAddress + selfPlayerOffset);

EntityList** ppEntityList = (EntityList**)(exeBaseAddress + entityListOffset);
int playerNums = *(int*)(exeBaseAddress + playerNumsOffset);
float* viewMatrix = (float*)(exeBaseAddress + viewMatrixOffset);

_wglSwapBuffers o_wglSwapBuffers =(_wglSwapBuffers)GetProcAddress(LoadLibraryA("opengl32.dll"), "wglSwapBuffers");
_SDL_SetRelativeMouseMode o_SDL_SetRelativeMouseMode =(_SDL_SetRelativeMouseMode)GetProcAddress(LoadLibraryA("SDL2.dll"), "SDL_SetRelativeMouseMode");

ULONG_PTR RadarPatchAddr = ULONG_PTR(exeBaseAddress + mapRadarPatchOffset);
ULONG_PTR RadarTargetAddr = ULONG_PTR(exeBaseAddress + mapRadarTargetOffset);
ULONG_PTR miniMapRadarPatchAddr = ULONG_PTR(exeBaseAddress + miniMapRadarPatchOffset);
ULONG_PTR miniMapRadarTargetAddr = ULONG_PTR(exeBaseAddress + miniMapRadarTargetOffset);
LPVOID g_ori_Radar = malloc(6);
LPVOID g_ori_miniRadar = malloc(6);

void updateConstants() {
	pPlayer = *(Player**)(exeBaseAddress + selfPlayerOffset);

	ppEntityList = (EntityList**)(exeBaseAddress + entityListOffset);
	playerNums = *(int*)(exeBaseAddress + playerNumsOffset);
}