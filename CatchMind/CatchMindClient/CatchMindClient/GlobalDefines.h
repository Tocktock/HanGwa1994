#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <stdafx.h>
#include <string>
#include <map>
#include <vector>
#include "GameManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "GameScene.h"
#include "LobbyScene.h"
#include "PACKET_HEADER.h"
#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }

#define SCREEN_WIDTH	1024
#define SCREEN_HEIGHT	768

#define WM_SOCKET (WM_USER+1)

enum SCENE_INDEX
{
	SCENE_INDEX_LOBBY,
	SCENE_INDEX_GAME,
	SCENE_INDEX_END
};

