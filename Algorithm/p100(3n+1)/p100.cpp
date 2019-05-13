#include <fstream>

using namespace std;

ifstream fIn;
ofstream fOut;

int g_first;
int g_second;
int g_answer;

void MainAlgorithm();
bool Input();
void Output();
void swap(int &a, int &b);

int main()
{
	fIn.open("p100.inp");
	fOut.open("p100.out");
	
	while (Input())
	{
		MainAlgorithm();
		Output();
	}
	fIn.close();
	fOut.close();
}


void MainAlgorithm()
{
	fOut << g_first << " " << g_second << " ";
	swap(g_first, g_second);
	
	int answer;
	int max = 1;
	int count = 1;
	for (int i = g_first; i <= g_second; i++)
	{
		answer = i;
		count = 1;
		while (answer != 1)
		{
			if (answer % 2 == 0)
				answer /= 2;
			else
				answer = answer * 3 + 1;

			count++;
		}
		if (max < count)
			max = count;
	}
	g_answer = max;
}
bool Input()
{
	fIn >> g_first;
	fIn >> g_second;

	if (g_first == -1 && g_second == -1)
		return false;
	else
		return true;

}
void Output()
{
	fOut<< g_answer<<endl;
	g_first = -1;
	g_second = -1;
}
void swap(int &a, int &b)
{
	int temp;
	if (a > b)
	{
		temp = a;
		a = b;
		b = temp;
	}

}
