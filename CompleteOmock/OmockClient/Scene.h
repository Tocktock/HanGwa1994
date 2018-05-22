#pragma once
#include <Windows.h>
#include "PACKET_HEADER.h"
class Scene
{
public:
	virtual void Init(HWND hWnd,SOCKET sock) = 0;
	virtual bool Input(WPARAM key) = 0;
	virtual void Update(float Elapse) = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual void Release() = 0;
	virtual bool RecvPacket(char*szBuf, int &retval, int &Buflen) = 0;
	virtual void SendPacket() =0;
	virtual void Chat(char key) = 0;
	Scene();
	virtual ~Scene();
};

