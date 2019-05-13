
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>

using namespace std;

ifstream g_fIn;
ofstream g_fOut;

void Input();
void Output();
void MainAlgolritm();
int GetLastNumber(int seqNo);

vector<int> g_vecCardSeq;
int g_output[2000];
int g_caseNum;
int g_inputSeq[2000];
int g_inputCSeq[2000];

int main()
{
	g_fIn.open("sequence.inp");
	g_fOut.open("sequence.out");

	Input();
	MainAlgolritm();
	Output();

	g_fIn.close();
	g_fOut.close();
	return 0;
}

void Input()
{

	g_fIn >> g_caseNum;

	int temp;

	for (int i = 0; i < g_caseNum; i++)
	{
		g_fIn >> temp;
		g_vecCardSeq.push_back(temp);
	}

	for (int i = 0; i < g_caseNum; i++)
	{
		g_fIn >> g_inputCSeq[i];
	}
}

void Output()
{
	for (int i = 0; i < g_caseNum; i++)
		g_fOut << g_output[i] << endl;
}

void MainAlgolritm()
{
	sort(g_vecCardSeq.begin(), g_vecCardSeq.end());

	for (int i = g_caseNum - 1; i >= 0; i--)
	{
		g_output[i] = GetLastNumber(i);
	}

}
int GetLastNumber(int seqNo)
{
	int temp = 0;

	temp = g_vecCardSeq[g_inputCSeq[seqNo]];
	g_vecCardSeq.erase(g_vecCardSeq.begin() + g_inputCSeq[seqNo]);
	return temp;
}