#pragma once
#include <Windows.h>
#include "genCode.h"


EXTERN_C HMODULE hModule;
EXTERN_C ULONG_PTR exeBaseAddress;
EXTERN_C Player* pPlayer;
EXTERN_C EntityList** ppEntityList;
EXTERN_C int playerNums;

void updateConstants();