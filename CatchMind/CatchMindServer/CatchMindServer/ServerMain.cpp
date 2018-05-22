#pragma comment (lib , "ws2_32")
#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <string>
#include <time.h>
#include "StructHeader.h"
#include "TcpManager.h"
#include "PACKET_HEADER.h"
#include "ErrorManager.h"
#define SERVERPORT	9000
#define BUFSIZE		512

using namespace std;
//
//void CommandProcess(int command,PLAYERINFO &playerInfo);
//
//unsigned char g_questionAnswer;
//
//std::map<SOCKET, PLAYERINFO*> g_mapUserInfo;
//
//int	g_iReadyCount = 0;
//int g_iGameState = GAMESTATE_NOTSTART;
//int g_iPlayerNumber = 0;
//int g_iNowTurnPlayer = 255;
//
//int g_iAnswer;
//string g_Answer;
//
//CRITICAL_SECTION g_crit;
DWORD WINAPI WorkerThread(LPVOID arg);
//
//
//bool CompareAnswer(string Answer);
//
//void AnswerMatch(int Answer);

int main(int argc, char *argv[])
{
	int retval;
	//InitializeCriticalSection(&g_crit); //크리티컬 섹션 초기화
	TcpManager::GetInstance()->Init();


	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	HANDLE hcp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hcp == NULL)
		return 1;

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	//cpu * x 만큼 작업자 슬드 생성

	HANDLE hThread;
	for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; i++)
	{
		hThread = CreateThread(NULL, 0, 
			WorkerThread, hcp, 0, NULL);
		if (hThread == NULL)
			return 1;
		CloseHandle(hThread);
	}

	//socket

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		ErrorManager::GetInstance()->err_quit("socket()");

	//bind();

	SOCKADDR_IN serveraddr;

	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);

	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
		ErrorManager::GetInstance()->err_quit("bind()");

	//listen

	retval = listen(listen_sock, SOMAXCONN);

	if (retval == SOCKET_ERROR)
		ErrorManager::GetInstance()->err_quit("listen");

	SOCKET client_sock;

	SOCKADDR_IN clientaddr;
	int addrlen;
	DWORD recvbytes, flags;

	while (1)
	{
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);

		bool optval = true;
		setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&optval, sizeof(optval));

		if (client_sock == INVALID_SOCKET)
		{
			ErrorManager::GetInstance()->err_display("accept()");
			break;
		}

		printf("TCP 서버 클라이언트 접속 IP : %s 포트번호 : %d \n", inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port));

		if (TcpManager::GetInstance()->RegistPlayerInfo(client_sock, clientaddr, addrlen, hcp) == false)
			continue;


		//PLAYERINFO *pInfo = new PLAYERINFO();
		//pInfo->perHandleData = new PER_HANDLE_DATA();
		//pInfo->perIOData = new PER_IO_DATA();
		//if (pInfo == NULL)
		//	break;

		//pInfo->perHandleData->hClntSock = client_sock;;
		//memcpy(&pInfo->perHandleData->clntAddr, &clientaddr, addrlen);

		//CreateIoCompletionPort((HANDLE)client_sock, hcp, (DWORD)pInfo->perHandleData, 0);

		//ZeroMemory(&pInfo->perIOData->overlapped, sizeof(pInfo->perIOData->overlapped));
		//pInfo->bufLen = 0;
		//pInfo->perIOData->wsaBuf.buf = pInfo->perIOData->buffer;
		//pInfo->perIOData->wsaBuf.len = BUFSIZE;
		//pInfo->iGameState = PLAYER_STATE_LOBBY;

		//g_mapUserInfo.insert(std::make_pair(pInfo->perHandleData->hClntSock,pInfo));

		//PACKET_COMMAND packetCmd;
		//packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
		//packetCmd.Header.PacketLen = sizeof(packetCmd);

		//packetCmd.CommandIndex = COMMAND_INDEX_LOG_RESULT;
		//send(pInfo->perHandleData->hClntSock,(char*)&packetCmd , sizeof(packetCmd) , 0); //로그인 확인
		
		////비동기 입출력 시작c
		//flags = 0;
		//DWORD recvBytes;
		//retval = WSARecv(client_sock, &pInfo->perIOData->wsaBuf, 1,
		//	&recvBytes, &flags, &pInfo->perIOData->overlapped, NULL);

		//if (retval == SOCKET_ERROR)
		//{
		//	if (WSAGetLastError() != ERROR_IO_PENDING)
		//	{
		//		ErrorManager::GetInstance()->err_display("WSARecv()");

		//	}
		//	continue;
		//}
	}

	//윈속 종료
	//for (auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end(); iter++)
	//{
	//	delete iter->second->perHandleData;
	//	delete iter->second->perIOData;
	//	delete iter->second;
	//}

	//DeleteCriticalSection(&g_crit);
	TcpManager::GetInstance()->Release();
	//g_mapUserInfo.clear();
	WSACleanup();

	return 0;
}

//
//DWORD WINAPI WorkerThread(LPVOID arg)
//{
//	int retval;
//	HANDLE hcp = (HANDLE)arg;
//	
//	while (1)
//	{
//		//비동기 입출력 완룍 ㅣ다리기
//		SOCKET Sock;
//
//		LPPER_HANDLE_DATA perHandleData;
//		LPPER_IO_DATA     perIOData;
//		DWORD dwRecved;
//
//		retval = GetQueuedCompletionStatus(hcp, &dwRecved, (LPDWORD)&perHandleData,
//			(LPOVERLAPPED*)&perIOData, INFINITE);
//
//		
//		Sock = perHandleData->hClntSock;
//
//		//PLAYERINFO pInfo_Thread = g_mapUserInfo[Sock];//정보를 얻어올 포인터
//		PLAYERINFO pInfo_Thread;
//		pInfo_Thread.bufLen = g_mapUserInfo[Sock]->bufLen;
//		pInfo_Thread.perHandleData = g_mapUserInfo[Sock]->perHandleData;
//		pInfo_Thread.perIOData = g_mapUserInfo[Sock]->perIOData;
//													  //	map<SOCKET, PLAYERINFO*> mapinfo;
//	//	memcpy(&mapinfo, &g_mapUserInfo, sizeof(g_mapUserInfo));
//
//		//비동기 입출력 결과 확인
//		if (retval == 0 || dwRecved == 0)
//		{
//			if (retval == 0)
//			{
//				DWORD temp1, temp2;
//				WSAGetOverlappedResult(perHandleData->hClntSock,
//					&perIOData->overlapped, &temp1, FALSE, &temp2);
//				ErrorManager::GetInstance()->err_display("WSAGetOverlappedResult");
//			}
//
//			closesocket(pInfo_Thread.perHandleData->hClntSock);
//			printf("TCP 서버 클라이언트 종료 : ip 주소 = %s 포트번호 = %d\n",
//				inet_ntoa(pInfo_Thread.perHandleData->clntAddr.sin_addr),
//				ntohs(pInfo_Thread.perHandleData->clntAddr.sin_port));
//			//delete pInfo_Thread;
//			continue;
//		}
//
//		EnterCriticalSection(&g_crit);
//
//		while(true)
//		{
//			memcpy(&pInfo_Thread.perIOData->buffer[pInfo_Thread.bufLen], //복사목적지
//				pInfo_Thread.perIOData->wsaBuf.buf,									//복사해올곳
//				dwRecved);																//복사량
//
//			pInfo_Thread.bufLen += dwRecved;
//
//			if (dwRecved > BUFSIZE)
//			{
//
//			}
//
//			else if (dwRecved <= BUFSIZE)
//			{
//
//				if (dwRecved < sizeof(PACKET_HEADER)) //받은게 헤더량 보다 작으면 다시 recv 요청
//				{
//					retval = WSARecv(pInfo_Thread.perHandleData->hClntSock,
//						&pInfo_Thread.perIOData->wsaBuf, 1,
//						NULL, 0, &pInfo_Thread.perIOData->overlapped, NULL);
//
//
//					return 0;
//				}
//
//				PACKET_HEADER header;
//				memcpy(&header, pInfo_Thread.perIOData->buffer, sizeof(PACKET_HEADER));
//
//				//이제 처리해야함.
//
//				switch (header.HeaderIndex)
//				{
//					case PACKET_INDEX_COMMAND:
//					{
//						PACKET_COMMAND packetCmd;
//						memcpy(&packetCmd, pInfo_Thread.perIOData->buffer, sizeof(packetCmd));
//						CommandProcess(packetCmd.CommandIndex, pInfo_Thread);
//					}
//					break;
//
//					case PACKET_INDEX_XY:
//					{
//						PACKET_XY packetXY;
//						memcpy(&packetXY, pInfo_Thread.perIOData->buffer, sizeof(packetXY));
//						for (auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end(); iter++)
//						{
//							if (iter->first == Sock)
//								continue;
//
//							int retval;
//							retval = send(iter->first, (char*)&packetXY, sizeof(packetXY), 0);
//							
//							if (retval == SOCKET_ERROR)
//							{
//								std::cout << retval << " " << GetLastError() << std::endl;
//
//							}
//						
//						}
//					}
//					
//					break;
//
//					case PACKET_INDEX_NICKNAME:
//					{
//						PACKET_NICKNAME packetNick;
//						memcpy(&packetNick, pInfo_Thread.perIOData->buffer, sizeof(packetNick));
//						packetNick.PlayerNumber = g_iPlayerNumber;
//						g_mapUserInfo[Sock]->iPlayerNumber = g_iPlayerNumber;
//						g_mapUserInfo[Sock]->NickName = packetNick.str;
//						g_mapUserInfo[Sock]->iGameState = PLAYER_STATE_ENTERED;
//						g_iPlayerNumber++;
//						
//
//					}
//					break;
//
//					case PACKET_INDEX_PENINFO:
//						PACKET_PENINFO packetPen;
//						memcpy(&packetPen, pInfo_Thread.perIOData->buffer, sizeof(packetPen));
//
//						for (auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end(); iter++)
//						{
//							if (iter->first == pInfo_Thread.perHandleData->hClntSock)
//								continue;
//
//							send(iter->first, (char*)&packetPen, sizeof(packetPen), 0);
//						}
//						break;
//
//					case PACKET_INDEX_STRING :
//						{
//							PACKET_STRING packetSTR;
//							memcpy(&packetSTR, pInfo_Thread.perIOData->buffer, sizeof(packetSTR));
//							packetSTR.PlayerNumber = g_mapUserInfo[Sock]->iPlayerNumber;
//							
//							if (CompareAnswer(packetSTR.str))
//							{
//								g_mapUserInfo[Sock]->iScore += 100;
//
//								PACKET_CORRECT_ANSWER packetCorAns;
//								packetCorAns.header.HeaderIndex = PACKET_INDEX_CORRECT_ANSWER;
//								packetCorAns.header.PacketLen = sizeof(packetCorAns);
//								packetCorAns.PlayerNumber = g_mapUserInfo[Sock]->iPlayerNumber;
//								for (auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end(); iter++)
//								{
//									send(iter->first, (char*)&packetCorAns, sizeof(packetCorAns), 0);
//								}
//							}
//
//							for (auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end(); iter++)
//							{
//
//								if (iter->first == pInfo_Thread.perHandleData->hClntSock)
//									continue;
//
//								send(iter->first, (char*)&packetSTR, packetSTR.header.PacketLen, 0);
//							}
//						}
//						break;
//
//				} //스위치 끝
//
//				memcpy(pInfo_Thread.perIOData->buffer,
//					&pInfo_Thread.perIOData->buffer[header.PacketLen],
//					dwRecved - header.PacketLen);
//
//				dwRecved -= header.PacketLen;
//
//				if (dwRecved == 0)
//				{
//					ZeroMemory(&pInfo_Thread.perIOData->overlapped, sizeof(OVERLAPPED));
//					pInfo_Thread.perIOData->wsaBuf.len = BUFSIZE;
//					pInfo_Thread.perIOData->wsaBuf.buf = pInfo_Thread.perIOData->buffer;
//
//					DWORD flags = 0;
//					DWORD recvBytes;
//
//					int retval;
//
//					LeaveCriticalSection(&g_crit);
//					retval = WSARecv(Sock, &g_mapUserInfo[Sock]->perIOData->wsaBuf,
//						1, &recvBytes, &flags, &g_mapUserInfo[Sock]->perIOData->overlapped, NULL);
//
//					if (retval == WSAGetLastError())
//					{
//						std::cout << retval << " " << WSAGetLastError() << std::endl;
//					}
//					break; // while - while 문에서 나옴
//				}
//			}
//
//		} //while - while 끝
//		
//	} //while 끝
//
//
//	return 0;
//}



//
//void CommandProcess(int command, PLAYERINFO &playerInfo)
//{
//	EnterCriticalSection(&g_crit);
//	switch(command)
//	{
//
//	case COMMAND_INDEX_STATE_READY:
//		g_mapUserInfo[playerInfo.perHandleData->hClntSock]->iGameState = PLAYER_STATE_READY;
//		
//		g_iReadyCount++;
//		
//		if (g_iReadyCount == g_mapUserInfo.size())
//		{
//			g_iGameState = GAMESTATE_INGAME;
//
//			int temp = rand() % (int)g_mapUserInfo.size();
//			
//			PACKET_COMMAND packetCmd,packetCmd2;
//			packetCmd.CommandIndex = COMMAND_INDEX_STARTGAME;
//			packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
//			packetCmd.Header.PacketLen = sizeof(packetCmd);
//			
//			PACKET_ANSWER packetAns;
//			packetAns.header.HeaderIndex = PACKET_INDEX_ANSWER;
//			packetAns.header.PacketLen = sizeof(packetAns);
//			packetAns.answerIndex = rand() % (int)QUESTION_INDEX_END;
//			g_iAnswer = packetAns.answerIndex;
//
//			AnswerMatch(g_iAnswer);
//			
//			g_iNowTurnPlayer = rand() % (int)g_mapUserInfo.size();
//
//			packetCmd2.CommandIndex = COMMAND_INDEX_STATE_MY_TURN;
//			packetCmd2.Header.HeaderIndex = PACKET_INDEX_COMMAND;
//			packetCmd2.Header.PacketLen = sizeof(packetCmd2);
//
//			for(auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end();iter++)
//			{
//
//				send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);
//
//				iter->second->iGameState = PLAYER_STATE_NOT_MY_TURN;
//				if (iter->second->iPlayerNumber == g_iNowTurnPlayer)
//				{
//					send((iter->first), (char*)&packetCmd2, sizeof(packetCmd2), 0);
//					send(iter->first, (char*)&packetAns, sizeof(packetAns), 0);
//					iter->second->iGameState = PLAYER_STATE_MY_TURN;
//				}
//			}	
//
//		}
//
//		break;
//
//	case COMMAND_INDEX_STATE_NOT_READY:
//	{
//		g_mapUserInfo[playerInfo.perHandleData->hClntSock]->iGameState = PLAYER_STATE_NOT_READY;
//
//		g_iReadyCount--;
//
//	}
//	case COMMAND_INDEX_ASK_PLAYERINFO:
//		{
//		
//			for (auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end(); iter++)
//			{
//				if (iter->second->iGameState == PLAYER_STATE_LOBBY)
//					continue;
//
//				PACKET_NICKNAME packetNick;
//				packetNick.header.HeaderIndex = PACKET_INDEX_NICKNAME;
//				packetNick.header.PacketLen = sizeof(packetNick.header) + sizeof(packetNick.PlayerNumber) +
//					g_mapUserInfo[iter->first]->NickName.length()+1;
//
//				packetNick.PlayerNumber = g_mapUserInfo[iter ->first]->iPlayerNumber;
//
//				strcpy(packetNick.str, g_mapUserInfo[iter->first]->NickName.c_str());
//						
//				for (auto iter2 = g_mapUserInfo.begin(); iter2 != g_mapUserInfo.end(); iter2++)
//				{
//					if (iter2->second->iGameState == PLAYER_STATE_LOBBY)
//						continue;
//
//					send(iter2->first, (char*)&packetNick, packetNick.header.PacketLen, 0);
//
//				}
//
//			}
//		}
//		break;
//
//	case COMMAND_INDEX_NEXT_TURN:
//		{
//			PACKET_COMMAND packetCmd, packetCmd2;
//			packetCmd.CommandIndex = COMMAND_INDEX_STATE_NOT_MY_TURN;
//			packetCmd.Header.HeaderIndex = PACKET_INDEX_COMMAND;
//			packetCmd.Header.PacketLen = sizeof(packetCmd);
//
//			PACKET_ANSWER packetAns;
//			packetAns.header.HeaderIndex = PACKET_INDEX_ANSWER;
//			packetAns.header.PacketLen = sizeof(packetAns);
//			packetAns.answerIndex = rand() % (int)QUESTION_INDEX_END;
//
//			g_iAnswer = packetAns.answerIndex;
//
//			AnswerMatch(g_iAnswer);
//
//			g_iNowTurnPlayer++;
//
//			if (g_iNowTurnPlayer == g_mapUserInfo.size())
//				g_iNowTurnPlayer = 0;
//
//			packetCmd2.CommandIndex = COMMAND_INDEX_STATE_MY_TURN;
//			packetCmd2.Header.HeaderIndex = PACKET_INDEX_COMMAND;
//			packetCmd2.Header.PacketLen = sizeof(packetCmd2);
//
//			for (auto iter = g_mapUserInfo.begin(); iter != g_mapUserInfo.end(); iter++)
//			{
//
//				send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);
//
//				iter->second->iGameState = PLAYER_STATE_NOT_MY_TURN;
//				if (iter->second->iPlayerNumber == g_iNowTurnPlayer)
//				{
//					send((iter->first), (char*)&packetCmd2, sizeof(packetCmd2), 0);
//					send(iter->first, (char*)&packetAns, sizeof(packetAns), 0);
//					iter->second->iGameState = PLAYER_STATE_MY_TURN;
//				}
//			}
//
//			
//		}
//		break;
//	}
//	LeaveCriticalSection(&g_crit);
//}
//bool CompareAnswer(string Answer)
//{
//	if (g_Answer == Answer)
//	{
//		g_Answer = "";
//		return true;
//
//	}
//	
//	else
//		return false;
//
//}
//
//
//void AnswerMatch(int Answer)
//{
//	switch (g_iAnswer)
//	{
//	case QUESTION_INDEX_COMPUTER:
//	{
//		g_Answer = "컴퓨터";
//	}
//	break;
//	case QUESTION_INDEX_TELEPHONE:
//	{
//
//		g_Answer = "휴대폰";
//	}
//	break;
//	case QUESTION_INDEX_DRUG:
//	{
//
//		g_Answer = "알약";
//	}
//	break;
//	case QUESTION_INDEX_TOOTH:
//	{
//
//		g_Answer = "이빨";
//	}
//	break;
//
//	case QUESTION_INDEX_SINGER:
//	{
//
//		g_Answer = "가수";
//	}
//	break;
//
//	case QUESTION_INDEX_SKY:
//	{
//
//		g_Answer = "하늘";
//	}
//	break;
//	case QUESTION_INDEX_BACKPACK:
//	{
//
//		g_Answer = "가방";
//	}
//	break;
//
//	case QUESTION_INDEX_CAT:
//	{
//
//		g_Answer = "고양이";
//	}
//	break;
//	}
//
//}

DWORD WINAPI WorkerThread(LPVOID hcp)
{
	TcpManager::GetInstance()->WorkerThread(hcp);
	return 0;
}