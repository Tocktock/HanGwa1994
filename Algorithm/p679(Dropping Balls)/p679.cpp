#include <fstream>
#include<string.h>
#include <stdlib.h>

using namespace std;

ifstream fIn;
ofstream fOut;

void MainAlgorithm();
void Input();
void Output();

bool g_nodeFlag[2560000];
int g_inputFirst;
int g_inputSecond;
int main()
{
	fIn.open("p679.inp");
	fOut.open("p679.out");
	int testcase;
	fIn >> testcase;
	for (int i = 0; i < testcase; i++)
	{
		Input();
		MainAlgorithm();
	}

	fIn.close();
	fOut.close();
	return 0;
}


void MainAlgorithm()
{
	int depthCount = 0;
	int index = 1;

	for (int i = 0; i < g_inputSecond; i++) // 볼카운트
	{
		index = 1;
		depthCount = 1;
		while (depthCount < g_inputFirst)
		{
			if (g_nodeFlag[index] == false)
			{
				g_nodeFlag[index] = true;
				index = index * 2;
			}
			else
			{
				g_nodeFlag[index] = false;
				index = index * 2 + 1;
			}			
			depthCount++;
		}
	}
	fOut << index << endl;
	int leafCount = 1;
	for (int i = 0; i < g_inputFirst; i++)
		leafCount *= 2;
	for (int i = 1; i < leafCount; i++)
		g_nodeFlag[i] = false;
}
void Input()
{
	fIn >> g_inputFirst;
	fIn >> g_inputSecond;
}
void Output()
{

}