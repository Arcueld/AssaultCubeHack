#include <iostream>
#include <iomanip>
#include <Windows.h>
#include "genCode.h"
#include "constants.h"
void hook() {
}

void console(HMODULE hModule) {
	AllocConsole();
	FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);
    freopen_s(&f, "CONIN$", "r", stdin);
	std::cout << "Console opened" << std::endl;
    while (true) {
        std::string input;
        std::cin >> input;
        if (input == "exit") {
            break;
        }
        if (input == "up") {
            pPlayer->posision.y += 5;
        }
        if (input == "down") {
            pPlayer->posision.y -= 5;
        }
        if (input == "print") {
            std::cout << "pPlayer" << std::hex << pPlayer << std::endl;
        }
        if(input == "nums") {
			std::cout << "Player nums: " << playerNums << std::endl;
		}
        if (input == "entities") {
            EntityList* pEntityList = *(ppEntityList);
            std::cout << std::hex << pEntityList->players << std::endl;
            for(int i =1; i< playerNums; i++) {
				if (pEntityList->players[i] != nullptr) {
                    std::cout << std::dec << std::fixed << std::setprecision(2);
					std::cout << "Entity " << i << ": " << pEntityList->players[i]->posision.z << std::endl;
				}
			}
        }
    }
    FreeConsole();
    HWND hwndConsole = GetConsoleWindow();
    if (hwndConsole != nullptr) {
        PostMessage(hwndConsole, WM_CLOSE, 0, 0);
    }
    FreeLibraryAndExitThread(hModule, 0);
}



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {

        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)console, hModule, 0, nullptr);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hook, nullptr, 0, nullptr);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

