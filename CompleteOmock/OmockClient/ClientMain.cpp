#include <WinSock2.h>
#include <windows.h>
#include <map>
#include "GameManager.h"
#include "PACKET_HEADER.h"
using namespace std;

#define BUFSIZE 1024
#define WM_SOCKET (WM_USER+1)

#define WINWIDTHSIZE	1024
#define WINHEIGHTSIZE	768

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
bool SocketInit(HWND hWnd);
HINSTANCE g_hInst;
char g_szClassName[256] = "Hello World!!";
SOCKET g_Sock;



int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = g_szClassName;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindow(g_szClassName, g_szClassName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);


	/*WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;

	g_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_Sock == INVALID_SOCKET)
	{
		MessageBox(hWnd, "err on socket", "err", MB_ICONERROR);
		return -1;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(g_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	int retval = WSAAsyncSelect(g_Sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	if (retval == SOCKET_ERROR)
	{
		MessageBox(hWnd, "err on WSAAsyncSelect", "err", MB_ICONERROR);
		return -1;
	}
	*/

	GameManager::GetInstance()->Init(hWnd);

	while (true)
	{
		/// 메시지큐에 메시지가 있으면 메시지 처리
		if (PeekMessage(&Message, NULL, 0U, 0U, PM_REMOVE))
		{
			if (Message.message == WM_QUIT)
				break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{

			GameManager::GetInstance()->Update();

			if (GameManager::GetInstance()->GetGameState() == GAME_STATE_INITIAL_GAME)
			{
				if (!SocketInit(hWnd))
					return -1;
			}

			if (GameManager::GetInstance()->GetGameState() == GAME_STATE_EXIT)
				SendMessage(hWnd, WM_DESTROY, 0, 0);
		}
	}
	WSACleanup();

	return (int)Message.wParam;
}

class Player
{
public:
	int x;
	int y;

};

map<int, Player*> g_mapPlayer;
int g_iIndex = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{

	case WM_CHAR:

		GameManager::GetInstance()->ChatManager((TCHAR)wParam);
		return 0;

	case WM_SOCKET:
		GameManager::GetInstance()->ProcessSocketMessage(hWnd, iMessage, wParam, lParam);
		return 0;

	case WM_KEYDOWN:
		if (GameManager::GetInstance()->InputManager(wParam))
			GameManager::GetInstance()->SendPos();

		return 0;

	case WM_DESTROY:
		for (auto iter = g_mapPlayer.begin(); iter != g_mapPlayer.end(); iter++)
		{
			delete iter->second;
		}
		GameManager::GetInstance()->Release();
		ResourceManager::GetInstance()->Release();
		g_mapPlayer.clear();
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));

}

/*
void SendPos()
{
	PACKET_PUTXY_INFO packet;
	packet.header.wPacketIndex = PACKET_INDEX_PLAYER_MAP;
	packet.header.wLen = sizeof(packet);
	packet.bStoneColor = GameManager::GetInstance()->GetStoneColor();
	packet.PlayerXY = GameManager::GetInstance()->GetPointXY();
	send(g_Sock, (char*)&packet, sizeof(packet), 0);

}

void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	int retval = 0;

	if (WSAGETSELECTERROR(lParam))
	{
		int errcode = WSAGETSELECTERROR(lParam);
		return;
	}

	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
		char szBuf[512];

		retval = recv(wParam, szBuf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSAEWOULDBLOCK)
			{
				//cout << "err on recv!!" << endl;
			}
		}

		ProcessPacket(wParam, szBuf, retval);
	}
}

void ProcessPacket(SOCKET sock, char* szBuf, int len)
{
	PACKET_HEADER Header;
	memcpy(&Header, szBuf, sizeof(Header));

	switch (Header.wPacketIndex)
	{
	case PACKET_INDEX_LOGIN_RESULT:
		{
			PACKET_LOGIN_RESULT packet;
			memcpy(&packet, szBuf, sizeof(packet));
			GameManager::GetInstance()->SetStoneColor(packet.bStoneColor);
			GameManager::GetInstance()->SetMap(*packet.Map);
		}
		break;
	case PACKET_INDEX_PLAYER_MAP:
		{
			PACKET_PUTXY_INFO packet;
			memcpy(&packet, szBuf, sizeof(packet));
			GameManager::GetInstance()->SetStone(packet.PlayerXY, packet.bStoneColor);
		}
		break;

	case PACKET_INDEX_GAMERESULT:
		{
			
		}
		break;
	}
}
*/

bool SocketInit(HWND hWnd)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	g_Sock = socket(AF_INET, SOCK_STREAM, 0);
	if (g_Sock == INVALID_SOCKET)
	{
		MessageBox(hWnd, "err on socket", "err", MB_ICONERROR);
		return false;
	}

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(9000);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(g_Sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	int retval = WSAAsyncSelect(g_Sock, hWnd, WM_SOCKET, FD_READ | FD_CLOSE);

	if (retval == SOCKET_ERROR)
	{
		MessageBox(hWnd, "err on WSAAsyncSelect", "err", MB_ICONERROR);
		return false;
	}
	GameManager::GetInstance()->SocketInit(g_Sock);
	return true;
}
