#pragma once
#include <Windows.h>

extern bool showMenu;


namespace Menu{
	void toggleMenu();
	void init();
	void startRender();
	void render();
	void endRender();
	BOOL __stdcall newSwapBuffers(HDC hdc);
	void cleanUp();
}