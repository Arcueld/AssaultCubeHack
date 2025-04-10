#include <iostream>
#include "menu.h"
#include "constants.h"
#include "gl/gl.h"
#include "gl/GLU.h"
#include "imgui/imgui_impl_opengl2.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui.h"
#include "settings.h"
#include "esp.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

const ImVec2 initWindowSize = ImVec2(400, 400);
bool showMenu = false;
bool initialized = false;
bool contextCreated = false;
HWND gameWindow;
HGLRC myContext;
HGLRC gameContext;
HDC g_HDC;
WNDPROC o_WndProc = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK newWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	if (showMenu) {
		
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam)) {
			return 0;
		}

		switch (msg) {
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_CHAR:
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
			return 0;
		}
	}
	return CallWindowProc(o_WndProc, hwnd, msg, wParam, lParam);
}

void Menu::toggleMenu(){
	showMenu = !showMenu;
	ImGuiIO& io = ImGui::GetIO();
	io.WantCaptureKeyboard =showMenu; // 拦截键鼠
	io.WantCaptureMouse = showMenu;
	io.MouseDrawCursor = showMenu; // 是否用imgui的鼠标
	o_SDL_SetRelativeMouseMode(!showMenu); // true 锁中心 false 随便动

}

void Menu::init() {
	ImGui::CreateContext();
	gameWindow = FindWindowA(NULL, "AssaultCube");
	o_WndProc = (WNDPROC)SetWindowLongPtr(gameWindow, GWLP_WNDPROC, (LONG_PTR)newWndProc);
	ImGui_ImplWin32_Init(gameWindow);
	ImGui_ImplOpenGL2_Init();
	ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; 
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableSetMousePos;

	io.Fonts->AddFontDefault();
	ImGui::SetCurrentContext(ImGui::GetCurrentContext());
	ImGui::SetNextWindowSize(initWindowSize);
	initialized = true;
	
	std::cout << "Menu initialized" << std::endl;
}

void Menu::startRender(){
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void testSettings() {
	if (!ImGui::BeginTabItem("testing")) return;


}

void espSettings() {
	if (!ImGui::BeginTabItem("ESP")) return;

	ImGui::Checkbox("Enable ESP", &Settings::ESP::enabled);
	ImGui::Checkbox("Draw Team", &Settings::ESP::drawTeam);
	ImGui::ColorEdit4("Team Color", (float*)&Settings::ESP::teamColor->Value);
	ImGui::ColorEdit4("Enemy Color", (float*)&Settings::ESP::enemyColor->Value);
	ImGui::EndTabItem();

}

void Menu::render(){
	if (!showMenu) return;

	ImGui::Begin("Menu", &showMenu);
	if (ImGui::BeginTabBar("Tabbar")) {
		espSettings();
		testSettings();
		ImGui::EndTabBar();
	}


	o_SDL_SetRelativeMouseMode(!showMenu); 

	ImGui::End();
}

void SetupContext(HDC& hdc) {
	myContext = wglCreateContext(hdc);

	wglMakeCurrent(hdc, myContext);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	contextCreated = true;
}

void Menu::endRender(){
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

}

BOOL __stdcall Menu::newSwapBuffers(HDC hdc){
	if (!initialized) {
		Menu::init();
		o_wglSwapBuffers(hdc);
	}
	gameContext = wglGetCurrentContext();
	if (!contextCreated) SetupContext(hdc);
	wglMakeCurrent(hdc, myContext);
	Menu::startRender();
	Menu::render();
	ESP::drawESP();
	Menu::endRender();

	wglMakeCurrent(hdc, gameContext);
	return o_wglSwapBuffers(hdc);
}

void Menu::cleanUp(){

	if (initialized) {
		// 恢复原始窗口过程
		if (o_WndProc && gameWindow) {
			SetWindowLongPtr(gameWindow, GWLP_WNDPROC, (LONG_PTR)o_WndProc);
		}

		// 清理 ImGui 资源
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplWin32_Shutdown();

		// 删除 OpenGL 上下文
		if (contextCreated && myContext) {
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(myContext);
		}

		ImGui::DestroyContext();

		initialized = false;
		contextCreated = false;
		std::cout << "Menu resources cleaned up" << std::endl;
	}
}
