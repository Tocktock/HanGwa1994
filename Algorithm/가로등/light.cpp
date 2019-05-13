

#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<queue>

class LIGHT_INFO
{
public:
	int left;
	int right;

};
using namespace std;

void Input();
void Output();
void MainAlgolritm();
int FindMin(int left, int right);
int FindLeft(int left, int oldLeftIndex);
int FindRight(int leftIndex, int oldRightIndex);
bool Cmp(LIGHT_INFO a, LIGHT_INFO b);
vector<LIGHT_INFO> g_vecLight;
ifstream fIn;
ofstream fOut;
int g_lightNum;
int g_maxRight;

int main()
{
	fIn.open("light.inp");
	fOut.open("light.out");

	Input();
	Output();
	MainAlgolritm();

	fIn.close();
	fOut.close();
}
void Input()
{
	fIn >> g_lightNum;
	LIGHT_INFO temp;
	for (int i = 0; i < g_lightNum; i++)
	{
		fIn >> temp.left >> temp.right;
		g_vecLight.push_back(temp);
		if (g_maxRight < temp.right)
			g_maxRight = temp.right;
	}
}

void MainAlgolritm()
{
	int inputLeft, inputRight;
	int startPoint = 0;
	int eraseNum = 0;
	sort(g_vecLight.begin(), g_vecLight.end(), Cmp);

	for (int i = 0; i < g_vecLight.size() - 1; i++) // left가 같은데 right 가 작은 애들은 없앤다.
	{
		if (g_vecLight[i].left == g_vecLight[i + 1].left && g_vecLight[i].right <= g_vecLight[i + 1].right)
		{
			eraseNum++;

		}
		else
		{
			g_vecLight.erase(g_vecLight.begin() + startPoint, g_vecLight.begin() + i);
			i -= eraseNum;
			eraseNum = 0;
			startPoint = i + 1;
		}
	}
	int caseNum;
	fIn >> caseNum;
	for (int i = 0; i < caseNum; i++) // 입력 left 값 보다 작거나 같은 애들 중에서 right가 가장 큰 애를 찾음.
	{
		fIn >> inputLeft >> inputRight;
		fOut << FindMin(inputLeft, inputRight) << endl;
	}
}

void Output()
{

}
bool Cmp(LIGHT_INFO a, LIGHT_INFO b)
{
	if (a.left == b.left)
		return a.right < b.right;
	else
		return a.left < b.left;
}
int FindMin(int left, int right)
{
	int answer = 0;
	int count = 0;
	int findLeftIndex = -1;
	int findRightIndex = -1;
	int findLeft = left;
	int findRight = 0;
	if (g_vecLight[0].left > left || g_maxRight < right)
		return -1;

	while (findLeft < right)
	{
		findLeftIndex = FindLeft(findLeft, findLeftIndex);
		if (findLeftIndex == -1)
		{
			answer = -1;
			break;
		}
		findRightIndex = FindRight(findLeftIndex, findRightIndex);
		if (findRightIndex == -1)
		{
			answer = -1;
			break;
		}
		findLeft = g_vecLight[findRightIndex].right;
		answer++;
		if (answer > g_vecLight.size())
			return -1;
	}
	return answer;
}

int FindLeft(int left, int oldLeftIndex)
{
	int i = oldLeftIndex;
	if (i == -1)
		i = 0;
	for (; i < g_vecLight.size(); i++)
	{
		if (g_vecLight[i].left == left)
		{
			break;
		}
		else if (g_vecLight[i].left > left && i > 0)
		{
			i = i - 1;
			break;
		}
	}

	if (i == oldLeftIndex)
		return -1;
	if (i == g_vecLight.size())
		return i - 1;
	return i;
}
int FindRight(int leftIndex, int oldRightIndex)
{
	int maxRightIndex = leftIndex;
	int maxRight = g_vecLight[maxRightIndex].right;
	int i = oldRightIndex;
	if (i == -1)
		i = 0;
	for (; i <= leftIndex; i++)
	{
		if (maxRight < g_vecLight[i].right)
		{
			maxRightIndex = i;
			maxRight = g_vecLight[i].right;
		}
	}
	return maxRightIndex;
}
