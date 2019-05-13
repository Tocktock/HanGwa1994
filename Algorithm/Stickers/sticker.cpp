
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>

using namespace std;

void Input();
void MainAlgoritm();
void Output();

ofstream fOut;
ifstream fIn;

int g_inputNum;
int g_inputData[2][200000];
int g_dp[2][200000];
long long g_answer;

int main()
{
	fIn.open("sticker.inp");
	fOut.open("sticker.out");
	int caseNum = 0;

	fIn >> caseNum;
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
	fIn >> g_inputNum;
	memset(g_inputData, 0, sizeof(g_inputData));
	g_inputData[0][0] = 0;
	g_inputData[1][0] = 0;
	for (int i = 0; i < g_inputNum; i++)
	{
		fIn >> g_inputData[0][i + 1];
	}
	for (int i = 0; i < g_inputNum; i++)
	{
		fIn >> g_inputData[1][i + 1];
	}
}
void MainAlgoritm()
{
	int right = 0;
	long long answer1 = g_inputData[0][0];
	long long answer2 = g_inputData[1][0];
	g_dp[0][0] = 0;
	g_dp[1][0] = 0;
	g_dp[0][1] = g_inputData[0][1];
	g_dp[1][1] = g_inputData[1][1];

	for (int i = 2; i < g_inputNum + 1; i++)
	{
		int case1, case2;

		case1 = g_dp[1][i - 2];
		case2 = g_dp[1][i - 1];

		g_dp[0][i] = g_inputData[0][i] + max(case1, case2);//위에거 먼저

		case1 = g_dp[0][i - 2];
		case2 = g_dp[0][i - 1];

		g_dp[1][i] = g_inputData[1][i] + max(case1, case2);
	}

	g_answer = max(g_dp[0][g_inputNum], g_dp[1][g_inputNum]);

}
void Output()
{
	fOut << g_answer << endl;;
}