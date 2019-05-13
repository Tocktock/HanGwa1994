
#include<iostream>
#include<fstream>
using namespace std;

struct FRACTION
{
	long long top;
	long long bottom;
};

ifstream fIn;
ofstream fOut;

FRACTION g_input;
FRACTION g_center;
FRACTION g_left;
FRACTION g_right;
char g_answer[100000];
long long g_answerCount;
bool Input();
void Output();
void MainAlgolritm();
bool Compare(FRACTION center, FRACTION input);
bool Match(FRACTION center, FRACTION input);
int main()
{
	fIn.open("fraction.inp");
	fOut.open("fraction.out");
	while (true)
	{
		g_answerCount = 0;

		g_right.top = 1;
		g_right.bottom = 0;
		g_left.top = 0;
		g_left.bottom = 1;
		g_center.top = 1;
		g_center.bottom = 1;

		if (!Input())
			break;
		MainAlgolritm();
		Output();
	}

	fIn.close();
	fOut.close();
	return 0;
}

bool Input()
{
	fIn >> g_input.top;
	fIn >> g_input.bottom;
	if (g_input.top == 1 && g_input.bottom == 1)
		return false;
	return true;
}

void Output()
{
	for (long long i = 0; i < g_answerCount; i++)
		fOut << g_answer[i];
	fOut << endl;
}

void MainAlgolritm()
{
	FRACTION temp;
	g_answerCount = 0;
	while (true)
	{
		if (Match(g_center, g_input) == true)
			break;

		if (Compare(g_center, g_input) == true) //인풋값이 더 크다. 센터가 오른쪽으로.
		{
			temp = g_center;
			g_center.top = temp.top + g_right.top;
			g_center.bottom = temp.bottom + g_right.bottom;
			g_left = temp;
			g_answer[g_answerCount] = 'R';
			g_answerCount++;
		}
		else // 인풋값이 작다. 센터가 왼쪽으로
		{
			temp = g_center;
			g_center.top = temp.top + g_left.top;
			g_center.bottom = temp.bottom + g_left.bottom;
			g_right = temp;
			g_answer[g_answerCount] = 'L';
			g_answerCount++;
		}

	}

}

bool Compare(FRACTION center, FRACTION input)
{
	input.top = input.top * center.bottom;
	center.top = center.top * input.bottom;

	if (center.top < input.top)
		return true;

	return false;
}
bool Match(FRACTION center, FRACTION input)
{
	if (center.top == input.top && center.bottom == input.bottom)
		return true;

	return false;
}