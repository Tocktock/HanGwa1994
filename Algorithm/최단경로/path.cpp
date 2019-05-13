#include<iostream>
#include<fstream>
#include<string.h>
#include<algorithm>
#include<vector>

#define UPSIDE true
#define LEFTSIDE false

using namespace std;
//경로가 몇개인지, 최소 비용의 경로의 비용이 얼마인지, 경로의 정보.

class XINFO
{
public:
	int beforeX;
	int beforeY;
	int minCost;
	unsigned long long pathNum;
	XINFO()
	{
		beforeX = 0;
		beforeY = 0;
		minCost = 0;
		pathNum = 0;
	}
};

class PATH_INFO
{
public:
	int rightCost;
	int bottomCost;
	XINFO xInfo[256]; // x를 몇번 거치고 오는지 그리고 그 때 경로는 몇개인지와, 그 경우 코스트는 최소를 취함.
	bool isX;
	int xCount; //x 몇개까지 유효한지.

	PATH_INFO()
	{
		xCount = 0;
		rightCost = 0;
		bottomCost = 0;
		isX = false;
	}
	int GetToRightCost(int k)
	{
		if (xInfo[k].pathNum == 0)
			return 40000000; //경로 최대의 수
		return xInfo[k].minCost + rightCost;
	}
	int GetToDownCost(int k)
	{
		if (xInfo[k].pathNum == 0)
			return 40000000; //경로 최대의 수
		return xInfo[k].minCost + bottomCost;
	}
	void Reset()
	{
		xCount = 0;
		rightCost = 0;
		bottomCost = 0;
		isX = false;

		for (int i = 0; i <= 256; i++)
		{
			xInfo[i].beforeX = 0;
			xInfo[i].beforeY = 0;
			xInfo[i].minCost = 0;
			xInfo[i].pathNum = 0;
		}
	}
};

using namespace std;
void Input();
void Output();
void MainAlgolrithm();

bool ComparePath(int y, int x, int k);
void Setting(int y, int x, int k, bool side);
void XCheck(int y, int x, bool side);
void Init();
int GetMaxK(int y, int x);
unsigned long long GetPathNum(int y, int x, int k);
int g_outputCaseNo = 0;

ifstream fIn;
ofstream fOut;
int g_answer;
int g_row;
int g_column;
int g_kNum;
PATH_INFO g_pathInfo[256][256];
vector<int> g_vecAnswer[2];

int main()
{
	fIn.open("path.inp");
	fOut.open("path.out");

	int caseNum;
	fIn >> caseNum;

	for (int i = 0; i < caseNum; i++)
	{
		Input();
		MainAlgolrithm();
		Output();
	}

	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{
	fIn >> g_row;
	fIn >> g_column;
	fIn >> g_kNum;

	int x;
	int y;

	for (int i = 0; i < g_kNum; i++)
	{
		fIn >> y;
		fIn >> x;
		g_pathInfo[y][x].isX = true;

	}

	int cost;
	for (int i = 0; i < g_row; i++)
	{
		for (int j = 0; j < g_column - 1; j++)
		{
			fIn >> cost;
			g_pathInfo[i][j].rightCost = cost;
		}
	}

	for (int i = 0; i < g_row - 1; i++)
	{
		for (int j = 0; j < g_column; j++)
		{
			fIn >> cost;
			g_pathInfo[i][j].bottomCost = cost;
		}
	}

}

void MainAlgolrithm()
{

	g_pathInfo[0][0].xInfo[0].pathNum = 1;

	Init();

	bool side;
	for (int i = 1; i < g_row; i++)
	{
		for (int j = 1; j < g_column; j++)
		{
			int maxK = GetMaxK(i, j);
			for (int k = 0; k <= maxK; k++)
			{
				side = ComparePath(i, j, k);
				Setting(i, j, k, side);
			}
			XCheck(i, j, side);
		}
	}

}

void Output()
{
	g_outputCaseNo++;
	fOut << "Test Case No:" << g_outputCaseNo << endl;

	for (int i = 0; i <= g_kNum; i++)
	{
		if (g_pathInfo[g_row - 1][g_column - 1].xInfo[i].pathNum % 100000 == 0)
			continue;
		fOut << "k:" << i << " count:" << g_pathInfo[g_row - 1][g_column - 1].xInfo[i].pathNum % 100000 << " cost:" << g_pathInfo[g_row - 1][g_column - 1].xInfo[i].minCost << endl;


		int y = g_row - 1;
		int x = g_column - 1;
		int k = i;
		int tempy = y;
		int tempx = x;
		while (true)
		{
			g_vecAnswer[0].push_back(y);
			g_vecAnswer[1].push_back(x);
			if (y == 0 && x == 0)
				break;
			tempy = y;
			tempx = x;
			y = g_pathInfo[y][x].xInfo[k].beforeY;
			x = g_pathInfo[tempy][x].xInfo[k].beforeX;

			if (g_pathInfo[tempy][tempx].isX == true)
				k--;

		}
		while (g_vecAnswer[0].size())
		{
			fOut << "(" << g_vecAnswer[0].back() << "," << g_vecAnswer[1].back() << ")";
			if (g_vecAnswer[0].size() != 1)
				fOut << "->";

			g_vecAnswer[0].pop_back();
			g_vecAnswer[1].pop_back();
		}
		fOut << endl;
		g_vecAnswer[0].clear();
		g_vecAnswer[1].clear();
	}
	for (int i = 0; i < g_row; i++)
	{
		for (int j = 0; j < g_column; j++)
		{
			g_pathInfo[i][j].Reset();
		}
	}
	fOut << endl;
}

bool ComparePath(int y, int x, int k) //더 작은값 리턴.
{
	if (y - 1 < 0)
		return LEFTSIDE;
	else if (x - 1 < 0)
		return UPSIDE;

	int left = g_pathInfo[y][x - 1].GetToRightCost(k);
	int up = g_pathInfo[y - 1][x].GetToDownCost(k);

	if (left > up)
	{
		return UPSIDE;
	}

	return LEFTSIDE; //같을 때는 왼쪽거를 넣어서 우선순위를 정해줌.
}
void Setting(int y, int x, int k, bool side)
{
	if (side == UPSIDE)
	{
		if (g_pathInfo[y - 1][x].xInfo[k].pathNum == 0)
			return;
		g_pathInfo[y][x].xInfo[k].minCost = g_pathInfo[y - 1][x].GetToDownCost(k);
		g_pathInfo[y][x].xInfo[k].beforeX = x;
		g_pathInfo[y][x].xInfo[k].beforeY = y - 1;
	}
	else
	{
		if (g_pathInfo[y][x - 1].xInfo[k].pathNum == 0)
			return;
		g_pathInfo[y][x].xInfo[k].minCost = g_pathInfo[y][x - 1].GetToRightCost(k);
		g_pathInfo[y][x].xInfo[k].beforeX = x - 1;
		g_pathInfo[y][x].xInfo[k].beforeY = y;
	}
	g_pathInfo[y][x].xInfo[k].pathNum = GetPathNum(y - 1, x, k) + GetPathNum(y, x - 1, k);
	if (g_pathInfo[y][x].xInfo[k].pathNum > 1000000)
		g_pathInfo[y][x].xInfo[k].pathNum = g_pathInfo[y][x].xInfo[k].pathNum % 1000000;
}

void XCheck(int y, int x, bool side)
{
	int xCount;
	PATH_INFO temp;
	if (side == UPSIDE)
	{
		xCount = g_pathInfo[y - 1][x].xCount;
		temp = g_pathInfo[y - 1][x];
	}
	else
	{
		xCount = g_pathInfo[y][x - 1].xCount;
		temp = g_pathInfo[y][x - 1];
	}

	if (g_pathInfo[y][x].isX == true) //
	{

		g_pathInfo[y][x].xCount = xCount + 1;
		memcpy(&g_pathInfo[y][x].xInfo[1], &g_pathInfo[y][x].xInfo[0], sizeof(XINFO) * g_pathInfo[y][x].xCount);
		// 밀어내고. 그 전 꺼 0만들기.
		g_pathInfo[y][x].xInfo[0].pathNum = 0;
		g_pathInfo[y][x].xInfo[0].minCost = 0;

	}
	else
	{
		g_pathInfo[y][x].xCount = xCount;
	}
}

void Init() // 젤 상단 젤 좌측단 부터 해줌.
{
	for (int i = 1; i < g_row; i++)
	{
		int maxK = g_pathInfo[i - 1][0].xCount;
		for (int k = 0; k <= maxK; k++)
		{
			Setting(i, 0, k, UPSIDE);
		}
		XCheck(i, 0, UPSIDE);
	}

	for (int i = 1; i < g_column; i++)
	{
		int maxK = g_pathInfo[0][i - 1].xCount;
		for (int k = 0; k <= maxK; k++)
		{
			Setting(0, i, k, LEFTSIDE);
		}
		XCheck(0, i, LEFTSIDE);
	}
}

int GetMaxK(int y, int x)
{
	if (g_pathInfo[y - 1][x].xCount < g_pathInfo[y][x - 1].xCount)
		return g_pathInfo[y][x - 1].xCount;
	else
		return g_pathInfo[y - 1][x].xCount;
}
unsigned long long  GetPathNum(int y, int x, int k)
{
	if (y < 0 || x < 0)
		return 0;

	return g_pathInfo[y][x].xInfo[k].pathNum;
}