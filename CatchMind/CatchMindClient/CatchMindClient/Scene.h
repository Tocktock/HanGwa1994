#pragma once
#include <Windows.h>


class Scene
{
public:
	virtual void Init(HWND hWnd, SOCKET sock) = 0;
	virtual bool Input(char key) = 0;
	virtual bool LbuttonDown(int x, int y) = 0;
	virtual bool LbuttonUp(int x, int y) = 0;
	virtual void Update(float Elapse) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual void Release() = 0;
	virtual void ProcessRecvPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)= 0;
	virtual bool RecvPacket(char*szBuf, int retval, int Buflen) = 0;
	virtual void SendPacket(char* buf,int packetLen) = 0;
	virtual void Chat(char key) = 0;
	Scene();
	virtual ~Scene();
};
