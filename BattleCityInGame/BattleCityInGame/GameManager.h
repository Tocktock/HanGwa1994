#pragma once
#include <Windows.h>
#include <vector>
#include "Tank.h"
#include "Missile.h"

#define BLOCKWIDTH 33
#define BLOCKHEIGHT 25
#define MAPWIDTHNUM 14
#define MAPHEIGHTNUM 16
#define EMPTYBLOCK 1000

#define MAPWIDTH	BLOCKWIDTH*MAPWIDTHNUM
#define MAPHEIGHT	BLOCKHEIGHT*MAPHEIGHTNUM

#define GAMESTART true
#define EXIT	  false
using namespace std;

enum GAME_STATE
{
	GAME_STATE_MENU,
	GAME_STATE_INGAME,
	GAME_STATE_DEAD,
	GAME_STATE_EXIT,
	GAME_STATE_END
};

class Block;
class Missile;
class Tank;
class Bitmap;
class Missile;
class ExplosionEffect;
class EnemyTank;
class ElseEffect;
class GameManager
{
private:


	HDC		m_hMemDC;
	HBITMAP m_hBitMap;
	HBITMAP m_hOldBit;
	HWND	m_hWnd;
	
	///////////////////////////////////////////////////////////////////////////비트맵
	Bitmap*		m_pBitmapBlock[15];                  //15s는 블럭 개수
	Bitmap*		m_pBitmapTank[8];					//8은 탱크 이밎 ㅣ개수
	Bitmap*		m_pBitmapMissile;
	Bitmap*		m_pBitmapExplosion[5];				//5는 폭발이미지 개수
	Bitmap*		m_pBitmapEnemyTank[12];				//enemy탱크 이미지 갯수
	Bitmap*		m_pBitmapShield[2];
	Bitmap*		m_pBitmapIcon[2];
	//////////////////////////////////////////////////////////////////// 탱크객체관련
	Tank		*m_pMyTank;
	EnemyTank	*m_pEnemyTank;
	int m_iFormer_Direction;
	int m_iFormer_Direction_Enemy;
	int m_iNumEnemy;
	int m_iLife;

	/////////////////////////////////////////////////////////////////////벡터관련
	vector<Block*> m_vecBlock;
	vector<Missile*> m_vecEnemyMissile;
	vector<ExplosionEffect*> m_vecExplosionEffect;
	vector<EnemyTank*> m_vecEnemyTank;
	vector<ElseEffect*> m_vecShield;
	///////////////////////////////////////////////////////////////////// 미사일 관련
	
	Missile		*m_pMyTankMissileFirst;
	Missile		*m_pMyTankMissileSecond;
	Missile		*m_pEnemyMissile;

	bool		m_bFirstBulletFired;
	bool		m_bSecondBulletFired;
	///////////////////////////////////////////////////////////////////// 이펙트 관련

	float			m_Explosion_float;
	int				m_iSpeed_Tank;
	/////////////////////////////////////////////////////////////////////키 입력 관련
	bool m_bLeftKeyDown;
	bool m_bRightKeyDown;
	bool m_bUpKeyDown;
	bool m_bDownKeyDown;
	bool m_bSpaceKeyDown;

	bool m_bUpKeyDown_Menu;
	bool m_bDownKeyDown_Menu;
	bool m_bEnterKeyDown_Menu;
	///////////////////////////////////////////////////////////////////////블럭관련

	int			m_iMap[MAPHEIGHTNUM][MAPWIDTHNUM];
	int			m_iBlockType;
	///////////////////////////////////////////////////////////////////////메뉴 관련

	float m_fElapse_Menu;
	bool m_ActionFlag;
	float m_iTemp_menu;
	float m_arrfTextGradient[3];
	//////////////////////////////////////////////////////////////////////기타
	GAME_STATE	m_eGameState;
	DWORD		m_dwElapseFormer;
	DWORD		m_dwElapseLater;
	float		m_fElapseValue;
	float		m_fElapseSapceKey;
	RECT m_WallRt[4];						//면이 4방면이기 때문에
	bool m_bMenuSelect;
	bool m_bFirstTank;
	int m_iScore;
	int m_iTemp_Gameover;
	float m_fElpase_ResetTimer;
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
	/////////////////////////////////////////////////////////////////////////이펙트관련
	void Operate_Explosion_Bullet(float x,float y);

	void Operate_Explosion_Tank(float x, float y);
	void Operate_Initial_Tank(Tank *pTank);
	void Check_Initailized_Tank();
	void Operate_Slide_Tank();
	//////////////////////////////////////////////////////////////////////////탱크 움직임 관련

	void KeyManager();
	void MoveRight();
	void MoveLeft();
	void MoveUp();
	void MoveDown();
	void Interpolate_Tank();
	inline void ResetMove()
	{
		m_pMyTank->ResetMove();
	}
	/////////////////////////////////////////////////////////////////////////// 탱크 발사 관련.
	void MissileFire();
	void FireMove();
	void RemoveMissile();

	///////////////////////////////////////////////////////////////////////////충돌 관련
	void MissileCollision();
	////////////////////////////////////////////////////////////////////////////변환함수
	int GetPixelPosX(int TileX);
	int GetPixelPosY(int TileY);
	int GetTilePosX(int PixelX);
	int GetTilePosY(int PixelY);
	/////////////////////////////////////////////////////////////////////////////적탱크
	void MoveRight_EnemyTank(EnemyTank *pEnemyTank);
	void MoveLeft_EnemyTank(EnemyTank *pEnemyTank);
	void MoveUp_EnemyTank(EnemyTank *pEnemyTank);
	void MoveDown_EnemyTank(EnemyTank *pEnemyTank);
	void MissileCollision_EnemyTank();
	void MissileFire_EnemyTank(EnemyTank *pEnemyTank);

	void NewTank();
	void AutoMove_EnemyTank();
	//////////////////////////////////////////////////////////////////////////////기타

	void SaveData(HWND hWnd);
	void LoadData(HWND hWnd);
	void Init(HWND hWnd);
	void ConstructBlock();
	void DeleteBlock();
	void OperateLbutton(int x, int y);
	void OperateRbutton(int x, int y);
	void Update();
	void SetRt();
	void Gameover();
	void KeyManager_Menu();
	void Draw();
	void Draw_Menu();
	void Reset_Game();
	bool IntersectRect_Wall(RECT rt);

	inline void BlockTypeSet(int x)
	{
		m_iBlockType = x;
	}

	inline void SelectChange_Menu()
	{
		if (m_bMenuSelect == GAMESTART)
			m_bMenuSelect = EXIT;

		else if (m_bMenuSelect == EXIT)
			m_bMenuSelect = GAMESTART;
	}

	inline void GameStart()
	{
		m_eGameState = GAME_STATE_INGAME;
	}
	
	void Release();
	~GameManager();
};

