
#include<iostream>
#include<fstream>
using namespace std;

ifstream g_fIn;
ofstream g_fOut;

int g_mapInfo[20][20]; //세로,가로 꼭지점 정보.
int g_inputData[1000][3]; //N,M,o위치
int g_caseNum;
int g_answer[1000];
int g_answerCount = 0;
int g_oLocation[2]; //n,m위치 정보.

void Input();
void Output();
void MainAlgolritm();
void Reset(int n, int m);
void FindLocation(int n, int m, int location);
void FindMap(int n, int m);


int main()
{
	g_fIn.open("path.inp");
	g_fOut.open("path.out");
	Input();
	MainAlgolritm();
	Output();
	g_fIn.close();
	g_fOut.close();
	return 0;
}

void Input()
{
	g_fIn >> g_caseNum;

	for (int i = 0; i < g_caseNum; i++)
	{
		g_fIn >> g_inputData[i][0];
		g_fIn >> g_inputData[i][1];
		g_fIn >> g_inputData[i][2];
	}
}

void Output()
{
	for (int i = 0; i < g_caseNum; i++)
		g_fOut << g_answer[i] << endl;

}

void MainAlgolritm()
{
	for (int i = 0; i < g_caseNum; i++)
	{
		FindLocation(g_inputData[i][0], g_inputData[i][1], g_inputData[i][2]);
		Reset(g_inputData[i][0], g_inputData[i][1]);
		FindMap(g_inputData[i][0], g_inputData[i][1]);
	}

}

void Reset(int n, int m) //n이 행, m이 열
{
	for (int i = 0; i < n; i++)
	{
		g_mapInfo[i][0] = 1;
	}

	for (int i = 0; i < m; i++)
	{
		g_mapInfo[0][i] = 1;
	}

	for (int i = 1; i < n; i++)
	{
		for (int j = 1; j < m; j++)
		{
			g_mapInfo[n][m] = 0;
		}
	}

	for (int i = g_oLocation[0]; i < n; i++)
	{
		g_mapInfo[i][g_oLocation[1]] = 1;
	}

	for (int i = g_oLocation[1]; i < m; i++)
	{
		g_mapInfo[g_oLocation[0]][i] = 1;
	}
}

void FindLocation(int n, int m, int location)
{
	if (location == 0)
	{
		g_oLocation[0] = n - 1; //n
		g_oLocation[1] = m - 1; //m
		return;
	}
	int temp = --location;//0부터 시작으로 맞추기 위해서

	g_oLocation[0] = temp / m; //n
	g_oLocation[1] = temp % m; //m
}

void FindMap(int n, int m)
{

	int beforeLoc = 1;
	int afterLoc = 1;
	for (int i = 1; i <= g_oLocation[0]; i++)
	{
		for (int j = 1; j <= g_oLocation[1]; j++)
		{
			g_mapInfo[i][j] = g_mapInfo[i - 1][j] + g_mapInfo[i][j - 1];
		}

		if (i == g_oLocation[0])
		{
			beforeLoc = g_mapInfo[g_oLocation[0]][g_oLocation[1]];
		}
	}
	g_mapInfo[g_oLocation[0]][g_oLocation[1]] = 1;


	for (int i = g_oLocation[0] + 1; i < n; i++)
	{
		for (int j = g_oLocation[1] + 1; j < m; j++)
		{
			g_mapInfo[i][j] = g_mapInfo[i - 1][j] + g_mapInfo[i][j - 1];
		}
		if (i == n - 1)
		{
			afterLoc = g_mapInfo[n - 1][m - 1];
		}
	}
	g_answer[g_answerCount] = afterLoc * beforeLoc;
	g_answerCount++;
}
