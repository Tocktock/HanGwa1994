
#include<iostream>
#include<fstream>
#include<string.h>
using namespace std;

ifstream fIn;
ofstream fOut;

char g_inputData[60000];
int g_inR[30000];
int g_caseNum;

void Input();
void Output();
void MainAlgolritm();

int main()
{
	fIn.open("tree.inp");
	fOut.open("tree.out");
	fIn >> g_caseNum;
	char temp[2];
	fIn.getline(temp, 1);
	for (int i = 0; i < g_caseNum; i++)
	{
		Input();
		Output();
		MainAlgolritm();
	}


	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{
	fIn.getline(g_inputData, 60000);
}

void Output()
{

}

void MainAlgolritm()
{
	//pre
	int rCount = 0;
	char szTemp;
	fOut << g_inputData << endl << "Preorder" << endl;
	for (int i = 0; i < strlen(g_inputData); i++)
	{
		if (g_inputData[i] == ' ' || g_inputData[i] == ')')
			continue;
		else if (g_inputData[i] == '(')
		{
			fOut << "r" << rCount << endl;
			rCount++;
		}
		else
		{
			szTemp = g_inputData[i];
			fOut << szTemp;
			while (g_inputData[i + 1] != ' ')
			{
				fOut << g_inputData[i + 1];
				i++;
			}
			fOut << endl;
		}


	}

	rCount = 0;
	int answerCount = -1;
	fOut << "Inorder" << endl;
	for (int i = 0; i < strlen(g_inputData); i++)
	{
		if (g_inputData[i] == ' ' || g_inputData[i] == ')')
			continue;
		else if (g_inputData[i] == '(')
		{
			answerCount++;
			g_inR[answerCount] = rCount;
			rCount++;
		}
		else
		{
			szTemp = g_inputData[i];
			fOut << szTemp;
			while (g_inputData[i + 1] != ' ')
			{
				fOut << g_inputData[i + 1];
				i++;
			}
			fOut << endl;
			if (answerCount != -1)
			{
				fOut << 'r' << g_inR[answerCount] << endl;
				answerCount--;
			}
		}
	}
}