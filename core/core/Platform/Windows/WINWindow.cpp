#include "WINWindow.h"
#include <iostream>

namespace VE
{
	WINWindow::WINWindow() :
		wc(), hInstance(NULL), hWnd(NULL), msg(), nCmdShow(1), isWindowOpen(false)
	{
	}

	WINWindow::~WINWindow()
	{
	}

	void WINWindow::Create(int PositionX, int PositionY, int Width, int Height)
	{
		const wchar_t CLASS_NAME[] = L"Vulkan Window";

		hInstance = GetModuleHandle(NULL);

		wc = { };
		wc.lpfnWndProc = WndProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = CLASS_NAME;

		RegisterClass(&wc);

		hWnd = CreateWindowEx(
			0,
			CLASS_NAME,
			L"Vulkan Engine 😊",
			WS_OVERLAPPEDWINDOW,
			PositionX,
			PositionY,
			Width,
			Height,
			NULL,
			NULL,
			hInstance,
			this);

		if (hWnd == NULL)
		{
			std::cout << "Failed to create a window!" << std::endl;
			return;
		}
		else
			std::cout << "Window created successfully!" << std::endl;

		isWindowOpen = true;
		ShowWindow(hWnd, nCmdShow);
	}

	void WINWindow::Update()
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	LRESULT CALLBACK WINWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WINWindow* window = nullptr;

		if (msg == WM_NCCREATE)
		{
			window = static_cast<WINWindow*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window)))
			{
				if (GetLastError() != 0)
					return false;
			}
		}
		else
			window = reinterpret_cast<WINWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (msg)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			window->isWindowOpen = false;
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			window->isWindowOpen = false;
			break;

		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return 0;
	}
}