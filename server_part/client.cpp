#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

#pragma comment (lib, "Ws2_32.lib")

HWND bt_lobby;
HWND bt_quit;
HWND bt_lead_board;
HWND bt_login;
HWND bt_singup;
HWND bt_name;
HWND bt_password;
HWND bt_ok;
HWND bt_reg;
HWND bt_back;

HWND hwnd;
WNDCLASSA wc1;


LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam); 

void client();

void makewindow() {
	memset(&wc1, 0, sizeof(WNDCLASSA));
	wc1.lpszClassName = "Client";
	wc1.lpfnWndProc = WNDPROC(WndProc);
	RegisterClassA(&wc1);

	hwnd = CreateWindow(L"Client", L"Client", WS_OVERLAPPEDWINDOW,
		10, 10, 640, 480, NULL, NULL, NULL, NULL);

	ShowWindow(hwnd, SW_SHOWNORMAL);

	bt_login = CreateWindow(L"button", L"LogIn", WS_VISIBLE | WS_CHILD,
		10, 10, 100, 30,
		hwnd, NULL, NULL, NULL);

	bt_singup = CreateWindow(L"button", L"SingUp", WS_VISIBLE | WS_CHILD,
		10, 40, 100, 30,
		hwnd, NULL, NULL, NULL);

	bt_quit = CreateWindow(L"button", L"Quit", WS_VISIBLE | WS_CHILD,
		10, 110, 100, 50,
		hwnd, NULL, NULL, NULL);
}

int main() {

	client();
	
	makewindow();

	/*bt_lobby = CreateWindow(L"button", L"Lobby", WS_VISIBLE | WS_CHILD,
		10, 10, 100, 50,
		hwnd, NULL, NULL, NULL);

	bt_lead_board = CreateWindow(L"button", L"Leader board", WS_VISIBLE | WS_CHILD,
		10, 60, 100, 50,
		hwnd, NULL, NULL, NULL);

	bt_quit = CreateWindow(L"button", L"Quit", WS_VISIBLE | WS_CHILD,
		10, 110, 100, 50,
		hwnd, NULL, NULL, NULL);

	bt_name = CreateWindow(L"button", L"Quit", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT,
		10, 110, 100, 50,
		hwnd, NULL, NULL, NULL);

	bt_password = CreateWindow(L"button", L"Quit", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_RIGHT,
		10, 110, 100, 50,
		hwnd, NULL, NULL, NULL);*/

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);
		UpdateWindow(hwnd);
	}

	return 0;
}

LRESULT WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_DESTROY) {
		PostQuitMessage(0);
	}
	else if (message == WM_MOUSEMOVE) {
		int xPos = LOWORD(lparam);
		int yPos = HIWORD(lparam);
	}
	else if (message == WM_COMMAND) {
		if (LPARAM(bt_lobby) == lparam) {

		}
		else if (LPARAM(bt_quit) == lparam) {
			PostQuitMessage(0);
		}
		else if (LPARAM(bt_lead_board) == lparam) {

		}
		else if (LPARAM(bt_login) == lparam) {
			DestroyWindow(bt_login);

			DestroyWindow(bt_singup);

			DestroyWindow(bt_quit);

			bt_ok = CreateWindow(L"button", L"Ok", WS_VISIBLE | WS_CHILD,
				30, 90, 50, 30,
				hwnd, NULL, NULL, NULL);

			bt_back = CreateWindow(L"button", L"back", WS_VISIBLE | WS_CHILD,
				10, 130, 100, 30,
				hwnd, NULL, NULL, NULL);
		}
		else if (LPARAM(bt_singup) == lparam) {
			DestroyWindow(bt_login);

			DestroyWindow(bt_singup);

			DestroyWindow(bt_quit);

			bt_ok = CreateWindow(L"button", L"Ok", WS_VISIBLE | WS_CHILD,
				30, 90, 50, 30,
				hwnd, NULL, NULL, NULL);

			bt_back = CreateWindow(L"button", L"back", WS_VISIBLE | WS_CHILD,
				10, 130, 100, 30,
				hwnd, NULL, NULL, NULL);
		}
		else if (LPARAM(bt_ok) == lparam) {
			
		}
		else if (LPARAM(bt_reg) == lparam) {

		}
		else if (LPARAM(bt_back) == lparam) {
			DestroyWindow(bt_name);
			DestroyWindow(bt_password);
			DestroyWindow(bt_back);
			DestroyWindow(bt_ok);

			bt_login = CreateWindow(L"button", L"LogIn", WS_VISIBLE | WS_CHILD,
				10, 10, 100, 30,
				hwnd, NULL, NULL, NULL);

			bt_singup = CreateWindow(L"button", L"SingUp", WS_VISIBLE | WS_CHILD,
				10, 40, 100, 30,
				hwnd, NULL, NULL, NULL);

			bt_quit = CreateWindow(L"button", L"Quit", WS_VISIBLE | WS_CHILD,
				10, 110, 100, 50,
				hwnd, NULL, NULL, NULL);
		}
	}
	else return DefWindowProcA(hwnd, message, wparam, lparam);
}

void client()
{
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);

	SOCKET soc;
	soc = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(8800);

	sa.sin_addr.S_un.S_addr = inet_addr("192.168.3.10");

	connect(soc, (const sockaddr*)&sa, sizeof(sa));
}