//	stdafx.h : include file for standard system include files,
//		or project specific include files that are used frequently, but
//		are changed infrequently
//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <winsock2.h>


//////////////////////////////////////////////////////////////////////////
//	상수 정의

#define BUFSIZE				100
#define SAFE_DELETE( p ) { if( p ) { delete ( p ); ( p ) = NULL; } }
#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p ); ( p ) = NULL; } }


//////////////////////////////////////////////////////////////////////////
//	구조체 선언

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


//////////////////////////////////////////////////////////////////////////
//	함수 원형 선언

void ErrorHandling(LPCSTR pszMessage);
