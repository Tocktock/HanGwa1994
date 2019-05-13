
#include <iostream>
#include <fstream>
#include<string.h>

using namespace std;

void Input();
void MainAlgoritm();
void Output();
void MergeSort();
ofstream fOut;
ifstream fIn;
int g_inputData[150000];
int g_sortTemp[150000];
long long int g_answer;
int g_inputNum;



int main()
{
	int caseNum;
	fIn.open("inv.inp");
	fOut.open("inv.out");

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
		fIn >> g_inputData[i];
	}
}
void MainAlgoritm()
{
	MergeSort();

}
void Output()
{
	fOut << g_answer << endl;
}
void MergeSort()
{
	int left = 0;
	int size = 1;
	int right = 0;
	int leftCount = 0;
	int rightCount = 0;
	int sortCount = 0;
	int evenCheck = g_inputNum % 2;
	g_answer = 0;
	while (size < g_inputNum)
	{
		sortCount = 0;

		for (int i = 0; i < g_inputNum; i += size * 2)
		{
			left = i;
			right = left + size;
			leftCount = 0;
			rightCount = 0;
			for (int j = 0; j < size * 2; j++)
			{
				if (sortCount == g_inputNum)
					break;

				if (rightCount == size || g_inputData[right + rightCount] == 0)
				{

					g_sortTemp[sortCount] = g_inputData[left + leftCount];
					sortCount++;
					leftCount++;
					g_answer += rightCount;
					continue;
				}
				else if (leftCount == size)
				{
					g_sortTemp[sortCount] = g_inputData[right + rightCount];
					sortCount++;
					rightCount++;
					continue;
				}

				if (g_inputData[right + rightCount] < g_inputData[left + leftCount])
				{
					g_sortTemp[sortCount] = g_inputData[right + rightCount];
					sortCount++;
					rightCount++;
				}
				else if (g_inputData[left + leftCount] < g_inputData[right + rightCount])
				{
					g_sortTemp[sortCount] = g_inputData[left + leftCount];
					sortCount++;
					leftCount++;
					g_answer += rightCount;
				}
			}
		}
		size *= 2;
		memcpy(g_inputData, g_sortTemp, sizeof(int) * sortCount);
	}
}