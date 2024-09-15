#pragma once

#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

/********************
	심볼릭 상수
********************/
namespace ToolUI
{
	constexpr int g_iWindowSizeX = 1280;
	constexpr int g_iWindowSizeY = 720;
	constexpr float g_fWindowSizeX = 1280.0f;
	constexpr float g_fWindowSizeY = 720.0f;
}

extern HWND g_hWnd;
extern HINSTANCE g_hInst;
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

/********************
	매크로 상수
********************/
#define _CRTDEBG_MAP_ALLOC
#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define _CRTDBG_MAP_ALLOC