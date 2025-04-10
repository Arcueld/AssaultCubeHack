#include <iostream>
#include <iomanip>
#include <Windows.h>
#include "genCode.h"
#include "constants.h"
#include "esp.h"
#include <detours/detours.h>
#include "menu.h"

HANDLE aimbotThread = NULL;
HANDLE hookThread = NULL;
volatile bool g_Running = true;

void aimbot() {
    while (g_Running) {
        updateConstants();
        ESP::aimbot();
        Sleep(60);
        if (GetAsyncKeyState(VK_INSERT)) {
            Menu::toggleMenu();
        }
    }
}

void hook() {
    Sleep(1000);
    DisableThreadLibraryCalls(hModule);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(&(PVOID&)o_wglSwapBuffers, Menu::newSwapBuffers);

    DetourTransactionCommit();
}

void unhook(){
    Sleep(1000);
    DisableThreadLibraryCalls(hModule);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID &)o_wglSwapBuffers, Menu::newSwapBuffers);
    DetourTransactionCommit();
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
            pPlayer->position.z += 5;
        }
        if (input == "down") {
            pPlayer->position.z -= 5;
        }
        if (input == "pPlayer") {
            std::cout << std::hex << pPlayer << std::endl;
            std::cout << pPlayer->name << std::endl;
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
					std::cout << "Entity " << i << ": " << pEntityList->players[i] << std::endl;
				}
			}
        }if (input == "aimbot") {
            Player* nearestPlayer = ESP::getNearestPlayer();
            std::cout << std::dec << std::fixed << std::setprecision(2);
            if (nearestPlayer != nullptr) {
				std::cout << "Nearest player: " << nearestPlayer << std::endl;
				std::cout << "Position: " << nearestPlayer->position.x << ", " << nearestPlayer->position.y << ", " << nearestPlayer->position.z << std::endl;
				std::cout << "HP: " << nearestPlayer->HP << std::endl;
			} else {
				std::cout << "No valid target found." << std::endl;
			}
        }
    }
    g_Running = false;
    Sleep(200); // wait for threads to finish
    unhook();

    Menu::cleanUp();

    if(aimbotThread != NULL) {
		TerminateThread(aimbotThread, 0);
		CloseHandle(aimbotThread);
	}if (hookThread != NULL) {
        TerminateThread(hookThread, 0);
        CloseHandle(hookThread);
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
        aimbotThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)aimbot, nullptr, 0, nullptr);
        hookThread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)hook, nullptr, 0, nullptr);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

