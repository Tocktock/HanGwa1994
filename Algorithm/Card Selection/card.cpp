
#include <iostream>
#include <fstream>
#include <string.h>
#include <algorithm>

using namespace std;

void Input();
void MainAlgoritm();
void Output();
int GetMax(int arr[], int length);
ofstream fOut;
ifstream fIn;

int g_inputNum;
int g_card[1024];
int g_answer;
int g_answerArr[1024][3];

int Check(int right, int jumpRange);

int main()
{
	fIn.open("card.inp");
	fOut.open("card.out");
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
	for (int i = 0; i < g_inputNum; i++)
	{
		fIn >> g_card[i];
	}
}
void MainAlgoritm()
{
	for (int i = 0; i < 1024; i++)
	{
		for (int j = 0; j < 3; j++)
			g_answerArr[i][j] = -100000000;
	}
	g_answer = Check(g_inputNum - 1, 3);
}
void Output()
{
	fOut << g_answer << endl;;
}

int Check(int right, int jumpRange)
{
	int answer;
	int jump[3];
	for (int i = 0; i < 3; i++)
	{
		jump[i] = -100000000;
	}

	if (right < 0)
		return 0;
	if (g_answerArr[right][jumpRange - 1] != -100000000)
		return g_answerArr[right][jumpRange - 1];

	if (right >= 0 && right <= 2)
		return g_card[right];

	if (right > 1 && jumpRange > 1)
		jump[0] = g_card[right] + Check(right - 2, 1);

	if (right > 2)
		jump[1] = g_card[right] + Check(right - 3, 2);

	if (right > 3)
		jump[2] = g_card[right] + Check(right - 4, 3);

	answer = GetMax(jump, 3);
	g_answerArr[right][jumpRange - 1] = answer;
	return answer;
}

int GetMax(int arr[], int length)
{
	int max = -100000000;
	for (int i = 0; i < length; i++)
	{
		if (arr[i] > max)
			max = arr[i];
	}
	return max;
}