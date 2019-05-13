#include<fstream>
#include<algorithm>
#include<string.h>
#include<map>
#include<vector>

using namespace std;

void MainAlgorithm();
void Input();
void Output();
int GetWire(int a,int b);

class ExtendedEuclidTool
{
private:
	int r, s, t;
	int r1, r2;
	int s1, s2;
	int t1, t2;
	int gcd;
public:
	void Init(int a, int b)
	{
		r1 = a;
		r2 = b;
		s1 = 1;
		s2 = 0;
		t1 = 0;
		t2 = 1;
		Calculate();
	}
	void Calculate()
	{
		int q; // 몫
		while (r2 > 0)
		{
			q = r1 / r2;
			r = r1 - q * r2;
			r1 = r2;
			r2 = r;
			s = s1 - q * s2;
			s1 = s2;
			s2 = s;
			t = t1 - q * t2;
			t1 = t2;
			t2 = t;
		}
		gcd = r1;
		s = s1;
		t = t1;
	}
	inline int GetS() { return s; } // r1 * s + r2 * t = gcd가 됨.
	inline int GetT() { return t; }
	inline int GetGcd() { return r1; } // r1 * s + r2 * t = gcd가 됨.

};

ifstream fIn;
ofstream fOut;

int g_dp[200000];// 만드는데 줄 얼마나 필요한지.
int g_dpCount = 0;
int g_testCase;
int main()
{
	fIn.open("ps26.inp");
	fOut.open("ps26.out");

	MainAlgorithm();
	Input();
	Output();

	fIn.close();
	fOut.close();
	return 0;
}


void MainAlgorithm()
{
	// a,b가 서로소 , a,b 중 하나는 even이여야한다.
	int max = 0;
	ExtendedEuclidTool tool;
	int temp = 0;
	for (int i = 1; i < 1000; i++)
	{

		for (int j = i + 1; j < 1000; j++)
		{
			if ((i % 2 == 1 && j % 2 == 1)
				|| i % 2 == 0 && j % 2 == 0)
				continue;

			tool.Init(i,j);
			if (tool.GetGcd() != 1)
				continue;

			int wire = GetWire(i,j);
			if (wire > 1000000 || wire == -1)
				break;
			g_dp[g_dpCount] = wire;
			g_dpCount++;
		}
	}
	sort(g_dp, g_dp + g_dpCount);
}

void Input()
{
	fIn >> g_testCase;
	int wire;
	int sum;
	for (int i = 0; i < g_testCase; i++)
	{
		sum = 0;
		fIn >> wire;
		for (int j = 0; j <= g_dpCount; j++)
		{
			sum += g_dp[j];
			if (sum == wire)
			{
				fOut << j + 1 << endl;
				break;
			}
			else if (sum > wire)
			{
				fOut << j << endl;
				break;
			}
		}
	}
}

void Output()
{

	
}
int GetWire(int a, int b)
{
	int width =  a * b * 2;
	int height = (b * b) - (a*a);

	return (width + height) * 2;
}