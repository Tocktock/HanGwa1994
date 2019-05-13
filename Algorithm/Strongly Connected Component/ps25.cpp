#include<fstream>
#include<algorithm>
#include<string.h>
#include<map>
#include<vector>

using namespace std;

class VertexInfo
{
public:
	vector<int> connected;
	int leavingTime;
	int arriveTime;
};

void MainAlgorithm();
void Input();
void Output();
void Dfs(int vertex);
void InversedDfs(int vertex);

ifstream fIn;
ofstream fOut;

vector<VertexInfo> g_vertexInfo;
vector<VertexInfo> g_vertexInversedInfo;
vector< vector<int> > g_commonSet;
int g_commonSetCount = 0;
vector<bool> g_usedVertex;
vector<int> g_leaveOrder;
int main()
{
	fIn.open("ps25.inp");
	fOut.open("ps25.out");


	Input();
	MainAlgorithm();
	Output();


	fIn.close();
	fOut.close();
	return 0;
}


void MainAlgorithm()
{
	g_usedVertex.assign(g_vertexInfo.size(), false); //정방향
	int count = g_vertexInfo.size();
	for (int i = 0; i < count; i++)
		Dfs(i);
	
	g_usedVertex.assign(g_vertexInfo.size(), false); //역방향
	count = g_leaveOrder.size();
	vector<int> tmp;
	g_commonSet.push_back(tmp);
	for (int i = count - 1; i >= 0; i--)
	{
		if (g_usedVertex[g_leaveOrder[i]] == false)
		{

			g_commonSetCount++;
			vector<int> tmp;
			g_commonSet.push_back(tmp);
			InversedDfs(g_leaveOrder[i]);
		}
	}
	
}

void Input()
{
	int vertexNum;
	int edgeNum;
	fIn >> vertexNum >> edgeNum;
	VertexInfo tmp;
	tmp.arriveTime = 0;
	tmp.leavingTime = 0;
	g_vertexInfo.assign(vertexNum, tmp);
	g_vertexInversedInfo.assign(vertexNum, tmp);
	int vertex1, vertex2;
	for (int i = 0; i < edgeNum; i++)
	{
		fIn >> vertex1 >> vertex2;
		g_vertexInfo[vertex1].connected.push_back(vertex2);
		g_vertexInversedInfo[vertex2].connected.push_back(vertex1);
	}
	
}

void Output()
{
	fOut << g_commonSetCount;
}
void Dfs(int vertex)
{
	if (g_usedVertex[vertex] == true)
		return;
	g_usedVertex[vertex] = true;
	for (int i = 0; i < g_vertexInfo[vertex].connected.size(); i++)
	{
		Dfs(g_vertexInfo[vertex].connected[i]);
	}
	g_leaveOrder.push_back(vertex);
}
void InversedDfs(int vertex)
{
	if (g_usedVertex[vertex] == true)
		return;
	g_usedVertex[vertex] = true;
	g_commonSet[g_commonSetCount].push_back(vertex);
	for (int i = 0; i < g_vertexInversedInfo[vertex].connected.size(); i++)
	{
		InversedDfs(g_vertexInversedInfo[vertex].connected[i]);
	}
	g_commonSet[g_commonSetCount].push_back(vertex);
}