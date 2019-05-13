
#include <iostream>
#include <fstream>
#include<string.h>

#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3

using namespace std;

void Input();
void MainAlgoritm();
void Output();
bool IsWhite(int direction, int x, int y);
bool IsBlack(int direction, int x, int y);
ofstream fOut;
ifstream fIn;

int g_whiteCount = 0;
int g_blackCount = 0;
int g_kValue;
char g_board[21][21];
int g_boardCheck[21][21];


int main()
{
	char endCheck;

	fIn.open("go.inp");
	fOut.open("go.out");
	Input();
	MainAlgoritm();
	Output();
	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{
	fIn >> g_kValue;

	for (int i = 0; i < g_kValue; i++)
	{
		fIn >> g_board[i];
	}
}
void MainAlgoritm()
{
	int count = 0;
	for (int i = 0; i < g_kValue; i++)
	{
		for (int j = 0; j < g_kValue; j++)
		{
			count = 0;
			if (IsBlack(LEFT, j, i) == true && g_board[i][j] != 'B')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));
			if (IsBlack(RIGHT, j, i) == true && g_board[i][j] != 'B')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));
			if (IsBlack(UP, j, i) == true && g_board[i][j] != 'B')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));
			if (IsBlack(DOWN, j, i) == true && g_board[i][j] != 'B')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));

			if (count == 4)
			{
				g_board[i][j] = 'B';
				g_blackCount++;
			}

			count = 0;
			if (IsWhite(LEFT, j, i) == true && g_board[i][j] != 'W')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));
			if (IsWhite(RIGHT, j, i) == true && g_board[i][j] != 'W')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));
			if (IsWhite(UP, j, i) == true && g_board[i][j] != 'W')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));
			if (IsWhite(DOWN, j, i) == true && g_board[i][j] != 'W')
				count++;
			memset(g_boardCheck, 0, sizeof(g_boardCheck));

			if (count == 4)
			{
				g_board[i][j] = 'W';
				g_whiteCount++;
			}

		}
	}
}

void Output()
{
	fOut << g_blackCount << " " << g_whiteCount << endl;
}
bool IsWhite(int direction, int x, int y)
{
	int count = 0;

	if (x >= g_kValue || y >= g_kValue || x < 0 || y < 0)
		return true;

	if (g_boardCheck[y][x] == 1)
		return true;
	g_boardCheck[y][x] = 1;
	if (g_board[y][x] == 'B')
		return false;
	else if (g_board[y][x] == 'W')
		return true;

	else
	{
		if (direction == LEFT) //오른쪽 빼고
		{
			if (IsWhite(LEFT, x - 1, y) == true) //왼쪽
				count++;
			if (IsWhite(UP, x, y - 1) == true) //우ㅢ쪽
				count++;
			if (IsWhite(DOWN, x, y + 1) == true) //아래쪽
				count++;
		}
		else if (direction == RIGHT) //왼쪽 빼고
		{
			if (IsWhite(RIGHT, x + 1, y) == true) //오른쪽
				count++;
			if (IsWhite(UP, x, y - 1) == true) //위쪽
				count++;
			if (IsWhite(DOWN, x, y + 1) == true) //아래쪽
				count++;
		}
		else if (direction == UP) //아래쪽 빼고
		{
			if (IsWhite(LEFT, x - 1, y) == true) //왼쪽
				count++;
			if (IsWhite(UP, x, y - 1) == true) //위쪽
				count++;
			if (IsWhite(RIGHT, x + 1, y) == true) //오른쪽
				count++;
		}
		else if (direction == DOWN) //위쪽 빼고
		{
			if (IsWhite(LEFT, x - 1, y) == true) //왼쪽
				count++;
			if (IsWhite(RIGHT, x + 1, y) == true) //오른쪽
				count++;
			if (IsWhite(DOWN, x, y + 1) == true) //아래쪽
				count++;
		}
	}
	if (count == 3)
		return true;
	return false;
}
bool IsBlack(int direction, int x, int y)
{
	int count = 0;
	if (x >= g_kValue || y >= g_kValue || x < 0 || y < 0)
		return true;

	if (g_boardCheck[y][x] == 1)
		return true;
	g_boardCheck[y][x] = 1;

	if (g_board[y][x] == 'B')
		return true;
	else if (g_board[y][x] == 'W')
		return false;
	else
	{
		if (direction == LEFT) //오른쪽 빼고
		{
			if (IsBlack(LEFT, x - 1, y) == true) //왼쪽
				count++;
			if (IsBlack(UP, x, y - 1) == true) //우ㅢ쪽
				count++;
			if (IsBlack(DOWN, x, y + 1) == true) //아래쪽
				count++;
		}
		else if (direction == RIGHT) //왼쪽 빼고
		{
			if (IsBlack(RIGHT, x + 1, y) == true) //오른쪽
				count++;
			if (IsBlack(UP, x, y - 1) == true) //위쪽
				count++;
			if (IsBlack(DOWN, x, y + 1) == true) //아래쪽
				count++;
		}
		else if (direction == UP) //아래쪽 빼고
		{
			if (IsBlack(LEFT, x - 1, y) == true) //왼쪽
				count++;
			if (IsBlack(UP, x, y - 1) == true) //위쪽
				count++;
			if (IsBlack(RIGHT, x + 1, y) == true) //오른쪽
				count++;
		}
		else if (direction == DOWN) //위쪽 빼고
		{
			if (IsBlack(LEFT, x - 1, y) == true) //왼쪽
				count++;
			if (IsBlack(RIGHT, x + 1, y) == true) //오른쪽
				count++;
			if (IsBlack(DOWN, x, y + 1) == true) //아래쪽
				count++;
		}
	}
	if (count == 3)
		return true;
	return false;
}