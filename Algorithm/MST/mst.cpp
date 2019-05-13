
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;
struct PARENT_CHILD
{
	int parent;
	int child;
};

struct EDGE_INFO
{
	int sVertex;
	int dVertex;
	int weight;
	int edgeNo;
	bool isUsed;
};
struct CMP
{
	bool operator()(EDGE_INFO a, EDGE_INFO b) {
		if (a.weight != b.weight)
			return a.weight > b.weight;// > 이거면 작은거 리턴 < 이거면 큰거 리턴
		else
			return a.edgeNo > b.edgeNo;
	}
};
struct VERTEX_INFO
{
	int childV;
	int parentV;
	int weight;
	int edgeNo;
};

void Input();
void MainAlgorithm();
void Output();
void Kruskal();
void Prim(int start);
void MergeSort();
bool Compare_Edge(EDGE_INFO a, EDGE_INFO b);
bool Compare_for_Prim(int a, int b);
int GetFalseVertex(int a, int b);
void ChangeParent(int origin, int parent);
int GetNewPosition(int edggeIndex);

ifstream fIn;
ofstream fOut;
int g_vertexNum;
int g_edgeNum;

VERTEX_INFO g_vertex[10000][10000];
EDGE_INFO g_edge[100001];
EDGE_INFO g_edgeCopy[100001];

vector<int> g_vertexChild[10000];
int g_vertexParent[10000];
bool g_isUsedVertex[10000];
vector<int> g_connect[10000];
vector<int> g_connectCopy[100000];
vector<int> g_waiting;
priority_queue<EDGE_INFO, vector<EDGE_INFO>, CMP> PQ;
int g_answer[100000];
long long g_answerCost;

int main()
{
	fIn.open("mst.inp");
	fOut.open("mst.out");

	Input();
	MainAlgorithm();
	Output();

	fIn.close();
	fOut.close();

}

void Input()
{
	fIn >> g_vertexNum;
	fIn >> g_edgeNum;
	int sVertex, dVertex, weight;

	for (int i = 0; i < g_edgeNum; i++)
	{
		fIn >> sVertex >> dVertex >> weight;
		if (dVertex < sVertex) // 큰 값을 d로 넣는다 //초키 자식과 부모는 자기 값으로.
		{
			int temp = sVertex;
			g_edge[i].sVertex = dVertex;
			g_edge[i].dVertex = temp;
		}
		g_edge[i].dVertex = dVertex;
		g_edge[i].sVertex = sVertex;
		g_edge[i].edgeNo = i;
		g_edge[i].weight = weight;

		g_vertex[sVertex][dVertex].parentV = dVertex;
		g_vertex[sVertex][dVertex].childV = sVertex;

		g_vertex[sVertex][dVertex].edgeNo = i;
		g_vertex[sVertex][dVertex].weight = weight;

		g_vertex[dVertex][sVertex].edgeNo = i;
		g_vertex[dVertex][sVertex].weight = weight;
		if (dVertex < sVertex)
		{
			g_vertex[sVertex][dVertex].parentV = sVertex;
			g_vertex[dVertex][sVertex].parentV = sVertex;
			g_vertex[sVertex][dVertex].childV = dVertex;
			g_vertex[dVertex][sVertex].childV = dVertex;
		}

		else
		{
			g_vertex[sVertex][dVertex].parentV = dVertex;
			g_vertex[dVertex][sVertex].parentV = dVertex;
			g_vertex[sVertex][dVertex].childV = sVertex;
			g_vertex[dVertex][sVertex].childV = sVertex;
		}
		g_connect[dVertex].push_back(i);
		g_connect[sVertex].push_back(i);

	}
}
void MainAlgorithm()
{
	Kruskal();
	Prim(0);
	Prim(g_vertexNum / 2);
	Prim(g_vertexNum - 1);
}

void Output()
{

}

void Kruskal()
{
	g_answerCost = 0;
	for (int i = 0; i < 10000; i++) // 초기 자기 부모는 자기로.
	{
		g_vertexParent[i] = i;
	}
	int g_answerC = 0;
	int originP, compareP;
	for (int i = 0; i < g_edgeNum; i++)
	{
		g_edgeCopy[i] = g_edge[i];
	}
	sort(g_edgeCopy, g_edgeCopy + g_edgeNum, Compare_Edge);

	for (int i = 0; i < g_edgeNum; i++)
	{
		originP = g_vertexParent[g_edgeCopy[i].sVertex];
		compareP = g_vertexParent[g_edgeCopy[i].dVertex];

		if (originP == compareP)
			continue;
		g_answer[g_answerC] = g_edgeCopy[i].edgeNo;
		g_answerC++;
		g_answerCost += g_edgeCopy[i].weight;
		if (originP < compareP)
		{
			g_vertexChild[compareP].push_back(originP);// 큰애 밑으로 자식으로 넣어줌.
			ChangeParent(originP, compareP); // 자식이 여러명인ㄱ 것도 처리
		}
		else
		{
			g_vertexChild[originP].push_back(compareP);// 큰애 밑으로 자식으로 넣어줌.
			ChangeParent(compareP, originP); // 자식이 여러명인ㄱ 것도 처리
		}
	}

	fOut << "Tree edges by Kruskal algorithm: " << g_answerCost << endl;

	for (int i = 0; i < g_answerC; i++)
	{
		fOut << g_answer[i] << endl;
	}
}
void Prim(int start)
{
	while (!PQ.empty())
		PQ.pop();

	for (int i = 0; i < g_vertexNum; i++)
		g_isUsedVertex[i] = false;
	int answerCount = 0;
	int answerCost = 0;
	int answerStart = start;
	for (int i = 0; i < g_edgeNum; i++)
		g_edgeCopy[i] = g_edge[i];
	for (int i = 0; i < g_edgeNum; i++)
		g_edgeCopy[i].isUsed = false;
	g_isUsedVertex[start] = true;
	EDGE_INFO now;
	while (answerCount < g_vertexNum - 1)
	{
		for (int i = 0; i < g_connect[start].size(); i++)
		{
			if (g_edgeCopy[g_connect[start][i]].isUsed == true)
				continue;

			PQ.push(g_edgeCopy[g_connect[start][i]]);
			g_edgeCopy[g_connect[start][i]].isUsed = true;
		}
		while (true)
		{

			now = PQ.top();
			if (g_isUsedVertex[now.sVertex] == true &&
				g_isUsedVertex[now.dVertex] == true)
			{
				PQ.pop();
			}
			else
				break;
		}

		answerCost += now.weight;
		g_answer[answerCount] = now.edgeNo;
		answerCount++;
		start = GetFalseVertex(now.sVertex, now.dVertex);
		g_isUsedVertex[now.sVertex] = true;
		g_isUsedVertex[now.dVertex] = true;
		PQ.pop();


	}
	fOut << "Tree edges by Prim algorithm with starting vertex " << answerStart << ": " << answerCost << endl;

	for (int i = 0; i < answerCount; i++)
	{
		fOut << g_answer[i] << endl;
	}
	/*for (int i = 0; i < g_vertexNum; i++)
	{
		g_connectCopy[i].assign(g_connect[i].begin(), g_connect[i].end());
	}
	for (int i = 0; i < g_connectCopy[start].size(); i++)
		g_waiting.push_back(g_connectCopy[start][i]);
	for (int i = 0; i < g_vertexNum; i++)
		g_bPrim[i] = false;
	g_bPrim[start] = true;
	sort(g_waiting.begin(), g_waiting.end(), Compare_for_Prim);

	while (true)
	{
		for (int i = 0; i < g_waiting.size(); i++)
		{
			if (g_bPrim[g_edge[g_waiting[i]].sVertex] == true &&
				g_bPrim[g_edge[g_waiting[i]].dVertex] == true)
			{
				g_waiting.erase(g_waiting.begin(), g_waiting.begin() + 1);
			}
			else
			{
				g_answer[answerCount] = g_edge[g_waiting[i]].edgeNo;
				start = GetFalseVertex(g_edge[g_waiting[i]].sVertex, g_edge[g_waiting[i]].dVertex);
				g_waiting.erase(g_waiting.begin(), g_waiting.begin() + 1);
				g_bPrim[g_edge[g_waiting[i]].sVertex] = true;
				g_bPrim[g_edge[g_waiting[i]].dVertex] = true;

			}
			int position;
			for (int i = 0; i < g_connectCopy[start].size(); i++)
			{
				position = GetNewPosition(g_connectCopy[start][i]);
				g_waiting.insert(g_waiting.begin() + GetNewPosition(g_connectCopy[start][i]), g_connectCopy[start][i]);
			}
		}
	}*/
	//2/n에서
	//n - 1에서
}
void MergeSort()
{
	int num = g_edgeNum;
	int size = 1;
	int start = 0;
	int leftIndex;
	int leftC = 0;
	int rightC = 0;
	int rightIndex;
	while (true) // 전체 머지 반복
	{
		while (leftC + rightC <= g_edgeNum) // 내부에서 반복
		{
			leftIndex = start;
			rightIndex = start + size;
			leftC = 0;
			rightC = 0;
			for (int i = start; i < size + start; i++)
			{

			}
			start += size * 2;
		}
		size = size * 2;

	}
}

bool Compare_Edge(EDGE_INFO a, EDGE_INFO b)
{
	if (a.weight < b.weight)
		return true;
	else if (a.weight == b.weight)
	{
		if (a.edgeNo < b.edgeNo)
			return true;
		else
			return false;
	}
	else
		return false;
}

void ChangeParent(int origin, int parent)
{
	int child;
	g_vertexParent[origin] = parent;
	for (int i = 0; i < g_vertexChild[origin].size(); i++) // 자식이 여러명일 수도 있으니까. 전부 바꿔줌.
	{
		child = g_vertexChild[origin][i];
		ChangeParent(child, parent);
	}
}
bool Compare_for_Prim(int a, int b)
{
	if (g_edge[a].weight < g_edge[b].weight) // a리턴
		return true;
	else if (g_edge[a].weight == g_edge[b].weight)
	{
		if (a < b)
			return true;
		else
			return false;
	}
	else // b리턴
		return false;
}
int GetFalseVertex(int a, int b)
{
	if (g_isUsedVertex[a] == false)
		return a;
	else
		return b;
}
int GetNewPosition(int edggIndex)
{
	for (int i = 0; i < g_waiting.size(); i++)
	{
		if (g_edge[edggIndex].weight < g_edge[g_waiting[i]].weight)
			return i;
	}
	return g_waiting.size();

}