
#include<iostream>
#include<fstream>
#include<algorithm>
#include <vector>
using namespace std;

ofstream fOut;
ifstream fIn;

struct VERTEX_INFO
{
	int sVertex;
	int edgeLength;
};

void Input();
void MainAlgorithm();
void Output();
void Reset();
bool Search(int sVertex, int checkVertex);

int g_vertexNum;
int g_edgeNum;
int g_queryNum;
int g_agentP1, g_agentP2;
int g_answerC = 0;
VERTEX_INFO g_vtInfo[512][512];
VERTEX_INFO g_vtCopy[512][512];
vector<int> g_vecSequence[512]; //��� ������ ������ ������ ����.
vector<int> g_answerSequence[512];


int main()
{
	fIn.open("contact.inp");
	fOut.open("contact.out");

	int caseNum;
	fIn >> caseNum;

	for (int i = 0; i < caseNum; i++)
	{
		Input();
		for (int j = 0; j < g_queryNum; j++)
		{
			MainAlgorithm();
			Output();
		}
	}
	int a;
	for (int i = 0; i < 1000000000; i++)
	{
		a = i;

	}
	fIn.close();
	fOut.close();
}
void Input()
{
	fIn >> g_vertexNum;
	fIn >> g_edgeNum;
	fIn >> g_queryNum;

	Reset();
	int sVertex;
	int dVertex;
	int edgeL;
	VERTEX_INFO temp;
	for (int i = 0; i < g_edgeNum; i++) //������ ������ �� ������ ���ε��� ����.
	{
		fIn >> sVertex;
		fIn >> dVertex;
		fIn >> edgeL;
		if (sVertex == dVertex)
			continue;

		if (g_vtInfo[sVertex][dVertex].edgeLength > edgeL)
		{
			g_vtInfo[sVertex][dVertex].edgeLength = edgeL;
			g_vtInfo[sVertex][dVertex].sVertex = sVertex;
		}
		if (g_vtInfo[dVertex][sVertex].edgeLength > edgeL)
		{

			g_vtInfo[dVertex][sVertex].edgeLength = edgeL;
			g_vtInfo[dVertex][sVertex].sVertex = dVertex;
		}

		g_vecSequence[sVertex].push_back(dVertex);
		g_vecSequence[dVertex].push_back(sVertex);
	}
}
void MainAlgorithm()
{
	fIn >> g_agentP1;
	fIn >> g_agentP2;
	vector<int> vecTemp;

	for (int i = 1; i <= g_vertexNum; i++)
	{
		for (int j = 1; j <= g_vertexNum; j++)
			g_vtCopy[i][j] = g_vtInfo[i][j];
	}
	for (int i = 1; i <= g_vertexNum; i++)
	{
		g_answerSequence[i].clear();
	}
	vecTemp.assign(g_vecSequence[g_agentP1].begin(), g_vecSequence[g_agentP1].end());

	for (int i = 0; i < vecTemp.size(); i++)
	{
		g_answerSequence[vecTemp[i]].push_back(g_agentP1);
	}
	int cost = 0;
	while (vecTemp.size() != 0)
	{
		int firstSequence = vecTemp[0]; // agent1�� ���������� ������ ó�� �ֺ��� �˻����.
		for (int i = 0; i < g_vecSequence[firstSequence].size(); i++)
		{
			int checkVertex = g_vecSequence[firstSequence][i]; // agent1 ������ ó�� �ִ� ���� �������� �� �պ��� ����.
			cost = g_vtCopy[g_agentP1][firstSequence].edgeLength + g_vtCopy[firstSequence][checkVertex].edgeLength;
			//cost = agent1 - firstSequence �� �Ÿ� + firstSequence + checkVertex

			if (g_vtCopy[g_agentP1][checkVertex].edgeLength == 100000 ||
				g_vtCopy[g_agentP1][checkVertex].edgeLength > cost) //���� ���� ���� �ְų� �� ���� ��� �����������
			{//���� �� �� ��� ���ʹ� ��� ������.
				g_vtCopy[g_agentP1][checkVertex].edgeLength = cost;
				g_vtCopy[g_agentP1][checkVertex].sVertex = firstSequence;
				g_answerSequence[checkVertex].clear();
				g_answerSequence[checkVertex].push_back(firstSequence);

				vecTemp.push_back(checkVertex);

			}

			else if (g_vtCopy[g_agentP1][checkVertex].edgeLength == cost) //���� ���� ���� �ְų� �� ���� ��� �����������
			{

				g_answerSequence[checkVertex].push_back(firstSequence);
				if (g_vtCopy[g_agentP1][checkVertex].sVertex > firstSequence)
					g_vtCopy[g_agentP1][checkVertex].sVertex = firstSequence;
			}
		}
		vecTemp.erase(vecTemp.begin());
	}
}
void Output()
{
	g_answerC++;
	//agent1 ��ġ���� 2���� �����鼭 ��ġ�� ����.
	float answerCost = (float)g_vtCopy[g_agentP1][g_agentP2].edgeLength / 2;
	int start;
	int end = g_agentP2;
	vector<int> answer[2];

	bool flag = true; // ��ΰ� �Ѱ����� �������� true 

	while (flag != false)
	{
		flag = false;
		answerCost = (float)g_vtCopy[g_agentP1][g_agentP2].edgeLength / 2;
		end = g_agentP2;
		while (true)
		{
			start = g_answerSequence[end].back();
			if (g_answerSequence[end].size() > 1)
			{
				g_answerSequence[end].pop_back();
				flag = true;
			}
			answerCost -= g_vtCopy[start][end].edgeLength;
			if (answerCost < 0)
			{
				break;
			}
			else if (answerCost == 0)
			{
				start = end;
				break;
			}
			end = start;

			if (start == g_agentP1)
				break;
		}

		if (end < start)
		{
			int temp = start;
			start = end;
			end = temp;
		}

		answer[0].push_back(start);
		answer[1].push_back(end);

	}
	start = answer[0][0];
	end = answer[1][0];
	for (int i = 1; i < answer[0].size(); i++)
	{
		if (start > answer[0][i])
		{

			start = answer[0][i];
			end = answer[1][i];
		}
	}
	////	while (true)
	////	{
	////		float answerCost = g_vtInfo[g_agentP1][g_agentP2].edgeLength / 2;
	//		while (true)
	//		{ //�ȿ��� ��� �˻�.
	//			answerCost -= g_vtInfo[start][end].edgeLength;
	//			if (answerCost < 0)
	//			{
	//				break;
	//			}
	//			else if (answerCost == 0)
	//			{
	//				start = end;
	//				break;
	//			}
	//			start = g_vtInfo[g_agentP1][g_agentP2].sVertex;
	//			end = g_vtInfo[g_agentP1][start].sVertex;
	//		}
	//		//start�� ũ�� �� ��°����
	////	}

	fOut << start << " " << end << endl;

}


void Reset()
{
	for (int i = 1; i <= g_vertexNum; i++)
	{
		for (int j = 1; j <= g_vertexNum; j++)
		{
			g_vtInfo[i][j].edgeLength = 100000;
			g_vtInfo[i][j].sVertex = -1;
		}
	}
	for (int i = 1; i <= g_vertexNum; i++)
	{
		g_vtInfo[i][i].edgeLength = 0;
	}

	for (int i = 1; i <= g_vertexNum; i++)
	{
		g_vecSequence[i].clear();
	}
	for (int i = 1; i <= g_vertexNum; i++)
	{
		g_answerSequence[i].clear();
	}

}
bool Search(int sVertex, int checkVertex)
{
	vector<int>::iterator iter;

	for (iter = g_vecSequence[sVertex].begin(); iter != g_vecSequence[sVertex].end(); iter++)
	{
		if (checkVertex == *iter)
			return true; //����
	}
	return false;// �������.
}

//1
//7 10 1
//1 2 8
//1 4 4
//1 5 4
//1 6 1
//2 3 10
//2 7 3
//3 4 9
//3 5 6
//5 7 8
//6 7 1
//3 7