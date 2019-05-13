
#include <iostream>
#include <fstream>
#include <string.h>

#define ALICE true
#define COMPUTER false

using namespace std;

void Input();
void MainAlgoritm();
void Output();
int Check(int left, int right, bool turn);

ofstream fOut;
ifstream fIn;

int g_inputNum;
int g_card[1024];
int g_answer;
int g_computer[1024][1024];
int g_alice[1024][1024];


int main()
{
	fIn.open("card.inp");
	fOut.open("card.out");

	int caseNum = 0;

	fIn >> caseNum;
	for (int i = 0; i < caseNum; i++)
	{
		memset(g_alice, -1, sizeof(g_alice));
		memset(g_computer, -1, sizeof(g_computer));
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
	g_answer = Check(0, g_inputNum - 1, ALICE);
}
void Output()
{
	fOut << g_answer << endl;;
}

int Check(int leftC, int rightC, bool turn)
{
	//�ٸ����� ����(���� �̱� ����), ��ǻ�ʹ� ����� ���� �ٸ����� ���� ���� ���� �̰�, ��ǻ�ʹ� �ٸ����� ���� ���� ���� �̰� �ؾ���.
	if (leftC > rightC)
		return 0; //���� ī�尡 ���� ����

	int answer;
	int frontCard;
	int backCard;

	if (turn == ALICE)//�ٸ����� ���� ���� ���� �̴´�.
	{
		if (g_alice[leftC][rightC] != -1)
			return g_alice[leftC][rightC]; // �̹� ����߱� ������ �ٽ� ��� x
		frontCard = g_card[leftC] + Check(leftC + 1, rightC, COMPUTER); // �տ� �� �̰� ��ǻ�Ͱ� ���� �� �� �̴� ���
		backCard = g_card[rightC] + Check(leftC, rightC - 1, COMPUTER); // �ڿ� �� �̰� ��ǻ�Ͱ� ���� �� �� �̴� ���

		if (frontCard < backCard)
		{
			answer = backCard;
		}
		else
		{
			answer = frontCard;
		}
		g_alice[leftC][rightC] = answer;
	}
	else // ��ǻ�ʹ� �ٸ����� ���� ���� ���ڸ� �̰� �ؾ��Ѵ�.
	{
		if (g_computer[leftC][rightC] != -1)
			return g_computer[leftC][rightC];// �̹� ����߱� ������ �ٽ� ��� x
		frontCard = Check(leftC + 1, rightC, ALICE);
		backCard = Check(leftC, rightC - 1, ALICE);

		if (frontCard < backCard)
		{
			answer = frontCard;
		}
		else
		{
			answer = backCard;
		}
		g_computer[leftC][rightC] = answer;
	}
	return answer;
}