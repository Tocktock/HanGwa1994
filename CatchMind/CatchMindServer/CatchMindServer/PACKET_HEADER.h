#pragma once
#include <Windows.h>
#define BUFSIZE 512

#pragma pack(1)


enum PEN_INFO
{
	PEN_INFO_BLACK,
	PEN_INFO_RED,
	PEN_INFO_BLUE,
	PEN_INFO_GREEN,
	PEN_INFO_YELLOW,
	PEN_INFO_WHITE,
	PEN_INFO_THIN,
	PEN_INFO_MEDIUM,
	PEN_INFO_THICK,
	PEN_INFO_ERASE,
	PEN_INFO_ERASEALL,
	PEN_INFO_END
};

enum PACKET_INDEX
{
	PACKET_INDEX_CORRECT_ANSWER,
	PACKET_INDEX_STRING,
	PACKET_INDEX_NICKNAME,
	PACKET_INDEX_COMMAND,
	PACKET_INDEX_XY,
	PACKET_INDEX_PENINFO,
	PACKET_INDEX_ANSWER,
	PACKET_INDEX_QUIT_PLAYER,
	PACKET_INDEX_END
};
enum QUESTION_INDEX
{
	QUESTION_INDEX_COMPUTER,
	QUESTION_INDEX_TELEPHONE,
	QUESTION_INDEX_DRUG,
	QUESTION_INDEX_TOOTH,
	QUESTION_INDEX_SINGER,
	QUESTION_INDEX_SKY,
	QUESTION_INDEX_BACKPACK,
	QUESTION_INDEX_CAT,
	QUESTION_INDEX_END
};



enum COMMAND_INDEX
{
	COMMAND_INDEX_NEXT_TURN,
	COMMAND_INDEX_ASK_PLAYERINFO,
	COMMAND_INDEX_LOG_RESULT,
	COMMAND_INDEX_VICTORY,
	COMMAND_INDEX_DEFEAT,
	COMMAND_INDEX_RESET,
	COMMAND_INDEX_EXIT,
	COMMAND_INDEX_READY,
	COMMAND_INDEX_STARTGAME,
	COMMAND_INDEX_STATE_NOT_MY_TURN,
	COMMAND_INDEX_STATE_MY_TURN,
	COMMAND_INDEX_STATE_READY,
	COMMAND_INDEX_STATE_NOT_READY,
	COMMAND_INDEX_STATE_QUIT,
	COMMAND_INDEX_GAME_QUIT,
	COMMAND_INDEX_END_OF_GAME,
	COMMAND_INDEX_END
};

struct PACKET_HEADER
{
	unsigned char HeaderIndex;
	WORD PacketLen;
};

struct PACKET_COMMAND
{
	PACKET_HEADER Header;
	unsigned char CommandIndex;
};

struct PACKET_PENINFO
{
	PACKET_HEADER Header;
	unsigned char penIndex;
};

struct PACKET_XY
{
	PACKET_HEADER Header;
	WORD x;
	WORD y;
};

struct PACKET_PAINT_COLOR
{
	PACKET_HEADER header;
	unsigned char Color;
};



struct PACKET_NICKNAME
{
	PACKET_HEADER header;
	unsigned char PlayerNumber;
	char str[256];
};

struct PACKET_ANSWER
{
	PACKET_HEADER header;
	unsigned char answerIndex;
};

struct PACKET_STRING
{
	PACKET_HEADER header;
	unsigned char PlayerNumber;
	char str[256];
};

struct PACKET_CORRECT_ANSWER
{
	PACKET_HEADER header;
	unsigned char PlayerNumber;
	WORD Score;
};

struct PACKET_QUIT_PLAYER
{
	PACKET_HEADER header;
	unsigned char PlayerNumber;
	
};
#pragma pack()