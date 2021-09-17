#pragma once
#include <Windows.h>
#include "../../Common.h"

namespace VE
{
	class VE_API WINWindow
	{
	private:
		WNDCLASS wc;
		HINSTANCE hInstance;
		HWND hWnd;
		MSG msg;
		int nCmdShow;
		bool isWindowOpen;

	public:
		WINWindow();
		~WINWindow();

		void Create(int PositionX, int PositionY, int Width, int Height);
		void Update();

		inline bool WindowIsOpen() { return isWindowOpen; }
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};
}