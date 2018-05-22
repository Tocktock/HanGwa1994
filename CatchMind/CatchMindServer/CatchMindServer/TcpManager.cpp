#include <iostream>
#include <ctime>
#include "TcpManager.h"
#include "ErrorManager.h"

TcpManager* TcpManager::s_this = NULL;

TcpManager::TcpManager()
{
}


TcpManager::~TcpManager()
{
}

void TcpManager::Init()
{
	InitializeCriticalSection(&m_crit);
	m_iReadyCount = 0;
	m_iGameState = GAMESTATE_NOTSTART;
	m_iPlayerNumber = 0;
	m_iNowTurnPlayer = 255;
	m_newElapse = GetTickCount();
	m_oldElapse = GetTickCount();
	m_iTurnCount = 0;

}

bool TcpManager::RegistPlayerInfo(SOCKET Sock, SOCKADDR_IN addr, int addrlen, HANDLE hcp)
{

	int retval;

	PLAYERINFO *pInfo = new PLAYERINFO();
	pInfo->perHandleData = new PER_HANDLE_DATA();
	pInfo->perIOData = new PER_IO_DATA();


	pInfo->perHandleData->hClntSock = Sock;;
	memcpy(&pInfo->perHandleData->clntAddr, &addr, addrlen);

	CreateIoCompletionPort((HANDLE)Sock, hcp, (DWORD)pInfo->perHandleData, 0);

	ZeroMemory(&pInfo->perIOData->overlapped, sizeof(pInfo->perIOData->overlapped));
	pInfo->bufLen = 0;
	pInfo->perIOData->wsaBuf.buf = pInfo->perIOData->buffer;
	pInfo->perIOData->wsaBuf.len = BUFSIZE;
	pInfo->iGameState = PLAYER_STATE_LOBBY;

	m_mapPlayerInfo.insert(std::make_pair(pInfo->perHandleData->hClntSock, pInfo));

	PACKET_COMMAND packetCmd;
	packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
	packetCmd.Header.PacketLen = sizeof(packetCmd);

	packetCmd.CommandIndex = COMMAND_INDEX_LOG_RESULT;


	send(pInfo->perHandleData->hClntSock, (char*)&packetCmd, sizeof(packetCmd), 0); //로그인 확인

	DWORD flags = 0;
	DWORD recvBytes;

	retval = WSARecv(Sock, &pInfo->perIOData->wsaBuf, 1,
		&recvBytes, &flags, &pInfo->perIOData->overlapped, NULL);

	if (retval == SOCKET_ERROR)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			ErrorManager::GetInstance()->err_display("WSARecv()");

		}
		return false;
	}
	return true;
}


DWORD WINAPI TcpManager::WorkerThread(LPVOID arg)
{

	srand(time(NULL));
	int retval;
	HANDLE hcp = (HANDLE)arg;

	while (1)
	{
		//비동기 입출력 완룍 ㅣ다리기
		SOCKET Sock;

		LPPER_HANDLE_DATA perHandleData;
		LPPER_IO_DATA     perIOData;
		DWORD dwRecved;

		retval = GetQueuedCompletionStatus(hcp, &dwRecved, (LPDWORD)&perHandleData,
			(LPOVERLAPPED*)&perIOData, INFINITE);

		EnterCriticalSection(&m_crit);
		if (m_iGameState == GAMESTATE_INGAME)
		{
			m_fTime -= GetElapse();
		}

		LeaveCriticalSection(&m_crit);

		Sock = perHandleData->hClntSock;

		//PLAYERINFO pInfo_Thread = g_mapUserInfo[Sock];//정보를 얻어올 포인터
		PLAYERINFO pInfo_Thread;
		pInfo_Thread.bufLen = m_mapPlayerInfo[Sock]->bufLen;
		pInfo_Thread.perHandleData = m_mapPlayerInfo[Sock]->perHandleData;
		pInfo_Thread.perIOData = m_mapPlayerInfo[Sock]->perIOData;
		//	map<SOCKET, PLAYERINFO*> mapinfo;
		//	memcpy(&mapinfo, &g_mapUserInfo, sizeof(g_mapUserInfo));

		//비동기 입출력 결과 확인
		if (retval == 0 || dwRecved == 0)
		{

			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(perHandleData->hClntSock,
					&perIOData->overlapped, &temp1, FALSE, &temp2);
				ErrorManager::GetInstance()->err_display("WSAGetOverlappedResult");
			}

			closesocket(pInfo_Thread.perHandleData->hClntSock);
			printf("TCP 서버 클라이언트 종료 : ip 주소 = %s 포트번호 = %d\n",
				inet_ntoa(pInfo_Thread.perHandleData->clntAddr.sin_addr),
				ntohs(pInfo_Thread.perHandleData->clntAddr.sin_port));

			PACKET_QUIT_PLAYER packetQP;
			packetQP.header.HeaderIndex = PACKET_INDEX_QUIT_PLAYER;
			packetQP.header.PacketLen = sizeof(packetQP);
			packetQP.PlayerNumber = m_mapPlayerInfo[Sock]->iPlayerNumber;
			
			for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
			{
				if (iter->second->iPlayerNumber > m_mapPlayerInfo[Sock]->iPlayerNumber)
					iter->second->iPlayerNumber--;
			}

			if (m_mapPlayerInfo[Sock]->iGameState == PLAYER_STATE_LOBBY)
			{
				delete(m_mapPlayerInfo[Sock]->perHandleData);
				delete(m_mapPlayerInfo[Sock]->perIOData);
				delete(m_mapPlayerInfo[Sock]);

				m_mapPlayerInfo.erase(Sock);

				continue;

			}

			m_iPlayerNumber--;

			for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
			{
				if (iter->first == Sock)
					continue;

				send(iter->first, (char*)&packetQP, sizeof(packetQP), 0);
			}

			if(m_mapPlayerInfo[Sock]->iGameState == PLAYER_STATE_READY)
				m_iReadyCount--;

			delete(m_mapPlayerInfo[Sock]->perHandleData);
			delete(m_mapPlayerInfo[Sock]->perIOData);
			delete(m_mapPlayerInfo[Sock]);

			m_mapPlayerInfo.erase(Sock);

			if (m_mapPlayerInfo.size() == 1)
			{
				for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
				{
					m_iReadyCount = 0;
					m_iTurnCount = 0;

					iter->second->iGameState = PLAYER_STATE_NOT_READY;

				}
			}
			//delete pInfo_Thread;
			continue;
		}



		EnterCriticalSection(&m_crit);

		while (true)
		{
			memcpy(&pInfo_Thread.perIOData->buffer[pInfo_Thread.bufLen], //복사목적지
				pInfo_Thread.perIOData->wsaBuf.buf,									//복사해올곳
				dwRecved);																//복사량

			pInfo_Thread.bufLen += dwRecved;

			if (dwRecved > BUFSIZE) //많은 버퍼가 들어오면 어카지
			{

			}

			else if (dwRecved <= BUFSIZE)
			{

				if (dwRecved < sizeof(PACKET_HEADER)) //받은게 헤더량 보다 작으면 다시 recv 요청
				{
					retval = WSARecv(pInfo_Thread.perHandleData->hClntSock,
						&pInfo_Thread.perIOData->wsaBuf, 1,
						NULL, 0, &pInfo_Thread.perIOData->overlapped, NULL);


					return 0;
				}

				PACKET_HEADER header;
				memcpy(&header, pInfo_Thread.perIOData->buffer, sizeof(PACKET_HEADER));

				//이제 처리해야함.

				switch (header.HeaderIndex)
				{
				case PACKET_INDEX_COMMAND:
				{
					PACKET_COMMAND packetCmd;
					memcpy(&packetCmd, pInfo_Thread.perIOData->buffer, sizeof(packetCmd));
					CommandProcess(packetCmd.CommandIndex, pInfo_Thread);
				}
				break;

				case PACKET_INDEX_XY:
				{
					PACKET_XY packetXY;
					memcpy(&packetXY, pInfo_Thread.perIOData->buffer, sizeof(packetXY));
					for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
					{
						if (iter->first == Sock)
							continue;

						int retval;
						retval = send(iter->first, (char*)&packetXY, sizeof(packetXY), 0);

						if (retval == SOCKET_ERROR)
						{
							std::cout << retval << " " << GetLastError() << std::endl;

						}

					}
				}

				break;

				case PACKET_INDEX_NICKNAME:
				{
					PACKET_NICKNAME packetNick;
					memcpy(&packetNick, pInfo_Thread.perIOData->buffer, sizeof(packetNick));
					packetNick.PlayerNumber = m_iPlayerNumber;
					m_mapPlayerInfo[Sock]->iPlayerNumber = m_iPlayerNumber;
					m_mapPlayerInfo[Sock]->NickName = packetNick.str;
					m_mapPlayerInfo[Sock]->iGameState = PLAYER_STATE_ENTERED;
					m_iPlayerNumber++;


				}
				break;

				case PACKET_INDEX_PENINFO:
					PACKET_PENINFO packetPen;
					memcpy(&packetPen, pInfo_Thread.perIOData->buffer, sizeof(packetPen));

					for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
					{
						if (iter->first == pInfo_Thread.perHandleData->hClntSock)
							continue;

						send(iter->first, (char*)&packetPen, sizeof(packetPen), 0);
					}
					break;

				case PACKET_INDEX_STRING:
				{
					PACKET_STRING packetSTR;
					memcpy(&packetSTR, pInfo_Thread.perIOData->buffer, sizeof(packetSTR));
					packetSTR.PlayerNumber = m_mapPlayerInfo[Sock]->iPlayerNumber;

					if (CompareAnswer(packetSTR.str))
					{
						m_mapPlayerInfo[Sock]->iScore += 100;

						PACKET_CORRECT_ANSWER packetCorAns;
						packetCorAns.header.HeaderIndex = PACKET_INDEX_CORRECT_ANSWER;
						packetCorAns.header.PacketLen = sizeof(packetCorAns);
						packetCorAns.PlayerNumber = m_mapPlayerInfo[Sock]->iPlayerNumber;
						 
						if (m_fTime > 10)
							packetCorAns.Score = (int)m_fTime * 10;
						else
							packetCorAns.Score = 100;
											
						for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
						{
							send(iter->first, (char*)&packetCorAns, sizeof(packetCorAns), 0);
						}

						
					}

					for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
					{

						if (iter->first == pInfo_Thread.perHandleData->hClntSock)
							continue;

						send(iter->first, (char*)&packetSTR, packetSTR.header.PacketLen, 0);
					}
				}
				break;

				} //스위치 끝

				memcpy(pInfo_Thread.perIOData->buffer,
					&pInfo_Thread.perIOData->buffer[header.PacketLen],
					dwRecved - header.PacketLen);

				dwRecved -= header.PacketLen;

				if (dwRecved == 0)
				{
					ZeroMemory(&pInfo_Thread.perIOData->overlapped, sizeof(OVERLAPPED));
					pInfo_Thread.perIOData->wsaBuf.len = BUFSIZE;
					pInfo_Thread.perIOData->wsaBuf.buf = pInfo_Thread.perIOData->buffer;

					DWORD flags = 0;
					DWORD recvBytes;

					int retvaltemp;

					LeaveCriticalSection(&m_crit);

					retvaltemp = WSARecv(Sock, &m_mapPlayerInfo[Sock]->perIOData->wsaBuf,
						1, &recvBytes, &flags, &m_mapPlayerInfo[Sock]->perIOData->overlapped, NULL);
					break;
				
						/*if (retval == SOCKET_ERROR)
					{
						std::cout << retval << " " << WSAGetLastError() << std::endl;
					}*/
					break; // while - while 문에서 나옴
				}
			}

		} //while - while 끝

	} //while 끝


	return 0;
}


void TcpManager::CommandProcess(int command, PLAYERINFO &playerInfo)
{
	EnterCriticalSection(&m_crit);
	switch (command)
	{

	case COMMAND_INDEX_STATE_READY:

		m_mapPlayerInfo[playerInfo.perHandleData->hClntSock]->iGameState = PLAYER_STATE_READY;
		m_iReadyCount++;

		if (m_iReadyCount == m_mapPlayerInfo.size())
		{
			m_iTurnCount++; // 턴카운터

			m_iGameState = GAMESTATE_INGAME;
			m_fTime = 60.0f;
			int temp = rand() % (int)m_mapPlayerInfo.size();

			int temp1 = rand();
			int temp2 = rand();
			PACKET_COMMAND packetCmd, packetCmd2;
			packetCmd.CommandIndex = COMMAND_INDEX_STATE_NOT_MY_TURN;
			packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
			packetCmd.Header.PacketLen = sizeof(packetCmd);

			PACKET_ANSWER packetAns;
			packetAns.header.HeaderIndex = PACKET_INDEX_ANSWER;
			packetAns.header.PacketLen = sizeof(packetAns);
			packetAns.answerIndex = rand() % (int)QUESTION_INDEX_END;
			m_iAnswer = packetAns.answerIndex;

			AnswerMatch(m_iAnswer);

			m_iNowTurnPlayer = rand() % (int)m_mapPlayerInfo.size();

			packetCmd2.CommandIndex = COMMAND_INDEX_STATE_MY_TURN;
			packetCmd2.Header.HeaderIndex = PACKET_INDEX_COMMAND;
			packetCmd2.Header.PacketLen = sizeof(packetCmd2);

			for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
			{

				send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);

				iter->second->iGameState = PLAYER_STATE_NOT_MY_TURN;
				if (iter->second->iPlayerNumber == m_iNowTurnPlayer)
				{
					send((iter->first), (char*)&packetCmd2, sizeof(packetCmd2), 0);
					send(iter->first, (char*)&packetAns, sizeof(packetAns), 0);
					iter->second->iGameState = PLAYER_STATE_MY_TURN;
				}
			}

		}

		break;

	case COMMAND_INDEX_STATE_NOT_READY:
	{
		m_mapPlayerInfo[playerInfo.perHandleData->hClntSock]->iGameState = PLAYER_STATE_NOT_READY;

		m_iReadyCount--;

	}
	case COMMAND_INDEX_ASK_PLAYERINFO:
	{

		for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
		{
			if (iter->second->iGameState == PLAYER_STATE_LOBBY)
				continue;

			PACKET_NICKNAME packetNick;
			packetNick.header.HeaderIndex = PACKET_INDEX_NICKNAME;
			packetNick.header.PacketLen = sizeof(packetNick.header) + sizeof(packetNick.PlayerNumber) +
				m_mapPlayerInfo[iter->first]->NickName.length() + 1;

			packetNick.PlayerNumber = m_mapPlayerInfo[iter->first]->iPlayerNumber;

			strcpy(packetNick.str, m_mapPlayerInfo[iter->first]->NickName.c_str());

			for (auto iter2 = m_mapPlayerInfo.begin(); iter2 != m_mapPlayerInfo.end(); iter2++)
			{
				if (iter2->second->iGameState == PLAYER_STATE_LOBBY)
					continue;

				send(iter2->first, (char*)&packetNick, packetNick.header.PacketLen, 0);

			}

		}
	}
	break;

	case COMMAND_INDEX_NEXT_TURN:
	{


	
		m_fTime = 60.0f;

		m_iTurnCount++;

		if (m_iTurnCount == 10)
		{
			PACKET_COMMAND packetCmd;
			packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
			packetCmd.Header.PacketLen = sizeof(packetCmd);
			packetCmd.CommandIndex = COMMAND_INDEX_END_OF_GAME;

			for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
			{
				send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);
			}

			for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
			{
				iter->second->iGameState = PLAYER_STATE_ENTERED;
				iter->second->ReadyFlag = false;
				iter->second->iScore = 0;
				m_iReadyCount = 0;
				m_iTurnCount = 0;
			}
			break;
		}

	
		PACKET_COMMAND packetCmd, packetCmd2;
		packetCmd.CommandIndex = COMMAND_INDEX_STATE_NOT_MY_TURN;
		packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
		packetCmd.Header.PacketLen = sizeof(packetCmd);

	
		PACKET_ANSWER packetAns;
		packetAns.header.HeaderIndex = PACKET_INDEX_ANSWER;
		packetAns.header.PacketLen = sizeof(packetAns);
		packetAns.answerIndex = rand() % (int)QUESTION_INDEX_END;

		m_iAnswer = packetAns.answerIndex;

		AnswerMatch(m_iAnswer);

		m_iNowTurnPlayer++;

		if (m_iNowTurnPlayer == m_mapPlayerInfo.size())
			m_iNowTurnPlayer = 0;

		packetCmd2.CommandIndex = COMMAND_INDEX_STATE_MY_TURN;
		packetCmd2.Header.HeaderIndex = PACKET_INDEX_COMMAND;
		packetCmd2.Header.PacketLen = sizeof(packetCmd2);

		for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
		{

			send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);

			iter->second->iGameState = PLAYER_STATE_NOT_MY_TURN;
			if (iter->second->iPlayerNumber == m_iNowTurnPlayer)
			{
				send((iter->first), (char*)&packetCmd2, sizeof(packetCmd2), 0);
				send(iter->first, (char*)&packetAns, sizeof(packetAns), 0);
				iter->second->iGameState = PLAYER_STATE_MY_TURN;
			}
		}


	}
	break;
	}
	LeaveCriticalSection(&m_crit);
}
bool TcpManager::CompareAnswer(string Answer)
{
	if (m_AnswerStr == Answer)
	{
		m_AnswerStr = "";
		return true;

	}

	else
		return false;

}


void TcpManager::AnswerMatch(int Answer)
{
	switch (m_iAnswer)
	{
	case QUESTION_INDEX_COMPUTER:
	{
		m_AnswerStr = "컴퓨터";
	}
	break;
	case QUESTION_INDEX_TELEPHONE:
	{

		m_AnswerStr = "휴대폰";
	}
	break;
	case QUESTION_INDEX_DRUG:
	{

		m_AnswerStr = "알약";
	}
	break;
	case QUESTION_INDEX_TOOTH:
	{

		m_AnswerStr = "이빨";
	}
	break;

	case QUESTION_INDEX_SINGER:
	{

		m_AnswerStr = "가수";
	}
	break;

	case QUESTION_INDEX_SKY:
	{

		m_AnswerStr = "하늘";
	}
	break;
	case QUESTION_INDEX_BACKPACK:
	{

		m_AnswerStr = "가방";
	}
	break;

	case QUESTION_INDEX_CAT:
	{

		m_AnswerStr = "고양이";
	}
	break;
	}

}

void TcpManager::Release()
{
	for (auto iter = m_mapPlayerInfo.begin(); iter != m_mapPlayerInfo.end(); iter++)
	{
		delete iter->second->perHandleData;
		delete iter->second->perIOData;
		delete iter->second;
	}
	m_mapPlayerInfo.clear();

	DeleteCriticalSection(&m_crit);
}

float TcpManager::GetElapse()
{
	m_newElapse = GetTickCount();

	m_fElapse = (m_newElapse - m_oldElapse) / 1000.0f;

	m_oldElapse = m_newElapse;

	return m_fElapse;
}


