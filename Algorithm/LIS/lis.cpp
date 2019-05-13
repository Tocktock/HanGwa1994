
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<string.h>
using namespace std;

struct INDEX_DATA
{
	int index;
	int data;
};

struct ANSWER_DATA
{
	int data;
	unsigned long long answer;
};

void Input();
void MainAlgorithm();
void Output();
int MyLower_bound(int *arr, int n, int key);
bool comp(const ANSWER_DATA &a, const ANSWER_DATA &b);
int GetLongestSeqence();
long long GetTop(int index, int data);
ANSWER_DATA GetPathNum(int index, int data);

INDEX_DATA g_data[100000];
int g_Sequence[100000];
vector<ANSWER_DATA> g_vec[100000];

ofstream fOut, fOut2;
ifstream fIn;
int g_dataNum;
int g_longestS;

int main()
{
	fIn.open("LIS.inp");
	fOut.open("LIS.out");
	fOut2.open("test.out");
	int caseNum;

	fIn >> caseNum;

	for (int i = 0; i < caseNum; i++)
	{
		Input();
		MainAlgorithm();
		Output();
	}


	fIn.close();
	fOut.close();
	fOut2.close();

}

void Input()
{
	for (int i = 1; i <= g_longestS; i++)
		g_vec[i].clear();

	memset(g_data, 0, sizeof(g_data));
	memset(g_Sequence, 0, sizeof(g_Sequence));
	fIn >> g_dataNum;
	for (int i = 0; i < g_dataNum; i++)
	{
		fIn >> g_data[i].data;
	}

}
void MainAlgorithm()
{
	g_longestS = GetLongestSeqence();
}
void Output()
{
	long long answer = 0;
	for (int i = 0; i < g_vec[g_longestS].size(); i++)
		answer += g_vec[g_longestS][i].answer % 1000000007;

	fOut << g_longestS << " " << answer % 1000000007 << endl;

	//fOut << g_longestS << " " << g_vec[g_longestS].back().answer % 1000000007 << endl;
}


int MyLower_bound(int *arr, int n, int key) // 해당하는 배열의 위치를 리턴.
{
	int start = 0;
	int end = n;

	int mid = n;
	while (end - start > 0)
	{
		mid = (start + end) / 2;
		if (arr[mid] < key)
			start = mid + 1;
		else
			end = mid;
	}

	return end + 1;
}
int GetLongestSeqence()
{
	ANSWER_DATA temp;
	int indexCount = 1;
	g_Sequence[indexCount - 1] = g_data[0].data;
	g_data[0].index = indexCount;
	temp.answer = 1;
	temp.data = g_data[0].data;
	g_vec[indexCount].push_back(temp);

	for (int i = 1; i < g_dataNum; i++)
	{
		if (g_data[i].data > g_Sequence[indexCount - 1])
		{
			g_Sequence[indexCount] = g_data[i].data;
			indexCount++;
			g_data[i].index = indexCount;
			g_vec[indexCount].push_back(GetPathNum(indexCount, g_data[i].data));
		}
		else
		{
			int index = MyLower_bound(g_Sequence, indexCount, g_data[i].data);
			g_Sequence[index - 1] = g_data[i].data;
			g_data[i].index = index;
			g_vec[index].push_back(GetPathNum(index, g_data[i].data));
		}
	}
	return indexCount;
}

ANSWER_DATA GetPathNum(int index, int data)
{
	ANSWER_DATA answer;
	unsigned long long left = 0, top = 0, topBound = 0;
	int iterPosition;
	answer.data = data;
	answer.answer = 0;

	if (index == 1)
	{
		if (g_vec[1].size() == 0)
		{
			answer.answer = 1;
			answer.data = data;
			return answer;
		}
		else
		{
			answer.answer = 1;
			answer.data = data;
		}
	}
	else
	{
		/*ANSWER_DATA temp;
		temp.data = data;
		vector<ANSWER_DATA>::iterator iter;
		iter = lower_bound(g_vec[index - 1].begin(), g_vec[index - 1].end(), temp, comp);
		iterPosition = iter - g_vec[index - 1].begin();
		*/


		top = GetTop(index - 1, data);
		//if (iterPosition + 1< g_vec[index - 1].size() && iterPosition > 0)
		//{

		//	while (true)
		//	{
		//		if (iterPosition + 1 < g_vec[index - 1].size())
		//		{
		//			if (g_vec[index - 1][iterPosition].data == g_vec[index - 1][iterPosition + 1].data
		//				&& iterPosition + 1 < g_vec[index - 1].size())
		//				iterPosition++;
		//			else
		//				break;
		//		}
		//		else
		//			break;

		//	}
		//}

		//if (iterPosition == 0)
		//	top = g_vec[index - 1].back().answer;
		//else
		//{
		//	top = g_vec[index - 1].back().answer;
		//	topBound = g_vec[index - 1][iterPosition - 1].answer;

		//	if (top < topBound)
		//		return answer;
		//	top = top - topBound;

		//}
		//if (g_vec[index].size() == 0)
		//	left = 0;
		//else
		//	left = g_vec[index].back().answer;
		answer.answer = top;
	}

	return answer;


}
bool comp(const ANSWER_DATA &a, const ANSWER_DATA &b)
{
	return a.data > b.data;
}
long long GetTop(int index, int data)
{
	long long answer = 0;
	for (int i = 0; i < g_vec[index].size(); i++)
	{
		if (g_vec[index][i].data < data)
			answer += g_vec[index][i].answer % 1000000007;
	}
	return answer;
}