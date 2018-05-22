#pragma once
#include "Scene.h"
#include <string>
#include <vector>
#include <map>
#include "PACKET_HEADER.h"
using namespace std;

#define WM_SOCKET (WM_USER+1)

#define PEN_WIDTH_THIN	1
#define PEN_WIDTH_MEDIUM 3
#define PEN_WIDTH_THICK	5

class PLAYERINFO_SCENE
{
public:
	string  PlayerNickName;
	string	PlayerChat;
	float	ChatElapse;
	bool	ChatFlag;
	int		PlayerScore;
	int		PlayerRank;

	bool	AnswerBoxFlag;
	float	fElapaseAnswerBox;

};


struct RGBStruct
{
	int red, green, blue;
};

enum GAMESTATE_SCENE
{
	GAMESTATE_SCENE_ENTER,
	GAMESTATE_SCENE_READY,
	GAMESTATE_SCENE_INGAME,
	GAMESTATE_SCENE_MY_TURN,
	GAMESTATE_SCENE_NOT_MY_TURN,
	GAMESTATE_SCENE_HAVE_CORRECT_ANSWER,
	GAMESTATE_SCENE_WIN,
	GAMESTATE_SCENE_DEFEAT,
	GAMESTATE_SCENE_END
};

class GameScene : public Scene
{
private:
	int		m_iRetval;
	int		m_iBuflen;
	char	m_buf[512];
	string	m_ChatSTR[19];
	char	m_ChatBuf[256];
	string	m_ShowMyChat;
	int		m_ChatBufCount;
	bool	m_ChatFlag;
	int		m_iSTRCount;

	RGBStruct m_rgb;
	int		  m_iPenColor;
	int		  m_iPenThickness;

	RECT	m_rtColor[PEN_INFO_WHITE+1];
	RECT	m_rtEraser;
	RECT	m_rtEraseAll;
	RECT	m_rtReadyBox;
	RECT	m_rtThickness[3]; //펜 두께는 3종류
	CRITICAL_SECTION m_crit;

	map<unsigned char, PLAYERINFO_SCENE *> m_mapPlayerInfo;
	vector<POINT> m_vecPoint;
	vector<RGBStruct> m_vecRGB;

	
	int		m_drawCount;

	SOCKET	m_sock;
	HWND	m_hWnd;
	HDC		m_backDC;
	unsigned char m_drawColor;
	POINT	m_ptMouse;
	bool	m_LbuttonDownFlag;
	bool	m_DrawFlag;
	int		m_PictureBoard[768][1024];
	RECT	m_DrawingRect;

	int		m_iAnswer;
	string	m_AnswerStr;
	bool	m_ResetFlag;
	int		m_iGameState;

	float	m_fGameTime;
	bool	m_GameTimeFlag;

	bool	m_endFlag;
	float	m_fElapseEnd;

	int		m_iPlayerNum;
	int		m_iPlayerMyNum;
	
	float	m_fNextTurnElapse;
	bool	m_NextTurnCommandFlag;
	int		m_winnerPlayerNum;
	
	int		m_iGameTurn;

	Bitmap *m_pBitmapNum[10]; //0~9
	Bitmap *m_pBitmapCorrectAnswer;
	Bitmap *m_pBitmapIngameBox;
	Bitmap *m_pBitmapReadyBox;
	Bitmap *m_pBitmapPlayBox;
	Bitmap *m_pBitmapBack;
	Bitmap *m_pBitmapChatBox[2]; //left,right
	Bitmap *m_pBitmapWordBack;
public:

	void	NextTurnCommand(float elapse);
	void	PaintPicture();
	void	ProcessPenInfo(int PenInfo);
	void	AnswerDraw();

	void	Reset();

	virtual void Init(HWND hWnd, SOCKET sock);
	virtual bool Input(char key);
	virtual bool LbuttonDown(int x, int y);
	virtual bool LbuttonUp(int x, int y);
	virtual void Update(float Elapse);
	virtual void Draw(HDC hdc);
	virtual void Release();
	virtual void ProcessRecvPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual bool RecvPacket(char*szBuf, int retval, int Buflen);
	virtual void SendPacket(char* buf, int packetLen);
	virtual void Chat(char key);

	void RankCalculation();
	void ProcessCommand(int Command);
	GameScene();
	virtual ~GameScene();
};

