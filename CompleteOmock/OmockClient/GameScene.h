#pragma once
#include <Windows.h>
#include <string>
#include "PACKET_HEADER.h"
#include "Scene.h"

using namespace std;
#define STONEWIDTH	32
#define	STONEHEIGHT	32

#define ADJUSTX 20
#define ADJUSTY 20

enum GAME_STATE_OMOCK
{
	GAME_STATE_OMOCK_BEFORE_GAME,
	GAME_STATE_OMOCK_INGAME,
	GAME_STATE_OMOCK_WIN,
	GAME_STATE_OMOCK_DEFEAT,
	GAME_STATE_OMOCK_END
};

class Bitmap;
class GameScene : public Scene
{
private:
	Bitmap	*m_pBitmapBoard;
	Bitmap	*m_pBitmapStone[STONE_COLOR_END];
	Bitmap	*m_pBitmapSelect;

	char	m_Buf[BUFSIZE];
	int		m_iBufLen;
	int		m_iPacketByte;
	string	m_ChatSTR[19];
	char	m_ChatBuf[256];
	string	m_ShowMyChat;
	int		m_ChatBufCount;
	bool	m_ChatFlag;
	int		m_iSTRCount;

	bool	m_bResetFlag;
	WORD m_wMap[OMOCK_WHNUM][OMOCK_WHNUM];
	
	SOCKET m_Sock;

	POINTXY m_MyPt;
	bool m_bStoneColor;
	bool m_bMyTurn;
	WORD m_wGameState;
public:

	void SetStone(POINTXY pt, bool StoneColor);
	void ArrangeChatSTR();

	virtual void Init(HWND hWnd,SOCKET sock);
	virtual bool Input(WPARAM key);
	virtual void Update(float Elapse);
	virtual void Draw(HDC hdc);
	virtual void Release();
	virtual bool RecvPacket(char*szBuf, int &retval, int &Buflen);
	virtual void SendPacket();
	virtual void Chat(char key);
	void CommandProcess(unsigned char Command);



	inline void SetMap(WORD* Map)
	{
		memcpy(m_wMap, Map, sizeof(m_wMap));
	}

	inline POINTXY GetPointXY()
	{
		return m_MyPt;
	}

	inline void SetStoneColor(bool StoneColor)
	{
		m_bStoneColor = StoneColor;
	}

	inline bool GetStoneColor()
	{
		return m_bStoneColor;
	}

	GameScene();
	virtual ~GameScene();
};

