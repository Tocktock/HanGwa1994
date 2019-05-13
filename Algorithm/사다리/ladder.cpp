
#include <iostream>
#include <fstream>
#include<string.h>
#include <algorithm>
using namespace std;

void Input();
void MainAlgoritm();
void Output();
ofstream fOut;
ifstream fIn;
struct LADDER
{
	int endLadder;
	int endHeight;
};

int g_ladderNum;
int g_inputNum;

LADDER g_ladderInfo[101][10001];
LADDER g_reverseInfo[101][10001];
int g_Sequence[101][20000];
int g_SeqCount[101] = { 0, };
int g_answer[101];
int main()
{
	fIn.open("ladder.inp");
	fOut.open("ladder.out");

	Input();
	MainAlgoritm();
	Output();


	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{
	int x1, x2, y1, y2;
	fIn >> g_ladderNum;

	memset(g_ladderInfo, -1, sizeof(g_ladderInfo));
	memset(g_reverseInfo, -1, sizeof(g_reverseInfo));
	memset(g_Sequence, -1, sizeof(g_Sequence));
	while (true)
	{ //x가 사다리번호 y가 높이
		fIn >> x1 >> y1 >> x2 >> y2;
		if (x1 == 0 && y1 == 0 && x2 == 0 && y2 == 0)
			break;
		g_ladderInfo[x1][y1].endLadder = x2;
		g_ladderInfo[x1][y1].endHeight = y2;

		g_reverseInfo[x2][y2].endLadder = x1;
		g_reverseInfo[x2][y2].endHeight = y1;

		g_Sequence[x1][g_SeqCount[x1]] = y1;
		g_SeqCount[x1]++;
		g_Sequence[x2][g_SeqCount[x2]] = y2;
		g_SeqCount[x2]++;
	}
}
void MainAlgoritm()
{
	for (int i = 0; i < g_ladderNum; i++)
	{
		sort(g_Sequence[i], g_Sequence[i] + g_SeqCount[i]);
	}
	LADDER start;
	for (int i = 0; i < g_ladderNum; i++)
	{
		start.endLadder = i;
		start.endHeight = g_Sequence[i][0];
		while (true)
		{
			if (g_ladderInfo[start.endLadder][start.endHeight].endHeight != -1)
			{
				start = g_ladderInfo[start.endLadder][start.endHeight];
			}
			else
			{
				start = g_reverseInfo[start.endLadder][start.endHeight];
			}
			if (start.endHeight == g_Sequence[start.endLadder][g_SeqCount[start.endLadder] - 1])
			{
				g_answer[i] = start.endLadder;
				break;
			}
			else
			{
				for (int i = 0; i < g_SeqCount[start.endLadder]; i++)
				{
					if (g_Sequence[start.endLadder][i] == start.endHeight)
					{
						start.endHeight = g_Sequence[start.endLadder][i + 1];
						break;
					}
				}
			}
		}
	}
}
void Output()
{
	for (int i = 0; i < g_ladderNum; i++)
	{
		fOut << g_answer[i] << " ";
	}
	fOut << endl;
}