
#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;

enum SECTION
{
	SECTION_A,
	SECTION_B,
	SECTION_C,
	SECTION_D,
	SECTION_E,
	SECTION_F,
	SECTION_G,
	SECTION_H,
	SECTION_I,
	SECTION_END
};

class POINT
{
public:
	int x[10];
	int y[10];
};

void Input();
void Output();
void MainAlgolritm();
void AddSection(int x, int y, int Section);
void Reset();
bool IsPossible(int x, int y, int value);
bool SectionCheck(int x, int y, int value, int section);
bool RowCheck(int x, int y, int value);
bool ColumnCheck(int x, int y, int value);
void BackTrack(int x, int y); //시작지점부터 시작

ifstream fIn;
ofstream fOut;
int g_boardInfo[10][10];
int g_sectionInfo[10][10];
POINT g_sameSection[10];
int g_sectionCount[10];
char g_section[10] = { 'a','b','c','d','e','f','g','h','i' };
int g_TestCase = 1;
bool stopFlag = false;

int main()
{
	fIn.open("sudoku.inp");
	fOut.open("sudoku.out");
	int casenum = 0;
	fIn >> casenum;

	for (int i = 0; i < casenum; i++)
	{
		Input();
		MainAlgolritm();
	}
	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{
	Reset();
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			fIn >> g_boardInfo[i][j];
	}

	char section;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fIn >> section;
			for (int k = 0; k < SECTION_END; k++)
			{
				if (section == g_section[k])
					AddSection(j, i, k);
			}
		}
	}
}

void Output()
{
	fOut << "Test Case No: " << g_TestCase << endl;
	g_TestCase++;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			fOut << g_boardInfo[i][j] << " ";
		}
		fOut << endl;
	}
	fOut << endl;
}

void MainAlgolritm()
{
	stopFlag = false;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			if (g_boardInfo[i][j] == 0)
			{
				BackTrack(j, i); //x , y 를 넣어야 함
				return;
			}
		}
	}

}
void BackTrack(int x, int y) //시작지점부터 시작
{
	bool promisingFlag = false;
	if (stopFlag == true)
		return;

	if (g_boardInfo[y][x] != 0) // 이미 숫자가 있다면.
	{
		if (y == 8 && x == 8)
		{
			Output();
			stopFlag = true;
			return;
		}
		if (x == 8) // 가로 마지막이면
			BackTrack(0, y + 1);
		else
			BackTrack(x + 1, y); // 아니면 가로 한칸식 가면서 검색.
		return;
	}


	for (int i = 1; i <= 9; i++)
	{
		g_boardInfo[y][x] = i;
		if (IsPossible(x, y, i))
		{
			if (y == 8 && x == 8)
			{
				Output();
				stopFlag = true;
				return;
			}
			if (x == 8) // 가로 마지막이면
				BackTrack(0, y + 1);
			else
				BackTrack(x + 1, y); // 아니면 가로 한칸식 가면서 검색.

		}
		g_boardInfo[y][x] = 0;
	}
}

bool IsPossible(int x, int y, int value)
{
	int section = g_sectionInfo[y][x];
	if (!SectionCheck(x, y, value, section))
		return false;
	if (!RowCheck(x, y, value))
		return false;
	if (!ColumnCheck(x, y, value))
		return false;
	return true;
}
bool SectionCheck(int x, int y, int value, int section)
{
	for (int i = 0; i < 9; i++)
	{
		if (g_sameSection[section].x[i] == x && g_sameSection[section].y[i] == y)
			continue;
		if (g_boardInfo[g_sameSection[section].y[i]][g_sameSection[section].x[i]] == value)
			return false;
	}
	return true;
}
bool RowCheck(int x, int y, int value)
{
	for (int i = 0; i < 9; i++)
	{
		if (i == y)
			continue;
		if (g_boardInfo[i][x] == value)
			return false;
	}
	return true;
}
bool ColumnCheck(int x, int y, int value)
{
	for (int i = 0; i < 9; i++)
	{
		if (i == x)
			continue;
		if (g_boardInfo[y][i] == value)
			return false;
	}
	return true;
}

void AddSection(int x, int y, int Section)
{
	g_sameSection[Section].x[g_sectionCount[Section]] = x;
	g_sameSection[Section].y[g_sectionCount[Section]] = y;
	g_sectionCount[Section]++;
	g_sectionInfo[y][x] = Section;
}
void Reset()
{
	memset(g_boardInfo, 0, sizeof(g_boardInfo));
	memset(g_sameSection, 0, sizeof(g_sameSection));
	memset(g_sectionCount, 0, sizeof(g_sectionCount));
	memset(g_sectionInfo, 0, sizeof(g_sectionInfo));
}
