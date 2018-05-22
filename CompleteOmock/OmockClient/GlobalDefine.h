#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <map>
#include <vector>
#include "Bitmap.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "Scene.h"

#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }