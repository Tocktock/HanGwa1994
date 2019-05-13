

#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#define ALICE true
#define COMPUTER false
#define BOTTLE_A 0
#define BOTTLE_B 1
#define BOTTLE_C 2

using namespace std;

void Input();
void MainAlgoritm();
void Output();
int Check(int coinNum, int bottleNum, int bottle[3], bool turn);
ofstream fOut;
ifstream fIn;

int g_inputNum;
int g_coinCase[128][128][128];
int g_inputCoin[3];
int g_answer;

int main()
{
	fIn.open("coin.inp");
	fOut.open("coin.out");
	int caseNum = 0;

	fIn >> caseNum;
	memset(g_coinCase, 0, sizeof(g_coinCase));
	for (int i = 0; i < caseNum; i++)
	{
		Input();
		MainAlgoritm();
		Output();
	}

	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{
	fIn >> g_inputCoin[0];
	fIn >> g_inputCoin[1];
	fIn >> g_inputCoin[2];
}
void MainAlgoritm()
{
	if (g_coinCase[g_inputCoin[0]][g_inputCoin[1]][g_inputCoin[2]] == 0)
		Check(0, 0, g_inputCoin, ALICE);

	g_answer = g_coinCase[g_inputCoin[0]][g_inputCoin[1]][g_inputCoin[2]];
}
void Output()
{
	fOut << "(" << g_inputCoin[0] << " " << g_inputCoin[1] << " " << g_inputCoin[2] << ")" << " : " << g_answer << endl;
}

int Check(int coinNum, int bottleNum, int bottle[3], bool turn) //지면 -1 이기면 1
{
	int bottleInfo[3] = { bottle[0],bottle[1],bottle[2] };

	bottleInfo[bottleNum] -= coinNum;

	if (bottleInfo[0] < 0 || bottleInfo[1] < 0 || bottleInfo[2] < 0)
	{
		bottleInfo[bottleNum] = 0;
	}


	if (bottleInfo[0] <= 0 && bottleInfo[1] <= 0 && bottleInfo[2] <= 0)
	{
		return -1;
	}

	if (g_coinCase[bottle[0]][bottle[1]][bottle[2]] != 0)
		return g_coinCase[bottle[0]][bottle[1]][bottle[2]];

	int isWin[9];
	int count = 0;
	memset(isWin, -1, sizeof(isWin));

	if (turn == ALICE)
	{
		if (bottleInfo[BOTTLE_A] > 0)
		{
			if (isWin[0] = Check(1, BOTTLE_A, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[1] = Check(2, BOTTLE_A, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[2] = Check(3, BOTTLE_A, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}

		}
		if (bottleInfo[BOTTLE_B] > 0)
		{

			if (isWin[3] = Check(1, BOTTLE_B, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[4] = Check(2, BOTTLE_B, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[5] = Check(3, BOTTLE_B, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
		}
		if (bottleInfo[BOTTLE_C] > 0)
		{
			if (isWin[6] = Check(1, BOTTLE_C, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[7] = Check(2, BOTTLE_C, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[8] = Check(3, BOTTLE_C, bottleInfo, COMPUTER) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
		}

	}
	else
	{
		if (bottleInfo[BOTTLE_A] > 0)
		{
			if (isWin[0] = Check(1, BOTTLE_A, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[1] = Check(2, BOTTLE_A, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[2] = Check(3, BOTTLE_A, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}

		}
		if (bottleInfo[BOTTLE_B] > 0)
		{

			if (isWin[3] = Check(1, BOTTLE_B, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[4] = Check(2, BOTTLE_B, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[5] = Check(3, BOTTLE_B, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
		}
		if (bottleInfo[BOTTLE_C] > 0)
		{
			if (isWin[6] = Check(1, BOTTLE_C, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[7] = Check(2, BOTTLE_C, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
			if (isWin[8] = Check(3, BOTTLE_C, bottleInfo, ALICE) == 1)
			{
				g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = 1;
				return -1;
			}
		}
	}

	g_coinCase[bottleInfo[0]][bottleInfo[1]][bottleInfo[2]] = -1;
	return 1;

}
