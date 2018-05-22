#include <WinSock2.h>
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <map>
#include <string>
#include "PACKET_HEADER.h"
using namespace std;

#define BUFSIZE 1024
#define WM_SOCKET (WM_USER+1)

#define BLACKTURN	true
#define WHITETURN	false

class UserInfo
{
public:
	SOCKET sock;
	char userBuf[BUFSIZE];
	bool bStoneColor;
	int len;
	int index;
};

enum GAMESTATE
{
	GAMESTATE_NOT_START,
	GAMESTATE_START,
	GAMESTATE_INGAME,
	GAMESTATE_END_OF_GAME,
	GAMESTATE_INDEX_END
};

int g_iGameState = GAMESTATE_NOT_START;
map<SOCKET, UserInfo*> g_mapUser;
int	g_iIndex = 0;
bool g_bStoneColor = BLACKSTONE;
bool g_bNowTurn = BLACKTURN;
WORD g_wMap[19][19];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);
bool ProcessPacket(UserInfo * pInfo, char* szBuf, int &len);
void err_display(int errcode);
void err_display(char* szMsg);

SORT_OF_CALCULATION Calculation();
bool CheckPossibilityPut();

int main(int argc, char* argv[])
{
	srand(GetTickCount());

	int retval;

	WNDCLASS WndClass;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = NULL;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = "WSAAsyncSelectServer";
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&WndClass))
		return -1;

	HWND hWnd = CreateWindow("WSAAsyncSelectServer", "TCP 서버", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, NULL, NULL);
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
	{
		cout << "err on socket" << endl;
		return -1;
	}

	retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on WSAAsyncSelect" << endl;
		return -1;
	}

	//bind
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	if (retval == SOCKET_ERROR)
	{
		cout << "err on bind" << endl;
		return -1;
	}

	//listen
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR)
	{
		cout << "err on listen" << endl;
		return -1;
	}

	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			g_wMap[i][j] = EMPTYBLOCK;
		}
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	WSACleanup();

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_KEYDOWN:

	case WM_SOCKET:
		ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen = 0;
	int retval = 0;


	if (WSAGETSELECTERROR(lParam))
	{
		int err_code = WSAGETSELECTERROR(lParam);
		err_display(WSAGETSELECTERROR(lParam));
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		// 접속을 하면 서버에서 유저 정보를 저장하는 작업.
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			int err_code = WSAGetLastError();

			if (err_code != WSAEWOULDBLOCK)
			{
				err_display("err on accept");
			}
			return;
		}
		printf("[TCP 서버] 클라이언트 접속 : IP 주소 = %s , 포트번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
		if (retval == SOCKET_ERROR)
		{
			cout << "err on WSAAsyncSelect!!" << endl;
		}

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (client_sock == iter->first)
				return;
		}

		if (g_bStoneColor == WHITESTONE)
			g_bStoneColor = BLACKSTONE;

		else
			g_bStoneColor = WHITESTONE;

		UserInfo* pInfo = new UserInfo();
		pInfo->sock = client_sock;
		pInfo->index = g_iIndex++;
		pInfo->bStoneColor = g_bStoneColor;
		pInfo->len = 0;
		g_mapUser.insert(make_pair(client_sock, pInfo));

		PACKET_LOGIN_RESULT packet;
		packet.header.wPacketIndex = PACKET_INDEX_LOGIN_RESULT;
		packet.header.bTrashPacket = false;
		packet.header.wLen = sizeof(packet);
		packet.bStoneColor = g_bStoneColor;
		memcpy(packet.Map, g_wMap, sizeof(g_wMap));

		send(client_sock, (char*)&packet, sizeof(packet), 0);

		if (g_iIndex == 2) //유저가 2명
		{
			g_iGameState = GAMESTATE_INGAME;

			PACKET_COMMAND packetCmd;
			packetCmd.header.bTrashPacket = false;
			packetCmd.header.wLen = sizeof(packetCmd);
			packetCmd.header.wPacketIndex = PACKET_INDEX_COMMAND;
			packetCmd.ucCommand = COMMAND_START_GAME;

			for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
			{
				send(iter->first, (char*)&packetCmd, sizeof(packetCmd),0);
			}
		}

		break;
	}
	case FD_READ:
	{
		char szBuf[BUFSIZE];

		retval = recv(wParam, szBuf, BUFSIZ, 0);

		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				cout << "err on recv!!" << endl;
			}
			break;
		}
		
		while (true)
		{
			UserInfo *pInfo = g_mapUser[wParam];

			if (ProcessPacket(pInfo, szBuf, retval) == false)
			{
				SendMessage(hWnd, WM_SOCKET, wParam, lParam);
				break;
			}

			else if (pInfo->len > sizeof(PACKET_HEADER))
				continue;

			break;
		}

		break;
	}
	case FD_CLOSE:



		break;
	}
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[오류]%s", lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(char* szMsg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s]%s\n", szMsg, lpMsgBuf);
	LocalFree(lpMsgBuf);
}


bool ProcessPacket(UserInfo * pInfo, char* szBuf, int &len)
{
	memcpy(pInfo->userBuf, szBuf, len);

	pInfo->len += len;
	len = 0;

	if (pInfo->len < sizeof(PACKET_HEADER))
		return false;

	PACKET_HEADER header;
	memcpy(&header, szBuf, sizeof(header));

	if (header.bTrashPacket == true)
		return true;

	if (pInfo->len < header.wLen)
		return false;



	switch (header.wPacketIndex)
	{
	case PACKET_INDEX_PLAYER_MAP:
	{
		if (g_iGameState == GAMESTATE_END_OF_GAME || g_iGameState == GAMESTATE_NOT_START)
			break;

		PACKET_PUTXY_INFO packet;
		memcpy(&packet, szBuf, sizeof(packet));

		if (g_bNowTurn == BLACKTURN && packet.bStoneColor != BLACKSTONE)
			break;

		else if (g_bNowTurn == WHITETURN && packet.bStoneColor != WHITESTONE)
			break;

		if (packet.bStoneColor == WHITESTONE)
		{
			g_wMap[packet.PlayerXY.y][packet.PlayerXY.x] = STONE_COLOR_WHITE;
			g_bNowTurn = BLACKTURN;
		}

		else if (packet.bStoneColor == BLACKSTONE)
		{
			g_wMap[packet.PlayerXY.y][packet.PlayerXY.x] = STONE_COLOR_BLACK;
			g_bNowTurn = WHITETURN;
		}


		if (Calculation() == SORT_OF_CALCULATION_33)
		{

			g_wMap[packet.PlayerXY.y][packet.PlayerXY.x] = STONE_COLOR_EMPTY;

			if (g_bNowTurn == WHITETURN)
				g_bNowTurn = BLACKTURN;

			else
				g_bNowTurn = WHITETURN;

			PACKET_COMMAND packetCmd;
			packetCmd.header.bTrashPacket = false;
			packetCmd.header.wLen = sizeof(packetCmd);
			packetCmd.header.wPacketIndex = PACKET_INDEX_COMMAND;

			if (packet.bStoneColor == WHITESTONE)
			{
				packetCmd.ucCommand = COMMAND_RETRY_PUT_33_WHITE;
			}

			else
				packetCmd.ucCommand = COMMAND_RETRY_PUT_33_BLACK;


			for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
			{
				send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);
			}
			break;
		}


		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (pInfo->sock == iter->first)
				continue;

			send(iter->first, (char*)&packet, sizeof(packet), 0);
		}

		if (Calculation() == SORT_OF_CALCULATION_VICTORY)
		{
			for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
			{

				PACKET_COMMAND packetCmd;

				packetCmd.header.bTrashPacket = false;
				packetCmd.header.wLen = sizeof(packetCmd);
				packetCmd.header.wPacketIndex = PACKET_INDEX_COMMAND;

				if (packet.bStoneColor == STONE_COLOR_BLACK)
				{
					packetCmd.ucCommand = COMMAND_BLACKWIN;
				}

				else if (packet.bStoneColor == STONE_COLOR_WHITE)
				{
					packetCmd.ucCommand = COMMAND_WHITEWIN;
				}

				send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);
			}
			g_iGameState = GAMESTATE_END_OF_GAME;

			break;

		}


		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			PACKET_COMMAND packetCmd;

			packetCmd.header.bTrashPacket = false;
			packetCmd.header.wLen = sizeof(packetCmd);
			packetCmd.header.wPacketIndex = PACKET_INDEX_COMMAND;

			if (g_bNowTurn == BLACKSTONE)
				packetCmd.ucCommand = COMMAND_BLACKTURN;
			else
				packetCmd.ucCommand = COMMAND_WHITETURN;


			int ret = send(iter->first, (char*)&packetCmd, sizeof(packetCmd), 0);

			if (ret == -1 || ret == 0)
				assert(0);

		}


	}
	break;

	case PACKET_INDEX_STRING:
	{
		PACKET_STRING packetSTR;
		memcpy(&packetSTR, &pInfo->userBuf, header.wLen);

		for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
		{
			if (iter->first == pInfo->sock)
				continue;

			send(iter->first, (char*)&packetSTR, packetSTR.header.wLen, 0);

		}
		break;

	}
		case PACKET_INDEX_COMMAND:
		{
			PACKET_COMMAND packetCmd;
			memcpy(&packetCmd, &pInfo->userBuf, header.wLen);

			if (packetCmd.ucCommand == COMMAND_RESET_GAME)
			{
				g_iGameState = GAMESTATE_INGAME;
				for (int y = 0; y < OMOCK_WHNUM; y++)
				{
					for (int x = 0; x < OMOCK_WHNUM; x++)
					{
						g_wMap[y][x] = EMPTYBLOCK;
					}


					for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
					{
						if (iter->second->bStoneColor == BLACKSTONE)
							iter->second->bStoneColor = WHITESTONE;
						else
							iter->second->bStoneColor = BLACKSTONE;
					
					}
					g_bNowTurn = BLACKSTONE;

				}

				for (auto iter = g_mapUser.begin(); iter != g_mapUser.end(); iter++)
				{
					if (iter->first == pInfo->sock)
						continue;

					send(iter->first, (char*)&packetCmd, packetCmd.header.wLen, 0);
				}
			}
			break;
		} 
	
	}//스윗치 끗


	memcpy(pInfo->userBuf, &pInfo->userBuf[header.wLen], pInfo->len - header.wLen);
	pInfo->len -= header.wLen;

	return true;
}

bool CheckPossibilityPut()
{
	return false;
}

SORT_OF_CALCULATION Calculation()
{
	//33 검사

	for (int value = STONE_COLOR_WHITE; value <= STONE_COLOR_BLACK; value++)
	{
		for (int y = 0; y < 19; y++)
		{
			for (int x = 0; x < 19; x++)
			{
				//위 && 오른쪽 위
				if (x + 2 < 19 && y - 2 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y - 1][x + 1] == value &&
						g_wMap[y - 2][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}

				//오른쪽 위 && 오른쪾
				if (x + 2 < 19 && y - 2 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value &&
						g_wMap[y - 1][x + 1] == value &&
						g_wMap[y - 2][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}

				//오른쪽 && 오른쪽 아래
				if (x + 2 < 19 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value &&
						g_wMap[y + 1][x + 1] == value &&
						g_wMap[y + 2][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 오른쪽 아래 && 아래
				if (x + 2 < 19 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value &&
						g_wMap[y + 1][x + 1] == value &&
						g_wMap[y + 2][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 아래 && 왼쪽 아래
				if (x - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value &&
						g_wMap[y + 1][x - 1] == value &&
						g_wMap[y + 2][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}

				// 왼쪽 아래 && 왼쪽
				if (x - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value &&
						g_wMap[y + 1][x - 1] == value &&
						g_wMap[y + 2][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 왼쪽 && 왼쪽 위 
				if (x - 2 > 0 && y - 2 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value &&
						g_wMap[y - 1][x - 1] == value &&
						g_wMap[y - 2][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				//왼쪽 위 && 위
				if (x - 2 > 0 && y - 2 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y - 1][x - 1] == value &&
						g_wMap[y - 2][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}

				//위 && 오른쪽
				if (x + 2 < 19 && y - 2 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 오른쪽 && 아래
				if (x + 2 < 19 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 아래 && 왼쪽
				if (x - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 왼쪽 && 위
				if (x - 2 > 0 && y - 2 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}

				//위 && 오른쪽 아래
				if (x + 2 < 19 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y + 1][x + 1] == value &&
						g_wMap[y + 1][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				//오른쪽 위 && 아래
				if (x + 2 < 19 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x + 1] == value &&
						g_wMap[y - 2][x + 2] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value)
						return SORT_OF_CALCULATION_33;
				}
				//오른쪽 && 왼쪽 아래
				if (x + 2 < 19 && x - 2 > 0 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value &&
						g_wMap[y + 1][x - 1] == value &&
						g_wMap[y + 2][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}

				//오른쪽 아래 && 왼쪽	
				if (x + 2 < 19 && x - 2 > 0 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value &&
						g_wMap[y + 1][x + 1] == value &&
						g_wMap[y + 2][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 아래 && 왼쪽 위
				if (x + 2 < 19 && x - 2 > 0 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value &&
						g_wMap[y - 1][x - 1] == value &&
						g_wMap[y - 2][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				//왼쪽 아래 && 위 
				if (x + 2 < 19 && x - 2 > 0 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y + 1][x - 1] == value &&
						g_wMap[y + 2][x - 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				// 왼쪽 && 오른쪽 위
				if (x + 2 < 19 && x - 2 > 0 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value &&
						g_wMap[y - 1][x + 1] == value &&
						g_wMap[y - 2][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
				//왼쪽 위 && 오른쪽
				if (x + 2 < 19 && x - 2 > 0 && y - 2 > 0 && y + 2 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x - 1] == value &&
						g_wMap[y - 2][x - 2] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value)
						return SORT_OF_CALCULATION_33;
				}
			}
		}
				

	}
	//육목 검사
	for (int value = STONE_COLOR_WHITE; value <= STONE_COLOR_BLACK; value++)
	{
		for (int y = 0; y < 19; y++)
		{
			for (int x = 0; x < 19; x++)
			{
				//오른쪽으로
				if (x + 5 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value &&
						g_wMap[y][x + 3] == value &&
						g_wMap[y][x + 4] == value &&
						g_wMap[y][x + 5] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}
				//왼쪽으로
				if (x - 5 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value &&
						g_wMap[y][x - 3] == value &&
						g_wMap[y][x - 4] == value &&
						g_wMap[y][x - 5] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}
				//위로
				if (y - 5 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y - 3][x] == value &&
						g_wMap[y - 4][x] == value &&
						g_wMap[y - 5][x] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}
				//아래로
				if (y + 5 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value &&
						g_wMap[y + 3][x] == value &&
						g_wMap[y + 4][x] == value &&
						g_wMap[y + 5][x] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}

				//오른쪽 위로
				if (x + 5 < 19 && y - 5 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x + 1] == value &&
						g_wMap[y - 2][x + 2] == value &&
						g_wMap[y - 3][x + 3] == value &&
						g_wMap[y - 4][x + 4] == value &&
						g_wMap[y - 5][x + 5] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}

				//오른쪽 아래로
				if (x + 5 < 19 && y + 5 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x + 1] == value &&
						g_wMap[y + 2][x + 2] == value &&
						g_wMap[y + 3][x + 3] == value &&
						g_wMap[y + 4][x + 4] == value &&
						g_wMap[y + 5][x + 5] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}

				//왼쪽 아래로
				if (x - 5 > 0 && y + 5 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x - 1] == value &&
						g_wMap[y + 2][x - 2] == value &&
						g_wMap[y + 3][x - 3] == value &&
						g_wMap[y + 4][x - 4] == value &&
						g_wMap[y + 5][x - 5] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}

				//왼쪽 위로
				if (x - 5 > 0 && y - 5 < 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x - 1] == value &&
						g_wMap[y - 2][x - 2] == value &&
						g_wMap[y - 3][x - 3] == value &&
						g_wMap[y - 4][x - 4] == value &&
						g_wMap[y - 5][x - 5] == value)
						return SORT_OF_CALCULATION_SIXSTONES;
				}
			}
		}
				
		
	}
	//승리 검사
	for (int value = STONE_COLOR_WHITE; value <= STONE_COLOR_BLACK; value++)
	{
		for (int y = 0; y < 19; y++)
		{
			for (int x = 0; x < 19; x++)
			{
				//오른쪽으로
				if (x + 4 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x + 1] == value &&
						g_wMap[y][x + 2] == value &&
						g_wMap[y][x + 3] == value &&
						g_wMap[y][x + 4] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}
				//왼쪽으로
				if (x - 4 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y][x - 1] == value &&
						g_wMap[y][x - 2] == value &&
						g_wMap[y][x - 3] == value &&
						g_wMap[y][x - 4] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}
				//위로
				if (y - 4 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x] == value &&
						g_wMap[y - 2][x] == value &&
						g_wMap[y - 3][x] == value &&
						g_wMap[y - 4][x] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}
				//아래로
				if (y + 4 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x] == value &&
						g_wMap[y + 2][x] == value &&
						g_wMap[y + 3][x] == value &&
						g_wMap[y + 4][x] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}

				//오른쪽 위로
				if (x + 4 < 19 && y - 4 > 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x + 1] == value &&
						g_wMap[y - 2][x + 2] == value &&
						g_wMap[y - 3][x + 3] == value &&
						g_wMap[y - 4][x + 4] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}
				//오른쪽 아래로
				if (x + 4 < 19 && y + 4 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x + 1] == value &&
						g_wMap[y + 2][x + 2] == value &&
						g_wMap[y + 3][x + 3] == value &&
						g_wMap[y + 4][x + 4] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}

				//왼쪽 아래로
				if (x - 4 > 0 && y + 4 < 19)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y + 1][x - 1] == value &&
						g_wMap[y + 2][x - 2] == value &&
						g_wMap[y + 3][x - 3] == value &&
						g_wMap[y + 4][x - 4] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}

				//왼쪽 위로
				if (x - 4 > 0 && y - 4 < 0)
				{
					if (g_wMap[y][x] == value &&
						g_wMap[y - 1][x - 1] == value &&
						g_wMap[y - 2][x - 2] == value &&
						g_wMap[y - 3][x - 3] == value &&
						g_wMap[y - 4][x - 4] == value)
						return SORT_OF_CALCULATION_VICTORY;
				}
		
			}

		}
				
	}
	return SORT_OF_CALCULATION_ORDINARY;

}