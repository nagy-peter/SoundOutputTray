
// SoundOutputTray.cpp

#include <windows.h>
#include <shellapi.h>

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define IDI_TRAYICON 101

#include "resource.h"

HINSTANCE g_hInst;
HWND g_hWnd;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void SendCtrlWinVInput()
{
	INPUT ip;

	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	ip.ki.dwFlags = 1;
	ip.ki.wVk = VK_CONTROL;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = 1;
	ip.ki.wVk = VK_LWIN;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = 'V';
	ip.ki.dwFlags = 0;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = VK_CONTROL;
	ip.ki.dwFlags = KEYEVENTF_KEYUP | 1;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.wVk = VK_LWIN;
	ip.ki.dwFlags = KEYEVENTF_KEYUP | 1;
	SendInput(1, &ip, sizeof(INPUT));

	ip.ki.dwFlags = KEYEVENTF_KEYUP;
	ip.ki.wVk = 'V';
	SendInput(1, &ip, sizeof(INPUT));
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	g_hInst = hInstance;

	// Window class
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("SoundOutputTrayWindow");
	RegisterClass(&wc);

	HWND hWnd = CreateWindow(wc.lpszClassName, TEXT("SoundOutputTrayWindow"), 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);

	// Tray icon
	NOTIFYICONDATA nid;
	nid.cbSize = sizeof(nid);
	nid.hWnd = hWnd;
	nid.uID = 1;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_TRAYICON;
	nid.hIcon = (HICON)LoadImage(g_hInst,MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	lstrcpy(nid.szTip, TEXT("Sound Output Tray App"));

	// Balloon notification
	lstrcpy(nid.szInfoTitle, TEXT("Sound Output Tray Running"));
	lstrcpy(nid.szInfo, TEXT("The application is now in the tray."));
	nid.dwInfoFlags = NIIF_NONE;

	Shell_NotifyIcon(NIM_ADD, &nid);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Shell_NotifyIcon(NIM_DELETE, &nid);
	return 0;
}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);

	HMENU hMenu = CreatePopupMenu();
	InsertMenu(hMenu, -1, MF_BYPOSITION, ID_TRAY_EXIT, TEXT("Quit"));

	SetForegroundWindow(hWnd);
	TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hWnd, NULL);

	DestroyMenu(hMenu);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_TRAYICON:
		if (lParam == WM_LBUTTONUP)
		{
			SendCtrlWinVInput();
		}
		else if (lParam == WM_RBUTTONUP)
		{
			ShowContextMenu(hWnd);
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == ID_TRAY_EXIT)
		{
			PostQuitMessage(0);
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
