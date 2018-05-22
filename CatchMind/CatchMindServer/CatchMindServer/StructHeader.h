#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <string>

using namespace std;

#define BUFSIZE 512

enum GAMESTATE_SERVER
{
	GAMESTATE_INGAME,
	GAMESTATE_END_OF_GAME,
	GAMESTATE_NOTSTART,
	GAMESTATE_END
};
enum PLAYER_STATE
{
	PLAYER_STATE_LOBBY,
	PLAYER_STATE_ENTERED,
	PLAYER_STATE_READY,
	PLAYER_STATE_NOT_READY,
	PLAYER_STATE_MY_TURN,
	PLAYER_STATE_NOT_MY_TURN,
	PLAYER_STATE_QUIT,
	PLAYER_STATE_END
};

enum QUESTION_EXAMPLE
{
	QUESTION_COMPUTER = 100,
	QUESTION_TELEPHONE,
	QUESTION_DRUG,
	QUESTION_TOOTH,
	QUESTION_SINGER,
	QUESTION_SKY,
	QUESTION_BACKPACK,
	QUESTION_CAT,
	QUESTION_END
};


typedef struct			// 소켓 정보를 구조체화
{
	SOCKET      hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct			// 소켓의 버퍼 정보를 구조체화
{
	OVERLAPPED overlapped;
	CHAR       buffer[BUFSIZE];
	WSABUF     wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

class PLAYERINFO
{
public:
	LPPER_HANDLE_DATA	perHandleData;
	LPPER_IO_DATA		perIOData;
	DWORD				bufLen;
	string	NickName;
	bool	ReadyFlag;
	int		iGameState;
	int		iScore;
	int		iPlayerNumber;
};