#pragma once
#include "StructHeader.h"
#include <Windows.h>
#include <map>
#include <vector>
#include <string>
#include "PACKET_HEADER.h"

using namespace std;

class TcpManager
{
	map<SOCKET, PLAYERINFO*> m_mapPlayerInfo;
	//vector<PLAYERINFO*> m_vecPlayerInfo;
	string m_AnswerStr;
	int    m_iAnswer;
	CRITICAL_SECTION m_crit;
	static TcpManager* s_this;
	int	m_iReadyCount;
	int m_iGameState;
	int m_iPlayerNumber;
	int m_iNowTurnPlayer;

	int m_iTurnCount;

	float m_fTime;
	float m_fElapse;
	DWORD m_oldElapse;
	DWORD m_newElapse;

	TcpManager();

public:
	static TcpManager* GetInstance()
	{
		if (s_this == NULL)
			s_this = new TcpManager();

		return s_this;
	}

	void Init();
	bool RegistPlayerInfo(SOCKET Sock,SOCKADDR_IN addr,int addrlen,HANDLE hcp);
	bool CompareAnswer(string Answer);
	void AnswerMatch(int Answer);
	void CommandProcess(int command, PLAYERINFO &playerInfo);
	
	float GetElapse();

	DWORD WINAPI WorkerThread(LPVOID arg);
	//void Init(SOCKET Sock);
	//	void ProcessCommand(int Command);

	void Release();
	virtual ~TcpManager();
};

