
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>
#include <vector>
using namespace std;

enum DIRECTION
{
	TOP,
	LEFT,
	RIGHT,
	BOTTOM,
	DIRECTION_END
};

class TankInfo
{
public:
	int x;
	int y;
	int m_hole[4];
	int m_height;
	TankInfo()
	{
		m_hole[0] = -1;
		m_hole[1] = -1;
		m_hole[2] = -1;
		m_hole[3] = -1;
	}
};

void Input();
void MainAlgoritm();
void Output();
void SetHeight(int x, int y, int holeHeight);
void PushInstance(int x, int y);
bool CheckWall(int x, int y, int holeHeight, int Direction);
ofstream fOut;
ifstream fIn;

int g_caseNum;
int g_row;
int g_column;
int g_height;

vector<TankInfo> g_vecTank;
TankInfo g_tankInfo[1024][1024];
bool	 g_isCheck[1024][1024];

int main()
{
	fIn.open("watertank.inp");
	fOut.open("watertank.out");
	fIn >> g_caseNum;

	for (int i = 0; i < g_caseNum; i++)
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
	fIn >> g_row;
	fIn >> g_column;
	fIn >> g_height;

	for (int i = 0; i < g_column; i++)
	{
		fIn >> g_tankInfo[0][i].m_hole[TOP];
	}

	for (int i = 0; i < g_row; i++)
	{
		for (int j = 0; j < g_column; j++)
		{

			fIn >> g_tankInfo[i][j].m_hole[BOTTOM];

			g_tankInfo[i][j].x = j;
			g_tankInfo[i][j].y = i;
			g_tankInfo[i][j].m_height = g_height;

			if (i > 0)
				g_tankInfo[i][j].m_hole[TOP] = g_tankInfo[i - 1][j].m_hole[BOTTOM];
		}
	}

	for (int i = 0; i < g_row; i++)
	{
		fIn >> g_tankInfo[i][0].m_hole[LEFT];

		for (int j = 0; j < g_column; j++)
		{
			fIn >> g_tankInfo[i][j].m_hole[RIGHT];

			if (j > 0)
				g_tankInfo[i][j].m_hole[LEFT] = g_tankInfo[i][j - 1].m_hole[RIGHT];
		}
	}

}

void MainAlgoritm()
{
	int holeHeight = 0;
	int x;
	int y;
	int count = 0;
	bool isFirst = true;
	int erasePoint = 0;
	int erasePoint2 = 0;

	for (int i = 0; i < g_column; i++)
	{
		if (g_tankInfo[0][i].m_hole[TOP] == -1)
			continue;

		holeHeight = g_tankInfo[0][i].m_hole[TOP];
		if (g_tankInfo[0][i].m_height <= holeHeight)
			continue;
		isFirst = true;
		memset(g_isCheck, false, sizeof(g_isCheck));
		g_isCheck[0][i] = true;
		g_tankInfo[0][i].m_height = holeHeight;
		g_vecTank.push_back(g_tankInfo[0][i]);

		while (!g_vecTank.empty())
		{
			count = 0;
			x = g_vecTank.back().x;
			y = g_vecTank.back().y;
			holeHeight = g_vecTank.back().m_height;

			erasePoint = g_vecTank.size() - 1;

			if (CheckWall(x, y, holeHeight, TOP) == false && isFirst == false)
				count++;
			if (CheckWall(x, y, holeHeight, LEFT) == false)
				count++;
			if (CheckWall(x, y, holeHeight, RIGHT) == false)
				count++;
			if (CheckWall(x, y, holeHeight, BOTTOM) == false)
				count++;

			g_vecTank.erase(g_vecTank.begin() + erasePoint);



			isFirst = false;

		}
	}

	for (int i = 0; i < g_row; i++)
	{
		if (g_tankInfo[i][0].m_hole[LEFT] == -1)
			continue;

		holeHeight = g_tankInfo[i][0].m_hole[LEFT];
		if (g_tankInfo[i][0].m_height <= holeHeight)
			continue;
		isFirst = true;
		memset(g_isCheck, false, sizeof(g_isCheck));
		g_isCheck[i][0] = true;
		g_tankInfo[i][0].m_height = holeHeight;
		g_vecTank.push_back(g_tankInfo[i][0]);


		while (!g_vecTank.empty())
		{
			count = 0;
			x = g_vecTank.back().x;
			y = g_vecTank.back().y;
			holeHeight = g_vecTank.back().m_height;

			erasePoint = g_vecTank.size() - 1;

			if (CheckWall(x, y, holeHeight, TOP) == false)
				count++;
			if (CheckWall(x, y, holeHeight, LEFT) == false && isFirst == false)
				count++;
			if (CheckWall(x, y, holeHeight, RIGHT) == false)
				count++;
			if (CheckWall(x, y, holeHeight, BOTTOM) == false)
				count++;

			g_vecTank.erase(g_vecTank.begin() + erasePoint);

			isFirst = false;

		}
	}

	for (int i = 0; i < g_row; i++)
	{
		if (g_tankInfo[i][g_column - 1].m_hole[RIGHT] == -1)
			continue;

		holeHeight = g_tankInfo[i][g_column - 1].m_hole[RIGHT];
		if (g_tankInfo[i][g_column - 1].m_height <= holeHeight)
			continue;
		isFirst = true;

		memset(g_isCheck, false, sizeof(g_isCheck));
		g_isCheck[i][g_column - 1] = true;
		g_tankInfo[i][g_column - 1].m_height = holeHeight;
		g_vecTank.push_back(g_tankInfo[i][g_column - 1]);

		while (!g_vecTank.empty())
		{
			count = 0;
			x = g_vecTank.back().x;
			y = g_vecTank.back().y;
			holeHeight = g_vecTank.back().m_height;

			erasePoint = g_vecTank.size() - 1;

			if (CheckWall(x, y, holeHeight, TOP) == false)
				count++;
			if (CheckWall(x, y, holeHeight, LEFT) == false)
				count++;
			if (CheckWall(x, y, holeHeight, RIGHT) == false && isFirst == false)
				count++;
			if (CheckWall(x, y, holeHeight, BOTTOM) == false)
				count++;

			g_vecTank.erase(g_vecTank.begin() + erasePoint);

			isFirst = false;

		}
	}

	for (int i = 0; i < g_column; i++)
	{
		if (g_tankInfo[g_row - 1][i].m_hole[BOTTOM] == -1)
			continue;

		holeHeight = g_tankInfo[g_row - 1][i].m_hole[BOTTOM];
		if (g_tankInfo[g_row - 1][i].m_height <= holeHeight)
			continue;
		isFirst = true;

		memset(g_isCheck, false, sizeof(g_isCheck));
		g_isCheck[g_row - 1][i] = true;
		g_tankInfo[g_row - 1][i].m_height = holeHeight;
		g_vecTank.push_back(g_tankInfo[g_row - 1][i]);

		while (!g_vecTank.empty())
		{
			count = 0;
			x = g_vecTank.back().x;
			y = g_vecTank.back().y;
			holeHeight = g_vecTank.back().m_height;

			erasePoint = g_vecTank.size() - 1;


			if (CheckWall(x, y, holeHeight, TOP) == false)
				count++;
			if (CheckWall(x, y, holeHeight, LEFT) == false)
				count++;
			if (CheckWall(x, y, holeHeight, RIGHT) == false)
				count++;
			if (CheckWall(x, y, holeHeight, BOTTOM) == false && isFirst == false)
				count++;

			g_vecTank.erase(g_vecTank.begin() + erasePoint);



			isFirst = false;

		}
	}
}

void Output()
{
	long long answer = 0;
	for (int i = 0; i < g_row; i++)
	{
		for (int j = 0; j < g_column; j++)
		{
			answer += g_tankInfo[i][j].m_height;
		}
	}
	fOut << answer << endl;
}
void SetHeight(int x, int y, int holeHeight)
{
	if (g_tankInfo[y][x].m_height > holeHeight)
		g_tankInfo[y][x].m_height = holeHeight;
	g_isCheck[y][x] = true;
}
void PushInstance(int x, int y)
{
	g_vecTank.push_back(g_tankInfo[y][x]);
	g_isCheck[y][x] = true;
}
bool CheckWall(int x, int y, int holeHeight, int Direction)
{
	int originX = x;
	int originY = y;
	if (g_tankInfo[y][x].m_hole[Direction] == -1) // 벽이 없다
		return false;
	switch (Direction)
	{
	case TOP:
		y = y - 1;
		break;
	case LEFT:
		x = x - 1;
		break;
	case RIGHT:
		x = x + 1;
		break;
	case BOTTOM:
		y = y + 1;
		break;
	}

	if (x < 0 || x >= g_column || y < 0 || y >= g_row) // 초과범위
		return false;
	if (g_isCheck[y][x] == true && g_tankInfo[y][x].m_height <= holeHeight) // 이번 루틴에 이미 검사했음.
		return false;

	if (g_tankInfo[originY][originX].m_hole[Direction] >= holeHeight)
	{
		SetHeight(x, y, g_tankInfo[originY][originX].m_hole[Direction]);
		PushInstance(x, y);
	}

	else if (g_tankInfo[originY][originX].m_hole[Direction] < holeHeight)
	{
		SetHeight(x, y, holeHeight);
		PushInstance(x, y);
	}

	return true;

}