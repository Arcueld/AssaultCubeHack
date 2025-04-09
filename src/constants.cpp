#include "constants.h"
#include "offset.h"

HMODULE hModule = GetModuleHandle(nullptr);
ULONG_PTR exeBaseAddress = (ULONG_PTR)GetModuleHandle(nullptr);
Player* pPlayer = *(Player**)(exeBaseAddress + selfPlayerOffset);

EntityList** ppEntityList = (EntityList**)(exeBaseAddress + entityListOffset);
int playerNums = *(int*)(exeBaseAddress + playerNumsOffset);

void updateConstants() {
	pPlayer = *(Player**)(exeBaseAddress + selfPlayerOffset);

	ppEntityList = (EntityList**)(exeBaseAddress + entityListOffset);
	playerNums = *(int*)(exeBaseAddress + playerNumsOffset);
}