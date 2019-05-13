#include<fstream>
#include<algorithm>

using namespace std;

class Eqauation
{
public:
	long long a, b, c; //ax = by + c 꼴 
	//ax = c (mod b) 
};

class ExtendedEuclidTool
{
private:
	long long r, s, t;
	long long r1, r2;
	long long s1, s2;
	long long t1, t2;
	long long gcd;
public:
	void Init(long long a, long long b)
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
		long long q; // 몫
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
	inline long long GetS() { return s; } // r1 * s + r2 * t = gcd가 됨.
	inline long long GetT() { return t; }
	inline long long GetGCD() { return gcd; }

};

void MainAlgorithm();
void Input();
void Output();
bool IsPossible(int gcd_ab,int c);
long long GetModular(long long a, long long modular);
Eqauation MergeEqa(Eqauation eqa1, Eqauation eqa2);
Eqauation GetAnswerEqa(Eqauation eqa1, Eqauation eqa2);
Eqauation GetEuclideAnswer(long long gcd, int a,int b);

ifstream fIn;
ofstream fOut;

Eqauation g_inputEqa[52];
long long g_answer;
int g_inputCaseNum;

int main()
{
	fIn.open("reminder.inp");
	fOut.open("reminder.out");

	int testCase;

	fIn >> testCase;

	for (int i = 0; i < testCase; i++)
	{
		Input();
		MainAlgorithm();
		Output();
	}

	fIn.close();
	fOut.close();
	return 0;
}


void MainAlgorithm()
{
	int gcd;
	ExtendedEuclidTool tool;
	Eqauation answer,temp;
	answer = g_inputEqa[0];

	for (int i = 1; i < g_inputCaseNum; i++)
	{
		tool.Init(answer.a, answer.b); // 유클리드 사용해서 값 얻어냄.
		if (!IsPossible(tool.GetGCD(), answer.c)) // 안되면 안되는거지 뭥
		{
			g_answer = -1;
			return;
		}
		temp = MergeEqa(answer, g_inputEqa[i]);
		if (temp.c == -1)
		{
			g_answer = -1;
			return;
		}
		answer = GetAnswerEqa(temp, answer);
	}
	g_answer = answer.c;
}

void Input()
{
	fIn >> g_inputCaseNum;

	for (int i = 0; i < g_inputCaseNum; i++)
	{
		fIn >> g_inputEqa[i].c >> g_inputEqa[i].b;
		g_inputEqa[i].a = 1;
	}
}

void Output()
{
	fOut << g_answer << endl;
}

bool IsPossible(int gcd_ab, int c)
{
	if (c %gcd_ab != 0)
		return false;

	return true;
}
long long GetModular(long long a, long long modular)
{
	if (a > 0)
		return a % modular;
	else
	{
		long long p = a / modular;
		return (-(p - 1) * modular) + a;
	}
}
Eqauation MergeEqa(Eqauation eqa1, Eqauation eqa2)
{
	ExtendedEuclidTool tool;
	Eqauation temp;
	temp.a = eqa1.b;
	temp.b = eqa2.b;
	temp.c = GetModular(eqa2.c - eqa1.c, temp.b);
	tool.Init(temp.a, temp.b);

	if (temp.c % tool.GetGCD() != 0)
	{
		temp.c = -1;
		return temp;
	}
	temp.a /= tool.GetGCD();
	temp.b /= tool.GetGCD();
	temp.c /= tool.GetGCD();
	return temp;
}
Eqauation GetAnswerEqa(Eqauation eqa1, Eqauation eqa2)
{
	ExtendedEuclidTool tool;
	Eqauation eqa1Answer, eqa2Answer;
	tool.Init(eqa1.a, eqa1.b);
	eqa1Answer.a = 1;
	eqa1Answer.b = eqa1.b;
	eqa1Answer.c = GetModular(tool.GetS() * (eqa1.c / tool.GetGCD()), eqa1.b);

	eqa2Answer.a = 1;
	eqa2Answer.b = eqa1Answer.b * eqa2.b;
	eqa2Answer.c = GetModular(eqa1Answer.c * eqa2.b + eqa2.c, eqa2Answer.b);

	return eqa2Answer;

}