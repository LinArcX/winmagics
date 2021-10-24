#include "main.h"

#define SHIFTED 0x8000 

INT __stdcall WinMain(HINSTANCE Instance,
					  HINSTANCE PreviousInstance,
					  PSTR CmdLine,
					  INT CmdShow) 
{
	UNREFERENCED_PARAMETER(Instance);
	UNREFERENCED_PARAMETER(PreviousInstance);
	UNREFERENCED_PARAMETER(CmdLine);
	UNREFERENCED_PARAMETER(CmdShow);

	INT Result = 0;
	MSG message = {0};

	if (IsAppAlreadyRunning())
	{
		MessageBoxA(NULL, "Another instance of this program is already running!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if (ERROR_SUCCESS != CreateMainGameWindow())
	{
		goto Exit;
	}

	gGameIsRunning = TRUE;
	while (gGameIsRunning)
	{
		while (PeekMessage(&message, gWindowHandle, 0, 0, PM_REMOVE))
		{
		  DispatchMessage(&message);
		}

		// Main Loop
		if(ProcessKeyStrokes())
		{
			SimulateKeyStrokes();
		}
		Sleep(100);
	}

Exit:
	return (Result);
}

LRESULT CALLBACK MainWindowProc(HWND WindowHandle,
								UINT Message,
								WPARAM WParam,
								LPARAM LParam)
{
	LRESULT Result = 0;

	switch (Message)
	{
		case WM_CLOSE:
		{
			gGameIsRunning = FALSE;
			PostQuitMessage(0);
		} break;
		default: 
		{
			Result = DefWindowProc(WindowHandle, Message, WParam, LParam);
		} break;
	}
	return Result;
}


DWORD CreateMainGameWindow(void)
{
	DWORD Result = ERROR_SUCCESS;
	WNDCLASSEX WindowClass = {0};

	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = 0;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.lpszMenuName = NULL;
	WindowClass.hInstance = GetModuleHandle(NULL);
	WindowClass.lpszClassName = APP_NAME;
	WindowClass.lpfnWndProc = MainWindowProc;

	if (!RegisterClassEx(&WindowClass))
	{
		Result = GetLastError();
		MessageBox(NULL, "Window Registration Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	gWindowHandle = CreateWindowEx(0, APP_NAME, APP_NAME, WS_OVERLAPPEDWINDOW | SW_HIDE, CW_USEDEFAULT,
		CW_USEDEFAULT, 10, 10, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (gWindowHandle == NULL)
	{
		Result = GetLastError();
		MessageBox(NULL, "Window Creation Failed!", "Error!",
			MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

	if (!SetWindowLongPtr(gWindowHandle, GWL_STYLE,
		(WS_OVERLAPPEDWINDOW | SW_HIDE) & ~WS_OVERLAPPEDWINDOW))
	{
		Result = GetLastError();
		MessageBox(NULL, "Can't SetWindowLongPtr!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		goto Exit;
	}

Exit:
	return (Result);
}

BOOL IsAppAlreadyRunning(void)
{
	HANDLE Mutex = NULL;
	Mutex = CreateMutex(NULL, FALSE, APP_NAME "_Mutex");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		return (TRUE);
	} 
	else
	{
		return (FALSE);
	}
}

bool ProcessKeyStrokes(void)
{
	bool successful = false;

	int virtualKeyCode = GetKeyState(VK_SHIFT); 
	if (virtualKeyCode & SHIFTED) 
	{ 
		if(GetKeyState(VK_ESCAPE) & 0x0001)
		{
			successful = true;
		}
	}

	return successful;
}

void SimulateKeyStrokes(void)
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// clear screen: ESC + CLS + ENTER
	ip.ki.wVk = VK_ESCAPE;
	ip.ki.dwFlags = 0; // pressed
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // released
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = 'C';
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = 'L';
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = 'S';
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = VK_RETURN;
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));
	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &ip, sizeof(INPUT));
}