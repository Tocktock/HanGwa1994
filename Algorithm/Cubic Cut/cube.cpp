
#include<iostream>
#include<fstream>
using namespace std;

class test
{
public:
	int asd;

	test()
	{
		asd = 0;
	}
	int getTest()
	{
		return asd;
	}

	void setTest()
	{
		asd = 1;
	}
	void reset()
	{
		asd = 0;
	}
};

void Input();
void Output();
void MainAlgolrithm(int input[3]);
void Setting(int arr[]);

int Check(int input[]);
int g_cubeInfo[256][256][256];
int Min(int arr[], int length);
void Set();
void StoreInfo(int input[], int piece);

ifstream fIn;
ofstream fOut;
int g_answer;
test a;
int main()
{
	fIn.open("cube.inp");
	fOut.open("cube.out");

	int caseNum;
	int input[3] = { 3,2,1 };
	Setting(input);
	fIn >> caseNum;

	g_cubeInfo[1][1][1] = 1;
	Set();
	for (int i = 0; i < caseNum; i++)
	{
		fIn >> input[0];
		fIn >> input[1];
		fIn >> input[2];
		Setting(input);

		MainAlgolrithm(input);
		Output();
	}

	fIn.close();
	fOut.close();
	return 0;
}

void Input()
{

}

void Output()
{
	a.setTest();
	if (a.getTest() == 1)
		fOut << g_answer << endl;
	a.reset();
}

void MainAlgolrithm(int input[3])
{
	g_answer = Check(input);
}

int Check(int input[])
{
	int szInput[3] = { input[0],input[1],input[2] };
	Setting(szInput);
	if (g_cubeInfo[szInput[0]][szInput[1]][szInput[2]] != -1)
		return g_cubeInfo[szInput[0]][szInput[1]][szInput[2]];
	int kalJil[200];

	for (int i = 1; i < szInput[2]; i++) //가로로 칼질.
	{
		int left[3] = { szInput[0],szInput[1],i };
		int right[3] = { szInput[0],szInput[1],szInput[2] - i };
		Setting(left);
		Setting(right);
		int leftAnswer = Check(left);
		int rightAnswer = Check(right);
		kalJil[i - 1] = leftAnswer + rightAnswer;
	}

	int min = kalJil[0];
	min = Min(kalJil, szInput[2] - 1);
	StoreInfo(input, min);
	return min;
}

void Setting(int arr[]) //젤큰거 마지막으로 밀어넣음
{
	int temp;

	if (arr[0] > arr[1])
	{
		temp = arr[0];
		arr[0] = arr[1];
		arr[1] = temp;
	}

	if (arr[1] > arr[2])
	{
		temp = arr[1];
		arr[1] = arr[2];
		arr[2] = temp;
	}

	if (arr[0] > arr[1])
	{
		temp = arr[0];
		arr[0] = arr[1];
		arr[1] = temp;
	}
}

int Min(int arr[], int length)
{
	int min = arr[0];
	for (int i = 0; i < length; i++)
	{
		if (min > arr[i])
			min = arr[i];
	}
	return min;
}

void Set()
{
	memset(g_cubeInfo, -1, sizeof(g_cubeInfo));
	for (int i = 0; i < 200; i++)
	{
		g_cubeInfo[1][1][i] = i;
	}
	for (int i = 1; i < 200; i++)
	{
		g_cubeInfo[i][i][i] = 1;
	}
}
void StoreInfo(int input[], int piece) //6가지 경우?
{
	g_cubeInfo[input[0]][input[1]][input[2]] = piece;
	g_cubeInfo[input[0]][input[2]][input[1]] = piece;
	g_cubeInfo[input[1]][input[0]][input[2]] = piece;
	g_cubeInfo[input[1]][input[2]][input[0]] = piece;
	g_cubeInfo[input[2]][input[0]][input[1]] = piece;
	g_cubeInfo[input[2]][input[1]][input[0]] = piece;
}