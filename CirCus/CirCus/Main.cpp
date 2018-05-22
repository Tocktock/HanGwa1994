#include <windows.h>
#include<ctime>
#include<iostream>
#include<string>
#include "resource.h"
#include "ResourceManager.h"
#include "GameManager.h"
#define	GAMING 1
#define Win 3
#define DEAD 2


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
char g_szClassName[256] = "CirCus";

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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

	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}

	return (int)Message.wParam;
}

bool g_isJump = false;
int	 g_MovingCount = 0;
bool g_isRightKeyDown = false;
bool g_isLeftKeyDown = false;
int g_JumpingCount = 0;
int g_CircleCount = 0;
int g_CircleMakeCount = 0;
int g_WinCount = 0;
int g_BonusCount = 0;
int g_EndCount = 0;
int g_StarCount = 0;
 bool g_isMove = false;
 bool g_madeCircle = false;
 bool g_isStart = false;
 bool g_MenuSelect = true;
 bool g_TermDraw = false;
 int g_ScoreFlag = 0;
 int g_CircleScoreFlag = 0;
 int g_WinEndCount = 0;
 int g_TermDrawCount = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH Brush, oldBrush;
	static POINT pt;
	

	switch (iMessage)
	{
	case WM_CREATE:
		SetTimer(hWnd, 1, 1000 / 144, NULL);
		ResourceManager::GetInstance()->Init(hWnd);
		GameManager::GetInstance()->Init(hWnd);
		hdc = GetDC(hWnd);
		GameManager::GetInstance()->DubbleBufferInit(hdc);
		ReleaseDC(hWnd, hdc);
		srand(time(NULL));

		return 0;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			if (g_isStart == false)
			{
				GameManager::GetInstance()->MenuSelectStart();
				g_MenuSelect = true;
			}
			break;
		case VK_DOWN:
			if (g_isStart == false)
			{
				GameManager::GetInstance()->MenuSelectExit();
				g_MenuSelect = false;
			}
			break;
		case VK_RETURN:
			if (g_MenuSelect == false)
				exit(0);

			GameManager::GetInstance()->StartGame();
			g_isStart = true;
			break;
		}
		return 0;

	case WM_TIMER:
		if(GameManager::GetInstance()->isGetTermDraw() == true)
			g_TermDrawCount++;

		if(g_TermDrawCount == 200 && GameManager::GetInstance()->isGetTermDraw() == true)
		{
			g_TermDrawCount = 0;
			GameManager::GetInstance()->TermDrawOff();
			InvalidateRect(hWnd, NULL, false);
		}

		if (g_isStart == false)
		{
			g_StarCount++;
			if (g_StarCount == 15)
			{
				GameManager::GetInstance()->StatePlus();
				g_StarCount = 0;
				InvalidateRect(hWnd, NULL, false);

			}
		}

		else if (g_isStart == true)
		{
			InvalidateRect(hWnd, NULL, false);

			GameManager::GetInstance()->CollisionCheck();

			if (GameManager::GetInstance()->EnemyScoreCheck() != 0)
			{
				g_ScoreFlag = GameManager::GetInstance()->EnemyScoreCheck();
			}

			if (GameManager::GetInstance()->CircleScoreCheck() != 0)
			{
				g_CircleScoreFlag = GameManager::GetInstance()->CircleScoreCheck();
			}

			g_BonusCount++;

			if (g_BonusCount == 12)
			{
				if (GameManager::GetInstance()->isGameState() == GAMING)
				{
					if (GameManager::GetInstance()->GetBonusScore() != 0)
						GameManager::GetInstance()->BonusMinus();
				}
				
				g_BonusCount = 0;
			}

			if (GameManager::GetInstance()->isGameState() == Win)
			{
				g_WinEndCount++;

				if (g_WinEndCount == 1000)
				{
					g_isStart = false;
					GameManager::GetInstance()->ToMenuReset();
					GameManager::GetInstance()->Init(hWnd);
					g_WinEndCount = 0;
				}

				if (GameManager::GetInstance()->GetBonusScore() != 0 &&
					GameManager::GetInstance()->isGameState() == Win)
				{
					GameManager::GetInstance()->WinBonusScore();
					GameManager::GetInstance()->ScorePlus(20);
				}

				g_WinCount++;
				if (g_WinCount == 25)
				{
					GameManager::GetInstance()->WinFirstState();
					InvalidateRect(hWnd, NULL, false);
				}
				else if (g_WinCount == 50)
				{
					GameManager::GetInstance()->WinSecondState();
					InvalidateRect(hWnd, NULL, false);
					g_WinCount = 0;
				}
			}

			if (GameManager::GetInstance()->isGameState() == DEAD)
			{
				g_EndCount++;

				if (g_EndCount == 300)
				{
					

					if (GameManager::GetInstance()->GetLife() == -1)
					{
						g_isStart = false;
						GameManager::GetInstance()->ToMenuReset();
						GameManager::GetInstance()->Init(hWnd);
					}
					else
					{
						GameManager::GetInstance()->Reset();
						GameManager::GetInstance()->Init(hWnd);
						GameManager::GetInstance()->TermDrawOn();

					}
						g_EndCount = 0;
				}
				else
					return 0;
			}


			GameManager::GetInstance()->CharacterSetRt();
			GameManager::GetInstance()->PotFlameSetRt();
			GameManager::GetInstance()->CircusCircleSetRt();
			GameManager::GetInstance()->EndSetRt();

			GameManager::GetInstance()->EnemyStateCtPlus();

			g_CircleCount++;

			if (rand() % 75 == 0 && g_madeCircle == false &&
				 GameManager::GetInstance()->isGameState() ==GAMING)						    //서클 만들기
			{
				if (GameManager::GetInstance()->isGameState() == Win)
					break;

				GameManager::GetInstance()->EnemyCircle();
				g_madeCircle = true;

			}

			if (g_madeCircle == true
				&& GameManager::GetInstance()->isGameState() == GAMING)													//만들어지면 일정 
			{																		//기간동안 만드는거 정지
				

				g_CircleMakeCount++;

				if (g_CircleMakeCount == 400)
				{
					g_madeCircle = false;

					g_CircleMakeCount = 0;
				}
			}

			if (g_CircleCount > 10)													//서클 움직이는거
			{
				if (GameManager::GetInstance()->isGameState() == Win)
					break;

				GameManager::GetInstance()->CircleMove();
				g_CircleCount = 0;
			}

			if (GetKeyState(VK_RIGHT) & 0x8000 && g_isJump == false &&
				GameManager::GetInstance()->isGameState() == GAMING)					//화살표 오른쪽 키
			{
				

				g_MovingCount++;
				GameManager::GetInstance()->CharacterRight();

				if (g_MovingCount == 10)
				{
					GameManager::GetInstance()->CharacterMoving();
					g_MovingCount = 0;
				}

				g_isRightKeyDown = true;
			}

			if (GetKeyState(VK_LEFT) & 0x8000 && g_isJump == false &&
				GameManager::GetInstance()->isGameState() == GAMING)					//화살표 왼쪽키
			{
				
				g_MovingCount++;
				GameManager::GetInstance()->CharacterLeft();

				if (g_MovingCount == 10)
				{
					GameManager::GetInstance()->CharacterLeftMoving();
					g_MovingCount = 0;
				}
				g_isLeftKeyDown = true;
			}



			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && g_isJump == false &&
				GameManager::GetInstance()->isGameState() == GAMING)				//스페이스 점프키
			{
				

				GameManager::GetInstance()->SetCenterX();
				GameManager::GetInstance()->SetCenterY();
				g_isJump = true;
			}







			if (GameManager::GetInstance()->GetCharacterState() > 0 && g_isJump == false
				&& GameManager::GetInstance()->isGameState() == GAMING)	//움직이다 정지했을때 모션
			{
				
				g_MovingCount++;
				if (g_MovingCount == 10)
				{
					GameManager::GetInstance()->MoveSet();
					g_MovingCount = 0;
				}
			}

			if (g_isJump == true &&
				GameManager::GetInstance()->isGameState() == GAMING)														//점프구현
			{
				GameManager::GetInstance()->JumpMotion();


				for (static int i = 180; i <= 360; i++)
				{
					g_JumpingCount++;
					GameManager::GetInstance()->CharacterJumping(i);
					if (g_JumpingCount == 3)
					{
						if (g_isRightKeyDown == true)
							GameManager::GetInstance()->CharacterRight();

						else if (g_isLeftKeyDown == true)
							GameManager::GetInstance()->CharacterLeft();
						g_JumpingCount = 0;
						break;
					}
					if (i == 360)
					{
						i = 180;
						GameManager::GetInstance()->SetY(300);
						if(g_ScoreFlag != 0 && g_CircleScoreFlag == 0 ||
							g_ScoreFlag == 0 && g_CircleScoreFlag != 0)
						{
							
								GameManager::GetInstance()->ScorePlus(g_ScoreFlag);

								GameManager::GetInstance()->ScorePlus(g_CircleScoreFlag);
						}

						else if (g_ScoreFlag != 0 && g_CircleScoreFlag != 0)
						{
							GameManager::GetInstance()->ScorePlus(700);
						}
					

						g_ScoreFlag = 0;
						g_CircleScoreFlag = 0;
						g_isJump = false;
						break;
					}
				}
			

			}

			InvalidateRect(hWnd, NULL, false);

			if (g_isJump != true)
			{
				g_isLeftKeyDown = false;
				g_isRightKeyDown = false;
			}
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
			GameManager::GetInstance()->FinalDraw(hdc);

				
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		KillTimer(hWnd, 0);
		GameManager::GetInstance()->Release();
		ResourceManager::GetInstance()->Release();
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}
