#pragma warning (disable:4996)
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <list>
#define NUMBER false
#define CHAR true
//��ȣ�� ���鶧�� ���ڰ� 1 �۾����� �� ���� ���ΰ� ���ڰ� Ŀ���� �����ʿ� ()�� �д�. ���ڰ� 
//1�̻� �۾����� ��ü�� ���Ѵ�.

//��ȣ�� ���ڷ� ���鶧�� (�� ���� ������ŭ )�� �������� ���ڰ� �ȴ�. )���� (�� �߰��� ���� ��
//������ )�� �¾ƾ� 1�� �۾����� �߰� (������ )������ �ٸ��ٸ� ������ �� ()�� ������ �ɷ� �����ؾ���.
using namespace std;

void MainAlgorithm();
void Input();
void Output();
void MakeNumber();
void MakeGualHo();
void InsertOutside(list<char> &lst, int from); // from~������ ���Ѵ�
void InsertRight(list<char> &lst);
void MakeGualHo(list<char> &lst, int maxNum);

bool g_answer;
ifstream fIn;
ofstream fOut;

bool g_Type;
vector<int> g_inputSutZZa;
vector<char> g_inputGualHo;
char g_inputCharTem[2048];
list<char> g_answerGualHo;
list<int> g_answerNumber;

vector<char> g_vecAnswerGualHo;
int main()
{
	fIn.open("transform.inp");
	fOut.open("transform.out");
	int testcase;
	fIn >> testcase;
	for (int i = 0; i < testcase; i++)
	{
		Input();
		MainAlgorithm();
		Output();

	}

	fIn.close();
	fOut.close();
	return 0;
}


void MainAlgorithm()
{
	if (g_Type == NUMBER)
	{
		MakeGualHo();
	}
	else
	{
		MakeNumber();
	}
}

void Input()
{
	int inputNum;
	fIn >> inputNum;
	fOut << inputNum << " ";
	fIn >> g_Type; //0�̶� 1 ����.
	if (g_Type == NUMBER)
	{
		int temp;
		for (int i = 0; i < inputNum; i++)
		{
			fIn >> temp;
			g_inputSutZZa.push_back(temp);
		}
	}
	else
	{

		fIn >> g_inputCharTem;
		int temp = strlen(g_inputCharTem);
		for (int i = 0; i < temp; i++)
			g_inputGualHo.push_back(g_inputCharTem[i]);

	}

}

void Output()
{
	if (g_Type == NUMBER)
	{
		for (list<char>::iterator iter = g_answerGualHo.begin(); iter != g_answerGualHo.end(); iter++)
			fOut << *iter;
	}
	else
	{
		for (list<int>::iterator iter = g_answerNumber.begin(); iter != g_answerNumber.end(); iter++)
			fOut << *iter<<" ";

	}
	
	fOut << endl;
	g_answerGualHo.clear();
	g_inputSutZZa.clear();
	g_inputGualHo.clear();
	g_answerNumber.clear();
}
void MakeNumber()
{
	vector<int> vecStack;
	int max = 1;
	int vecCount = g_inputGualHo.size();
	for(int i = 0;i< vecCount;i++)
	{
		if (g_inputGualHo[i] == '(')
		{
			vecStack.push_back(max);
			max++;
		}
		else
		{
			g_answerNumber.push_back(vecStack.back());
			vecStack.pop_back();
		}
	}

}
void MakeGualHo()
{
	int count = g_inputSutZZa[0];
	int minusCount = 1;
	int vecCount = g_inputSutZZa.size();

	for (int i = 0; i < g_inputSutZZa[0]; i++)
		g_answerGualHo.push_back('(');

	for (int i = 1; i < vecCount; i++)
	{
		if (g_inputSutZZa[i] - g_inputSutZZa[i - 1] == -1)
			minusCount++;
		else
		{
			for (int j = 0; j < minusCount; j++)
				g_answerGualHo.push_back(')');

			while (g_inputSutZZa[i] > count)
			{
				g_answerGualHo.push_back('(');
				count++;
			}
			minusCount = 1;
		}
	}
	for (int i = 0; i < minusCount; i++)
		g_answerGualHo.push_back(')');

}
