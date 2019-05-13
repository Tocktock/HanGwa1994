
#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;

void Input(int instanceNum);
void Output();
void MainAlgolritm();
bool Compare(int instanceNum);
void InverseRun(int instanceNum);
ifstream g_fIn;
ofstream g_fOut;

int g_testCase;
int g_origin[30000];
int g_compare[30000];
int g_inverseCompare[30000];
int g_answer[30000];
int main()
{
	g_fIn.open("polygon.inp");
	g_fOut.open("polygon.out");

	MainAlgolritm();
	Output();

	g_fIn.close();
	g_fOut.close();
	return 0;
}


void Input(int instanceNum)
{
	for (int i = 0; i < instanceNum; i++)
	{
		g_fIn >> g_origin[i];
	}

	for (int i = 0; i < instanceNum; i++)
	{
		g_fIn >> g_compare[i];
	}
}

void Output()
{
	for (int i = 0; i < g_testCase; i++)
	{
		g_fOut << g_answer[i] << endl;
	}
}

void MainAlgolritm()
{
	g_fIn >> g_testCase;
	int instanceNum = 0;
	for (int i = 0; i < g_testCase; i++)
	{
		g_fIn >> instanceNum;
		Input(instanceNum);
		InverseRun(instanceNum);
		if (Compare(instanceNum) == true)
		{
			g_answer[i] = 1;
		}
		else
			g_answer[i] = 0;
	}

}
bool Compare(int instanceNum)
{
	int compareStartPoint;
	int compareEndPoint;

	//방향이 같을 때 체크
	for (int i = 0; i < instanceNum; i++)
	{
		if (g_origin[0] == g_compare[i])
		{
			compareStartPoint = i + 1;
			compareEndPoint = i - 1;

			//origin은 오른쪽방향 compare은 오른쪽방향 검색. 같은 방향이니까
			for (int j = 1; j < instanceNum; j++)
			{
				if (compareStartPoint == instanceNum)
					compareStartPoint = 0;

				if (g_origin[j] != g_compare[compareStartPoint])
					break;
				if (compareStartPoint == compareEndPoint)
					return true;
				compareStartPoint++;
			}
		}
	}


	//방향이 다를 때 체크
	for (int i = 0; i < instanceNum; i++)
	{
		if (g_origin[0] == g_inverseCompare[i])
		{
			compareStartPoint = i - 1;
			compareEndPoint = i + 1;

			//origin은 오른쪽방향 compare은 오른쪽방향 검색. 같은 방향이니까
			for (int j = 1; j < instanceNum; j++)
			{
				if (compareStartPoint == -1)
					compareStartPoint = instanceNum - 1;

				if (g_origin[j] != g_inverseCompare[compareStartPoint])
					break;

				if (compareStartPoint == compareEndPoint)
					return true;

				compareStartPoint--;
			}
		}
	}

	return false;
}

void InverseRun(int instanceNum)
{

	memcpy(g_inverseCompare, g_compare, sizeof(int)*instanceNum);

	if (g_compare[0] * g_compare[instanceNum - 1] > 0)
	{
		g_inverseCompare[0] *= -1;
	}

	for (int i = 1; i < instanceNum; i++)
	{
		if (g_compare[i] * g_compare[i - 1] > 0)
		{
			g_inverseCompare[i] *= -1;
		}
	}
}
