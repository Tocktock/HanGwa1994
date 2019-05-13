#include <fstream>
#include<string.h>
#include <stdlib.h>

#define NOTMATCH -1
using namespace std;

ifstream fIn;
ofstream fOut;

char g_input[256];

void MainAlgorithm();
void Input();
void Output();
int IsSlump(char input[]);
int IsSlimp(char input[]);

bool g_answer;
int main()
{
	fIn.open("p384.inp");
	fOut.open("p384.out");
	fOut << "SLURPYS OUTPUT" << endl;
	int testcase;
	fIn >> testcase;
	for (int i = 0; i < testcase; i++)
	{
		Input();
		MainAlgorithm();
	}
	fOut << "END OF OUTPUT" << endl;
	
	fIn.close();
	fOut.close();
	return 0;
}


void MainAlgorithm()
{
	char temp[256] = { 0, };
	strcpy(temp, g_input);
	int a = IsSlimp(temp);
	strncpy(temp, &temp[a], strlen(temp) - (a - 1));
	int b = IsSlump(temp);
	if (temp[a + b] == 0)
	{
		if (a != NOTMATCH && b != NOTMATCH)
			fOut << "YES" << endl;
		else
			fOut << "NO" << endl;

	}
	else
		fOut << "NO" << endl;
}
void Input()
{
	memset(g_input, 0, sizeof(g_input));
	fIn >> g_input;
}
void Output()
{
	
}
int IsSlump(char input[])
{
	int count = 0;
	char temp[256] = {0,};
	strcpy(temp, input);
	if (temp[0] != 'D' && temp[0] != 'E')
		return NOTMATCH;

	if (temp[1] == 'F')
	{
		count = 1;
		while (temp[count] == 'F')
			count++;
		if (temp[count] == 'G')
			return count + 1;
		strncpy(temp, &temp[count], strlen(temp) - (count- 1));
		int a = IsSlump(temp);
		if (a != NOTMATCH)
			return a + count;
	}
	return NOTMATCH;
}
int IsSlimp(char input[]) // 해당하는 애의 글자수 리턴. 리턴
{
	int count = 0;
	char temp[256] = { 0, };
	strcpy(temp, input);
	if (temp[0] != 'A')
		return NOTMATCH;
	if (temp[1] == 'H')
		return 2; //
	else
	{
		count++;
		if (temp[1] == 'B')
		{
			count++;
			strncpy(temp, &temp[2], strlen(temp) - 1);
			int a= IsSlimp(temp);	//  카운트는 글자수.
			
			if (a != NOTMATCH)
			{
				if (temp[a] == 'C')
					return a + count + 1;
			}
		}
		else
		{
			count++;
			strncpy(temp, &temp[1], strlen(temp));
			int a = IsSlump(temp);
			if (a != NOTMATCH)
			{
				if (temp[a] == 'C')
					return a + count;
			}
		}

	}
	return NOTMATCH;
}