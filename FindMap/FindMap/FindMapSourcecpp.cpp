#include <windows.h>
#include<ctime>
#include<iostream>
#include<string>
#include "GameManager.h"
#include "ResouceManager.h"
#include "resource.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK SettingDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
HINSTANCE g_hInst;
char g_szClassName[256] = "FindMap";


enum Game_Mode
{
	EASY,NORMAL,HARD
};

bool RbuttonClicking =false;
bool LbuttonClicking =false;

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
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, 0, 0,
		1024, 1024, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH Brush, oldBrush;
	static POINT pt;
	static int timer = 0;

	static string timer_str;

	timer_str = to_string(timer);

	switch (iMessage)
	{
	case WM_CREATE:
		srand(time(NULL));
		ResouceManager::GetInstance()->Init(hWnd);

		GameManager::GetInstance()->Init(hWnd);
		return 0;

	case WM_COMMAND:
		if (LOWORD(wParam) == ID_SETTING_GAMESETTING)
		{
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, SettingDlg);                  //다이얼로그 박스 띄우는거


		}
	
		InvalidateRect(hWnd, NULL, true);
		GameManager::GetInstance()->Init(hWnd);

		return 0;
	case WM_TIMER:
		timer++;
		InvalidateRect(hWnd, NULL, true);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		GameManager::GetInstance()->Draw(hdc);
		SetBkColor(hdc, RGB(255, 255, 0));											//텍스트배경 색깔
		SetBkMode(hdc, TRANSPARENT);												//텍스트배경 모드
		TextOut(hdc, 180, 480, timer_str.c_str(), timer_str.length());              //시간표시
		
		Brush = CreateSolidBrush(RGB(255,0,0));
		oldBrush = (HBRUSH)SelectObject(hdc, Brush);
		Ellipse(hdc, 400, 10, 430, 40);

		DeleteObject(Brush);
		EndPaint(hWnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:

		SetTimer(hWnd, 0, 1000, NULL);
		LbuttonClicking = true;

		if (LOWORD(lParam) > 400 && LOWORD(lParam) < 430 && HIWORD(lParam) > 10 && HIWORD(lParam) < 40)
		{
			GameManager::GetInstance()->EraseMap();
			InvalidateRect(hWnd, NULL, true);
			GameManager::GetInstance()->Init(hWnd);
		}
		
		if (LbuttonClicking == true && RbuttonClicking == true)                        //좌우 동시클릭
		{
			GameManager::GetInstance()->OperateLRbutton(hWnd, pt.x = LOWORD(lParam), pt.y = HIWORD(lParam));

				GameManager::GetInstance()->VictoryCheck(hWnd);

				LbuttonClicking = false;
		}

		GameManager::GetInstance()->VictoryCheck(hWnd);

		InvalidateRect(hWnd, NULL, true);


		return 0;

	case WM_RBUTTONDOWN:
		RbuttonClicking = true;

		if (LbuttonClicking == true && RbuttonClicking == true)                //좌우 동시클릭
		{
			GameManager::GetInstance()->OperateLRbutton(hWnd, pt.x = LOWORD(lParam), pt.y = HIWORD(lParam));

			GameManager::GetInstance()->VictoryCheck(hWnd);


			RbuttonClicking = false;
		}
		InvalidateRect(hWnd, NULL, true);

		return 0;

	case WM_RBUTTONUP:

		if (LbuttonClicking == false)
			GameManager::GetInstance()->OperateRbutton(hWnd, pt.x = LOWORD(lParam), pt.y = HIWORD(lParam));

		InvalidateRect(hWnd, NULL, true);
		RbuttonClicking = false;
		return 0;

	case WM_LBUTTONUP:
		if (RbuttonClicking == false)
		{
			GameManager::GetInstance()->OperateLbutton(hWnd, pt.x = LOWORD(lParam), pt.y = HIWORD(lParam));
		}
		InvalidateRect(hWnd, NULL, true);
		LbuttonClicking = false;
		return 0;


	case WM_DESTROY:
		KillTimer(hWnd, 0);
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}


INT_PTR CALLBACK SettingDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hRadio;

	switch (message)
	{
	case WM_INITDIALOG:
		hRadio = GetDlgItem(hDlg, IDC_RADIO1);
		SendMessage(hRadio, BM_SETCHECK, BST_CHECKED, 0);
		return (INT_PTR)TRUE;

	case WM_COMMAND:

		if (LOWORD(wParam) == IDOK)
		{
			if (IsDlgButtonChecked(hDlg, IDC_RADIO1) == BST_CHECKED)
			{
				GameManager::GetInstance()->SetGameMode(GAME_MODE_EASY); 
				GameManager::GetInstance()->EraseMap();

			}
			
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO2) == BST_CHECKED)
			{
				GameManager::GetInstance()->SetGameMode(GAME_MODE_NORMAL);
				GameManager::GetInstance()->EraseMap();
			}
			else if (IsDlgButtonChecked(hDlg, IDC_RADIO3) == BST_CHECKED)
			{
				GameManager::GetInstance()->SetGameMode(GAME_MODE_HARD);
				GameManager::GetInstance()->EraseMap();
			}

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		break;
	}
	return (INT_PTR)FALSE;
}
