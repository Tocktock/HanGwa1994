#include <WinSock2.h>
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include "GameManager.h"
#include "PACKET_HEADER.h"
#include "stdafx.h"

using namespace std;
#define WM_SOCKET (WM_USER+1)

SOCKET g_Sock;
int	g_iIndex = 0;
HINSTANCE g_hInst;


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
//bool ProcessPacket(UserInfo * pInfo, char* szBuf, int &len);
//void err_display(int errcode);
//void err_display(char* szMsg);



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "CatchMindClient";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow("CatchMindClient", "CatchMindClient", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);


	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	g_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_Sock == INVALID_SOCKET)
	{
		MessageBox(hWnd, "err on socket", "err", MB_ICONERROR);
		return -1;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(g_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	bool optval = true;
	setsockopt(g_Sock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));


	int retval = WSAAsyncSelect(g_Sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	if (retval == SOCKET_ERROR)
	{
		MessageBox(hWnd, "err on WSAAsyncSelect", "err", MB_ICONERROR);
		return -1;

	}

	GameManager::GetInstance()->Init(hWnd, g_Sock);

	while (true)
	{
		/// 메시지큐에 메시지가 있으면 메시지 처리
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{

			GameManager::GetInstance()->Update();

		}
	}
	WSACleanup();

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN:
		GameManager::GetInstance()->LbuttonDown(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_LBUTTONUP:
		GameManager::GetInstance()->LbuttonUp(LOWORD(lParam), HIWORD(lParam));
		return 0;

	case WM_KEYDOWN:
		return 0;

	case WM_CHAR:
		GameManager::GetInstance()->InputManager((TCHAR)wParam);
		return 0;
	case WM_SOCKET:
		GameManager::GetInstance()->ProcessRecvPacket(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
	//	GameManager::GetInstance()->QuitGame();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}