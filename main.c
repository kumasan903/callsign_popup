#include <windows.h>
#include "resource.h"

const char	g_class_name[] = "myWindowClass";
HWND hStaticText;

LRESULT CALLBACK dialog_proc(HWND hdlg, UINT msg, WPARAM wparam, LPARAM lparam)
{
	static char new_text[256];

	switch(msg)
	{
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			switch (LOWORD(wparam))
			{
				case IDOK:
					GetDlgItemText(hdlg, IDC_EDIT1, new_text, sizeof(new_text));
					SetWindowText(hStaticText, new_text);
					EndDialog(hdlg, IDOK);
					return TRUE;
				case IDCANCEL:
					EndDialog(hdlg, IDCANCEL);
					return TRUE;
			}
			break;
	}
	return FALSE;
}

LRESULT	CALLBACK window_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		case WM_CREATE:
		{
			hStaticText = CreateWindow(
				"STATIC",
				"Click to change text",
				WS_VISIBLE | WS_CHILD | SS_CENTER | SS_NOTIFY,
				10,
				10,
				200,
				80,
				hwnd,
				(HMENU)1,
				NULL,
				NULL
			);
			if (!hStaticText)
				MessageBox(NULL, "Static Failed", "Error", MB_OK | MB_ICONERROR);
			if (!SetWindowText(hStaticText, "JA123A"))
				MessageBox(NULL, "Failed to Set Text", "Error", MB_OK | MB_ICONERROR);
			HFONT hFont = CreateFont(
					40,
					0,
					0,
					0,
					FW_BOLD,
					FALSE,
					FALSE,
					FALSE,
					DEFAULT_CHARSET,
					OUT_DEFAULT_PRECIS,
					CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_SWISS,
					"Arial"
					);
			SendMessage(hStaticText, WM_SETFONT, (WPARAM)hFont, TRUE);
			SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			break;
		}
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == 1)
			{
				if (HIWORD(wParam) == STN_CLICKED)
				{
					DialogBox(NULL, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, dialog_proc);
				}
			}
			break;
		}
		case WM_CTLCOLORSTATIC:	 // 消去?
		{
			HDC hdcStatic = (HDC)wParam;
			SetBkColor(hdcStatic, GetSysColor(COLOR_WINDOW));
			return (LRESULT)GetStockObject(WHITE_BRUSH);
		}
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return (0);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG Msg;

	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= 0;
	wc.lpfnWndProc		= window_proc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= g_class_name;
	wc.hIconSm			= LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed", "Error",
				MB_ICONEXCLAMATION | MB_OK);
		return (0);
	}

	hwnd = CreateWindowEx
	(
			WS_EX_CLIENTEDGE,
			g_class_name,
			"callsign",
			WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, 240, 100,
			NULL, NULL, hInstance, NULL
	);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!",
				MB_ICONEXCLAMATION | MB_OK);
		return (0);
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}
