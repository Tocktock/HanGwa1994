
#include<iostream>
#include<fstream>
#include<algorithm>
#include<string.h>
using namespace std;

ifstream fIn;
ofstream fOut;

void Input();
void Output();
void MainAlgolritm();
bool IsWin(int s, int k);
void Reset();

bool g_dp[1000001][10];

int g_p;
int g_k;
int g_s;

int main()
{
	fIn.open("coinmove.inp");
	fOut.open("coinmove.out");

	int caseNum;
	fIn >> caseNum;

	for (int i = 0; i < caseNum; i++)
	{
		Reset();
		Input();
		MainAlgolritm();
		Output();
	}

	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{
	fIn >> g_p;
	fIn >> g_k;
	fIn >> g_s;
}

void Output()
{
	for (int i = 1; i <= g_k; i++)
	{
		if (g_dp[g_s][i] == true)
		{
			fOut << g_s - i << endl;
			return;
		}
	}
	fOut << -1 << endl;
}

void MainAlgolritm()
{
	for (int i = 0; i <= g_s; i++)
	{
		for (int j = 1; j <= g_k; j++) // j가 칸
		{
			g_dp[i][j] = IsWin(i, j);
		}
	}
	int arr[10] = { 1,2,3,4,5,6,7,8,9,10 };
	int *a = lower_bound(arr, arr + 10, 5);

}
void Reset()
{
	for (int i = 0; i < g_s; i++)
	{
		for (int j = 0; j < g_k; j++)
			g_dp[i][j] = false;
	}
}
bool IsWin(int s, int k)
{
	//false가 진다. true가 이긴다
	if (s - k < 0)
		return false;
	if ((s - k) % g_p == 0 && s - k != 0)
		return false;

	for (int i = 1; i <= g_k; i++)
	{
		if (i == k)
			continue;

		if (g_dp[s - k][i] == true)
			return false;
	}

	return true;
}