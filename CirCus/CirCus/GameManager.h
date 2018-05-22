#pragma once
#include <Windows.h>
#include "Macro.h"
#include "Character.h"
#include "Enemy.h"
#include "CirCusCircle.h"
#include <vector>
#include <string>
using namespace std;

#define pi 3.141592

#define CHARACTER_WIDTH 66
#define CHARACTER_HEIGHT 63

#define BACK_WIDTH	67
#define BACK_HEIGHT	183

#define BACKDECO_WIDTH 66
#define BACKDECO_HEIGHT 67
 
#define BACKNORMAL1_WIDTH 65
#define BACKNORMAL1_HEIGHT 64

#define BACKNORMAL2_WIDTH 65
#define BACKNORMAL2_HEIGHT 64

#define CHARACTER_WIDTH 66
#define CHARACTER_HEIGHT 63

#define GAMING 1
#define WIN 3
#define DEAD 2
class Bitmap;
class Character;
class Enemy;
class CirCusCircle;
class Back;
class End;
class UI;
class UIStar;
class LuckyCircle;
class GameManager
{
	Bitmap * m_arrBackBitmap[4];
	Bitmap * m_arrCharacterBitmap[6];
	Bitmap * m_Meter;
	Bitmap * m_arrEnemy[11];
	Bitmap * m_EndCircleBitmap;
	Bitmap * m_UIBitmap[5];
	Bitmap * m_Money[5];
	vector<Enemy*> m_vecEnemy;
	vector<CirCusCircle*> m_vecCircle;
	vector<Back *> m_vecBack;
	vector<UI * > m_vecUI;
	vector<UIStar * > m_vecUIStar;
	vector<LuckyCircle *> m_vecLuckyCircle;

	HDC		m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP m_hOldBit;
	int m_MeterCt;
	int m_WindowWidth;
	int m_WindowHegiht;
	int m_BackX;
	int m_MeterX;
	int m_EnemyX;
	int m_MeterValue;
	int movePoint;
	int m_Life;
	int m_Score;
	int m_Stage;
	int m_Bonus;
	int m_HighScore;
	int m_MenuSelect;
	int m_CharacterResetX;
	bool m_isStart;
	bool m_TermDraw;
	Character *m_Character;
	End *m_EndCircle;

	int isGameEnd;

	static GameManager* s_this;
	GameManager();
public:

	static GameManager* GetInstance()
	{
		if (s_this == NULL)
		{
			s_this = new GameManager();
		}
		return s_this;
	}

	void DubbleBufferInit(HDC hdc);
	void Init(HWND hWnd);
	void EnemyCircle();
	void GameDraw(HDC hdc);
	void MenuDraw(HDC hdc);
	void FinalDraw(HDC hdc);
	void CircleMove();
	void EnemyStateCtPlus();
	void PotFlameSetRt();
	void CircusCircleSetRt();
	void CollisionCheck();
	void CharacterLeft();
	void CharacterRight();
	void StarStatePlus();
	void EndSetRt();
	void WinFirstState();
	void WinSecondState();
	void WriteTextNum(HDC hdc,int value, int x, int y);
	void WriteTextChar(HDC hdc, string temp , int x, int y,int r,int g,int b);
	void WhenDeadXSet();
	int EnemyScoreCheck();
	int CircleScoreCheck();
	void ToMenuReset();

	inline void TermDrawOn()
	{
		m_TermDraw = true;
	}

	inline void TermDrawOff()
	{
		m_TermDraw = false;
	}

	inline bool isGetTermDraw()
	{
		return m_TermDraw;
	}
	inline int GetLife()
	{
		return m_Life;
	}

	inline void WinBonusScore()
	{
		m_Bonus -= 10;
	}

	inline int GetBonusScore()
	{
		return m_Bonus;
	}
	inline void ScorePlus(int x)
	{
		m_Score += x;
	}
	inline void MenuSelectStart()
	{
		m_MenuSelect = 360;
	}

	inline void MenuSelectExit()
	{
		m_MenuSelect = 390;
	}

	void Reset();

	void StatePlus();
	void TermDraw(HDC hdc);

	inline void StartGame()
	{
		m_isStart = true;
	}

	inline void BonusMinus()
	{
		m_Bonus -= 10;
	}

	inline int isGameState()
	{
		return isGameEnd;
	}
	
	inline void GameSet()
	{
		isGameEnd = DEAD;
	}

	inline void JumpMotion()
	{
		m_Character->JumpMotion();
	}

	inline float ToRadian(float Seta)
	{
		return pi * Seta / 180.0f;
	}

	inline void CharacterSetRt()
	{
		m_Character->SetmyRect();
	}

	

	
	inline void CharacterMoving()
	{
		m_Character->Moving();
	}

	inline void CharacterLeftMoving()
	{
		m_Character-> LeftMoving();
	}

	inline void MoveSet()
	{
		m_Character->MoveSet();
	}

	inline void CharacterJumping(float Seta)
	{
		m_Character->Jumping(Seta);
	}
	inline void SetCenterX()
	{
		m_Character->SetCenterX();
	}

	inline void SetCenterY()
	{
		m_Character->SetCenterY();
	}

	inline int GetCharacterState()
	{
		return m_Character->GetCharactertate();
	}

	inline void SetY(int x)
	{
		m_Character->SetY(x);
	}





	inline void Release()
	{
		SAFE_DELETE(s_this);
	}
	~GameManager();
};

