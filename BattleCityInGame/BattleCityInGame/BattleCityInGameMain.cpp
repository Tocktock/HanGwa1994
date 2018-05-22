#include<windows.h>
#include<ctime>
#include<iostream>
#include<string>
#include "ResourceManager.h"
#include "GameManager.h"
#include "Block.h"
#include <crtdbg.h>
#include<iostream>

using namespace std;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "빠뜰시티인게임";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtDumpMemoryLeaks();
	//_CrtSetBreakAlloc(392); //괄호 안에 릭 숫자를 넣음
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
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, 0, 0,
		1024, 1024, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	ResourceManager::GetInstance()->Init(hWnd);
	GameManager::GetInstance()->Init(hWnd);
	while (true)                          //픽메세지는  kb히트랑 비슷함
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE))
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

	return (int)Message.wParam;
}

#define BLOCKWIDTH 33
#define BLOCKHEIGHT 25


LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
//	HDC hdc;
//	PAINTSTRUCT ps;
//	//TCHAR *Mes = TEXT("왼쪽 클릭 : 에디트 이동, 오른쪾 클릭 : 보임/ 숨김");

//	static int KeyDownCount = 0;
//	static bool isSpaceDown = false;
	switch (iMessage)
	{
/*	case WM_CREATE:

		SetTimer(hWnd, 10, 1000 / 60, NULL);
		ResourceManager::GetInstance()->Init(hWnd);
		GameManager::GetInstance()->Init(hWnd);
		GameManager::GetInstance()->LoadData(hWnd);
		GameManager::GetInstance()->ConstructBlock();

		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			isSpaceDown = true;
			GameManager::GetInstance()->MissileFire();

			break;
		}
		return 0;
		
	case WM_LBUTTONDOWN:
		GameManager::GetInstance()->OperateLbutton(LOWORD(lParam), HIWORD(lParam));
	//	InvalidateRect(hWnd, NULL, false);
		return 0;

	case WM_RBUTTONDOWN:
		GameManager::GetInstance()->OperateRbutton(LOWORD(lParam), HIWORD(lParam));
	//	InvalidateRect(hWnd, NULL, true);
		return 0;

	case WM_TIMER:
		GameManager::GetInstance()->();
		GameManager::GetInstance()->SetRt();
		GameManager::GetInstance()->FireMove();
		MovementCode();
		InvalidateRect(hWnd, NULL, false);
		return 0;


	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
	//	Rectangle(hdc, 0, 0, BLOCKWIDTH * 14, BLOCKHEIGHT * 16);
		for (int x = 0; x < 14; x++)
		{
			for (int y = 0; y < 16; y++)
			{
			Rectangle(hdc, BLOCKWIDTH *x, BLOCKHEIGHT*y, BLOCKWIDTH*(x + 1), BLOCKHEIGHT*(y + 1));
			}
		}
		GameManager::GetInstance()->Draw();
		//	InvalidateRect(hWnd, NULL, false);
		EndPaint(hWnd, &ps);
		return 0;
		*/
		
	case WM_DESTROY:

		//GameManager::GetInstance()->SaveData(hWnd);

	//	GameManager::GetInstance()->Release();
	//	ResourceManager::GetInstance()->Release();

		ResourceManager::GetInstance()->Release();
		GameManager::GetInstance()->Release();
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
