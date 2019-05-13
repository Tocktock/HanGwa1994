
#include <iostream>
#include <fstream>
#include<string.h>

using namespace std;

bool Input();
void MainAlgoritm();
void Output();
void MakeB(int left, int right, int top, int bottom); //나누고
void MakeD(int left, int right, int top, int bottom);
char CheckTypeB(int left, int right, int top, int bottom); //검사(재귀)
char CheckTypeD(int left, int right, int top, int bottom, char bit); //검사(재귀)

ofstream fOut;
ifstream fIn;

char g_type;
int g_row;
int g_column;
char g_inputData[50000];
char g_DInfo[50000];
char g_matrix[512][512];
int g_answerCount = 0;
int g_DCount = 0;
int g_DinputCount = 0;

int main()
{
	char endCheck;

	fIn.open("bitmap.inp");
	fOut.open("bitmap.out");

	while (true)
	{
		g_DCount = 0;
		g_DinputCount = 0;
		g_answerCount = 0;
		if (!Input())
			break;
		MainAlgoritm();
		Output();
	}

	fIn.close();
	fOut.close();
	return 0;
}

bool Input()
{
	int inputCount = 0;
	fIn >> g_type;
	if (g_type == '#')
		return false;
	fIn >> g_row;
	fIn >> g_column;

	if (g_type == 'D')
	{
		MakeD(1, g_column, 1, g_row);
		return true;
	}
	while (g_row * g_column > inputCount)
	{
		fIn >> &g_inputData[inputCount]; // 50개씩 들어오는거 생각해야함
		inputCount += 50;
	}

	if (g_type == 'B')
	{
		int count = 0;
		for (int i = 1; i <= g_row; i++)
		{
			for (int j = 1; j <= g_column; j++)
			{
				g_matrix[i][j] = g_inputData[count];
				count++;
			}
		}
	}
	return true;
}
void MainAlgoritm()
{
	if (g_type == 'B')
		MakeB(1, g_column, 1, g_row);

}

void Output()
{
	int count = 0;
	if (g_type == 'B')
	{
		fOut << "D" << " " << g_row << " " << g_column << endl;
		for (int i = 0; i < g_row; i++)
		{
			for (int j = 0; j < g_column; j++)
			{
				fOut << g_DInfo[count];
				count++;
				if (g_answerCount == count)
				{
					fOut << endl;
					return;
				}
				if (count % 50 == 0)
					fOut << endl;

			}
		}
	}
	else
	{
		fOut << "B" << " " << g_row << " " << g_column << endl;
		for (int i = 1; i <= g_row; i++)
		{
			for (int j = 1; j <= g_column; j++)
			{
				fOut << g_matrix[i][j];
				count++;
				if (count % 50 == 0 && i * j < g_row * g_column)
					fOut << endl;
			}
		}
		fOut << endl;
	}
}
void MakeB(int left, int right, int top, int bottom)
{
	char firstchar = g_matrix[top][left];
	int rowTemp = 0; //보정숫자
	int columnTemp = 0;
	if (right % 2 == 1)
		columnTemp = 1;
	if (bottom % 2 == 1)
		rowTemp = 1;

	if (CheckTypeB(left, right, top, bottom) == 'D') //전체검사 먼저.
	{
		g_DInfo[g_answerCount] = 'D';
		g_answerCount++;

		int LT_left = left;
		int LT_right = (right - left) / 2 + left;
		int LT_top = top;
		int LT_bottom = (bottom - top) / 2 + top;

		MakeB(LT_left, LT_right, LT_top, LT_bottom); // LT
		if (LT_right + 1 <= right)
			MakeB(LT_right + 1, right, LT_top, LT_bottom);//RT
		if (LT_bottom + 1 <= bottom)
		{
			MakeB(LT_left, LT_right, LT_bottom + 1, bottom);
			if (LT_right + 1 <= right)
				MakeB(LT_right + 1, right, LT_bottom + 1, bottom);
		}
	}
	else
	{
		g_DInfo[g_answerCount] = firstchar;
		g_answerCount++;
	}


}
char CheckTypeB(int left, int right, int top, int bottom)
{
	char binaryNum = g_matrix[top][left];

	for (int i = top; i <= bottom; i++)
	{
		for (int j = left; j <= right; j++)
		{
			if (binaryNum != g_matrix[i][j])
				return 'D';
		}
	}
	return binaryNum;
}
void MakeD(int left, int right, int top, int bottom)
{
	int rowTemp = 0; //보정숫자
	int columnTemp = 0;
	if (right % 2 == 1)
		columnTemp = 1;
	if (bottom % 2 == 1)
		rowTemp = 1;
	if (g_DinputCount >= g_row * g_column)
		return;
	fIn >> g_inputData[g_DCount];

	if (g_inputData[g_DCount] == 'D') //전체검사 먼저.
	{
		g_DCount++;
		int LT_left = left;
		int LT_right = (right - left) / 2 + left;
		int LT_top = top;
		int LT_bottom = (bottom - top) / 2 + top;

		MakeD(LT_left, LT_right, LT_top, LT_bottom); // LT
		if (LT_right + 1 <= right)
			MakeD(LT_right + 1, right, LT_top, LT_bottom);//RT
		if (LT_bottom + 1 <= bottom)
		{
			MakeD(LT_left, LT_right, LT_bottom + 1, bottom);
			if (LT_right + 1 <= right)
				MakeD(LT_right + 1, right, LT_bottom + 1, bottom);
		}
	}
	else
	{

		for (int i = top; i <= bottom; i++)
		{
			for (int j = left; j <= right; j++)
				g_matrix[i][j] = g_inputData[g_DCount];
		}
		g_DCount++;
		g_DinputCount += (right - left + 1) * (bottom - top + 1);

	}
}
char CheckTypeD(int left, int right, int top, int bottom, char bit)//검사(재귀)
{
	return 'D';
}