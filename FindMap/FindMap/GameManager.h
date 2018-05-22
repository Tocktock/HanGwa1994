#pragma once
#include <Windows.h>
#include <vector>
#include "Macro.h"
using namespace std;

#define MINEBLOCK 12

enum GAME_STATE
{
	GAME_STATE_WAIT,
	GAME_STATE_ONE,
};

enum GAME_MODE
{
	GAME_MODE_EASY,
	GAME_MODE_NORMAL,
	GAME_MODE_HARD,
	GAME_MODE_END

};
class Piece;
class Bitmap;
class GameManager
{
private:

	vector<Piece*>		m_vecPiece;

	Bitmap*				m_pBitmapBlock;
	GAME_STATE			m_eGameState;
	bool				WhosTurn = true;
	int					m_gameLevelX;
	int					m_gameLevelY;
	int					m_MineNumber;
	bool				m_END;
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


	void Init(HWND hWnd);


	void Draw(HDC hdc);
	void OperateLbutton(HWND hWnd, int x, int y);
	void OperateRbutton(HWND hWnd, int x, int y);
	void OperateLRbutton(HWND hWnd, int x, int y);
	void ReturnState();
	void SetGameMode(GAME_MODE type);
	void VictoryCheck(HWND hWnd);

	void EraseMap();
	
	inline bool GameEnd()
	{
		m_END = true;
	}

	inline int GameLevelX()
	{
		return m_gameLevelX;
	}

	inline int GameLevelY()
	{
		return m_gameLevelY;
	}

	inline void MakeEndTrue()
	{
		m_END = true;
	}


	static int GetPixelPosX(int TileX);
	static int GetPixelPosY(int TileY);
	static int GetTilePosX(int PixelX);
	static int GetTilePosY(int PixelY);


	~GameManager();
};

