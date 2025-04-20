#include "Radar.h"
#include <iostream>





namespace Radar {
	void drawRadar(){

		DWORD oldProtection = NULL;

		memcpy(g_ori_Radar, (LPVOID)RadarPatchAddr, sizeof(g_ori_Radar));
	
		std::cout << RadarPatchAddr << std::endl;
		VirtualProtect((LPVOID)RadarPatchAddr, PAGE_SIZE, PAGE_READWRITE, &oldProtection);
		unsigned char buf[] = {0xe9,0x12,0x34,0x56,0x78,0x90};
		ULONG offset = RadarTargetAddr - (RadarPatchAddr + 5);
		memcpy(&buf[1], &offset, 4);
		memcpy((LPVOID)RadarPatchAddr, buf, sizeof(buf));
		VirtualProtect((LPVOID)RadarPatchAddr, PAGE_SIZE, oldProtection, &oldProtection);
	}
	void drawMiniMapRadar(){
		static bool saved = false;

		DWORD oldProtection = NULL;

		memcpy(g_ori_miniRadar, (LPVOID)miniMapRadarPatchAddr, sizeof(g_ori_miniRadar));

		VirtualProtect((LPVOID)miniMapRadarPatchAddr, PAGE_SIZE, PAGE_READWRITE, &oldProtection);
		unsigned char buf[] = {0xe9,0x12,0x34,0x56,0x78,0x90};
		ULONG offset = miniMapRadarTargetAddr - (miniMapRadarPatchAddr + 5);
		memcpy(&buf[1], &offset, 4);
		memcpy((LPVOID)miniMapRadarPatchAddr, buf, sizeof(buf));
		VirtualProtect((LPVOID)miniMapRadarPatchAddr, PAGE_SIZE, oldProtection, &oldProtection);
	}
	void unpatchRadar(){
		DWORD oldProtection = NULL;
		VirtualProtect((LPVOID)RadarPatchAddr, PAGE_SIZE, PAGE_READWRITE, &oldProtection);
		memcpy((LPVOID)RadarPatchAddr, g_ori_Radar, sizeof(g_ori_Radar));
		VirtualProtect((LPVOID)RadarPatchAddr, PAGE_SIZE, oldProtection, &oldProtection);
	}
	void unpatchMiniMapRadar(){
		DWORD oldProtection = NULL;
		VirtualProtect((LPVOID)miniMapRadarPatchAddr, PAGE_SIZE, PAGE_READWRITE, &oldProtection);
		memcpy((LPVOID)miniMapRadarPatchAddr, g_ori_miniRadar, sizeof(g_ori_miniRadar));
		VirtualProtect((LPVOID)miniMapRadarPatchAddr, PAGE_SIZE, oldProtection, &oldProtection);
	}
}