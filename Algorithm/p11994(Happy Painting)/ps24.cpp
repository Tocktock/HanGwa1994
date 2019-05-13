#include<fstream>
#include<algorithm>
#include<string.h>
#include<map>
#include<vector>

using namespace std;

class Node
{
public:
	int father;
	int color;
};

void MainAlgorithm();
void Input();
bool ExecuteInput();
void Output();
void Command_1(int x, int y,int c);
void Command_2(int x, int y, int c);
void Command_3(int x, int y);
bool IsAncestor(int x, int y);
bool IsExistPath(int x, int y);
int FindCommonAncestor(int x, int y);
ifstream fIn;
ofstream fOut;

int g_nodeNum;
int g_executeNum;
int g_command[4];
vector<Node> g_vecNodeFather; //노드의 아빠 저장.
bool g_bColorSet[64];
int g_edgeAnswer, g_colorAnswer;

int main()
{
	fIn.open("ps24.inp");
	fOut.open("ps24.out");

	
	Input();
	for (int i = 0; i < g_executeNum; i++)
	{
		ExecuteInput();
		MainAlgorithm();
	}
	fIn.close();
	fOut.close();
	return 0;
}


void MainAlgorithm()
{
	if (g_command[0] == 1)
	{
		Command_1(g_command[1], g_command[2], g_command[3]);
	}
	else if (g_command[0] == 2)
	{
		Command_2(g_command[1], g_command[2], g_command[3]);

	}
	else if (g_command[0] == 3)
	{
		Command_3(g_command[1], g_command[2]);
		Output();
	}
}

void Input()
{
	fIn >> g_nodeNum >> g_executeNum;
	Node tmpNode;
	tmpNode.father = -1;
	tmpNode.color = -1;
	g_vecNodeFather.push_back(tmpNode); // 1부터 "쓸려고.
	for (int i = 0; i < g_nodeNum; i++)
	{
		fIn >> tmpNode.father;
		g_vecNodeFather.push_back(tmpNode);
	}
	for (int i = 0; i < g_nodeNum; i++)
	{
		fIn >> tmpNode.color;
		g_vecNodeFather[i + 1].color = tmpNode.color;
	}
}

void Output()
{
	fOut << g_edgeAnswer << " " << g_colorAnswer << endl;
}
bool ExecuteInput()
{
	fIn >> g_command[0] >> g_command[1] >> g_command[2];
	if (g_command[0] != 3)
		fIn >> g_command[3];

	if ((g_command[0], g_command[1], g_command[2]) == EOF)
		return false;

	return true;
}
void Command_1(int x, int y, int c)
{
	if (IsAncestor(x, y) || x == y)
		return;
	g_vecNodeFather[x].father = y;
	g_vecNodeFather[x].color = c;
}
void Command_2(int x, int y, int c)
{
	if (!IsExistPath(x, y))
		return;

	int commonAncestor = FindCommonAncestor(x, y);

	while (x != commonAncestor)
	{
		g_vecNodeFather[x].color = c;
		x = g_vecNodeFather[x].father;
	}
	while ( y != commonAncestor)
	{
		g_vecNodeFather[y].color = c;
		y = g_vecNodeFather[y].father;
	}
}
void Command_3(int x, int y)
{
	memset(g_bColorSet, false, sizeof(g_bColorSet));
	g_colorAnswer = 0;
	g_edgeAnswer = 0;

	if (!IsExistPath(x, y))
		return;

	int commonAncestor = FindCommonAncestor(x, y);
	while (x != commonAncestor)
	{
		g_edgeAnswer++;
		g_bColorSet[g_vecNodeFather[x].color] = true;
		x = g_vecNodeFather[x].father;
	}
	while (y != commonAncestor)
	{
		g_edgeAnswer++; 
		g_bColorSet[g_vecNodeFather[y].color] = true;
		y = g_vecNodeFather[y].father;
	}
	for (int i = 0; i < 50; i++)
		if (g_bColorSet[i] == true)
			g_colorAnswer++;
}
int FindCommonAncestor(int x, int y)
{
	vector<int> xAncestorSet, yAncestorSet; //자기자신도 포함.
	int xRoot, yRoot;
	xRoot = x;
	xAncestorSet.push_back(x);
	while (g_vecNodeFather[xRoot].father != 0)
	{
		xRoot = g_vecNodeFather[xRoot].father;
		xAncestorSet.push_back(xRoot);
	}

	yRoot = y;
	yAncestorSet.push_back(y);
	while (g_vecNodeFather[yRoot].father != 0)
	{
		yRoot = g_vecNodeFather[yRoot].father;
		yAncestorSet.push_back(yRoot);
	}
	int startIndex = 0;
	if (xAncestorSet.size() < yAncestorSet.size()) //조상이 더 많다는거는 레벨이 더 높다는 것이다이말이야.
	{
		startIndex = yAncestorSet.size() - xAncestorSet.size(); //y가 더크다.
		for (int i = 0; i < xAncestorSet.size(); i++)
		{
			if (xAncestorSet[i] == yAncestorSet[i + startIndex])
				return xAncestorSet[i];
		}
	}
	else
	{
		startIndex = xAncestorSet.size() - yAncestorSet.size(); // x가 더크다
		for (int i = 0; i < yAncestorSet.size(); i++)
		{
			if (xAncestorSet[i + startIndex] == yAncestorSet[i])
				return yAncestorSet[i];
		}
	}

}
bool IsAncestor(int x, int y)
{
	while (g_vecNodeFather[y].father != 0)
	{
		if (g_vecNodeFather[y].father == x)
			return true;
		y = g_vecNodeFather[y].father;
	}
	return false;
}
bool IsExistPath(int x, int y)
{
	int xRoot,yRoot;
	xRoot = x;
	while (g_vecNodeFather[xRoot].father != 0)
	{
		xRoot = g_vecNodeFather[xRoot].father;
	}

	yRoot = y;
	while (g_vecNodeFather[yRoot].father != 0)
	{
		yRoot = g_vecNodeFather[yRoot].father;
	}

	if (xRoot == yRoot)
		return true;
	return false;
}