#pragma warning (disable:4996)
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <list>
#define NUMBER false
#define CHAR true
//괄호로 만들때는 숫자가 1 작아지면 양 옆을 감싸고 숫자가 커지면 오른쪽에 ()를 둔다. 숫자가 
//1이상 작아지면 전체를 감싼다.

//괄호를 숫자로 만들때는 (가 나온 개수만큼 )가 나왔을때 숫자가 된다. )이후 (가 추가로 들어올 때
//개수는 )와 맞아야 1씩 작아진다 추가 (개수와 )개수가 다르다면 오른쪽 에 ()가 생성된 걸로 생각해야함.
using namespace std;

void MainAlgorithm();
void Input();
void Output();
void MakeNumber();
void MakeGualHo();
void InsertOutside(list<char> &lst, int from); // from~끝까지 감싼다
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
	fIn >> g_Type; //0이랑 1 들어옴.
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
