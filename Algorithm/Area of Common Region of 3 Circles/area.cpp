#include <fstream>
#include <cmath>

#define X_SQUARE 0
#define X_ 1
#define Y_SQUARE 2
#define Y_ 3
#define CONSTANT 4

using namespace std;

struct POINT
{
	double x;
	double y;
};

class EQUATION
{
public:
	POINT m_coordinate;
	double m_r;
	double m_coeffi[5]; // �����
	double m_xAnswer[2]; //
	double m_yAnswer[2];
};


void MainAlgorithm();
void Input();
void Output();
void MakeEquation(EQUATION &circle);
void MakeEquation(EQUATION &circle);
EQUATION GetCommonLine(EQUATION circle1, EQUATION circle2);
void Reset();
bool IsInside(EQUATION circle, POINT pt);

POINT FindXY(EQUATION circle, EQUATION otherCircle, EQUATION line);
void GetX(EQUATION circle, EQUATION line, POINT &first, POINT &second);
void Formula(EQUATION eqa, double &first, double &second, int type); //���� �������� �� ã��
double GetArea();

ifstream fIn;
ofstream fOut;
EQUATION g_circleInfo[3];
double g_answer;
int g_testCase;
POINT g_threePoint[3];

int main()
{
	fIn.open("area.inp");
	fOut.open("area.out");

	fIn >> g_testCase;
	for (int i = 0; i < g_testCase; i++)
	{
		Reset();
		Input();
		MainAlgorithm();
		Output();
	}

	fIn.close();
	fOut.close();
}


void MainAlgorithm()
{

	for (int i = 0; i < 3; i++)
	{
		MakeEquation(g_circleInfo[i]);
	}
	EQUATION equalLine;
	equalLine = GetCommonLine(g_circleInfo[0], g_circleInfo[1]);
	g_threePoint[0] = FindXY(g_circleInfo[0], g_circleInfo[2], equalLine);

	equalLine = GetCommonLine(g_circleInfo[1], g_circleInfo[2]);
	g_threePoint[1] = FindXY(g_circleInfo[1], g_circleInfo[0], equalLine);

	equalLine = GetCommonLine(g_circleInfo[0], g_circleInfo[2]);
	g_threePoint[2] = FindXY(g_circleInfo[0], g_circleInfo[1], equalLine);

	g_answer = GetArea();

}
void Input()
{
	for (int i = 0; i < 3; i++)
	{
		fIn >> g_circleInfo[i].m_coordinate.x;
		fIn >> g_circleInfo[i].m_coordinate.y;
		fIn >> g_circleInfo[i].m_r;
	}
}
void Output()
{
	fOut.precision(2);

	fOut << fixed << g_answer << endl;
}


void MakeEquation(EQUATION &circle)
{
	circle.m_coeffi[X_SQUARE] = 1;
	circle.m_coeffi[Y_SQUARE] = 1;
	circle.m_coeffi[X_] = circle.m_coordinate.x * -2;
	circle.m_coeffi[Y_] = circle.m_coordinate.y * -2;
	circle.m_coeffi[CONSTANT] = circle.m_coordinate.x * circle.m_coordinate.x + circle.m_coordinate.y * circle.m_coordinate.y
		- circle.m_r*circle.m_r;

}


EQUATION GetCommonLine(EQUATION circle1, EQUATION circle2)
{
	EQUATION line;
	line.m_coeffi[X_SQUARE] = 0;
	line.m_coeffi[Y_SQUARE] = 0;
	line.m_coeffi[X_] = circle1.m_coeffi[X_] - circle2.m_coeffi[X_];
	line.m_coeffi[Y_] = circle1.m_coeffi[Y_] - circle2.m_coeffi[Y_];
	line.m_coeffi[CONSTANT] = circle1.m_coeffi[CONSTANT] - circle2.m_coeffi[CONSTANT];
	return line; // x+y+c = 0 ���·� ����.
}
void Reset()
{
	for (int i = 0; i < 3; i++)
	{
		g_circleInfo[i].m_coordinate.x = 0;
		g_circleInfo[i].m_coordinate.y = 0;
		g_circleInfo[i].m_r = 0;
		for (int j = 0; j < 5; j++)
		{
			g_circleInfo[i].m_coeffi[j] = 0;
		}

		g_circleInfo[i].m_xAnswer[0] = 0; //
		g_circleInfo[i].m_xAnswer[1] = 0; //
		g_circleInfo[i].m_yAnswer[0] = 0; //
		g_circleInfo[i].m_yAnswer[1] = 0; //
	}
}
bool IsInside(EQUATION circle, POINT pt)
{
	//��Ÿ��� ����
	double a = (pt.x - circle.m_coordinate.x) * (pt.x - circle.m_coordinate.x);
	double b = (pt.y - circle.m_coordinate.y) * (pt.y - circle.m_coordinate.y);
	double c = sqrt(a + b);
	if (c <= circle.m_r)
		return true;

	return false;
}
POINT FindXY(EQUATION circle, EQUATION otherCircle, EQUATION line)
{

	//line�� ���� ax+by+c = 0�� �����̴�
	EQUATION xEqa; // y = ax+c ���·� ��Ÿ���� y���� �����ϰ� ǥ���ϸ� ��.
	POINT first, second;

	if (line.m_coeffi[Y_] == 0)// y�� ����� 0�̸� x = c �������� ��Ÿ���� x���� �ٷ� ����
	{
		first.x = -line.m_coeffi[CONSTANT] / line.m_coeffi[X_];
		second.x = first.x;
		EQUATION temp = circle;
		temp.m_coeffi[CONSTANT] += temp.m_coeffi[X_] * first.x + first.x*first.x;
		Formula(temp, first.y, second.y, Y_); // ���ҷ��°� y
	}

	else if (line.m_coeffi[X_] == 0) // X����� 0�̸� y = c �������� ��Ÿ���� y�� �ٷ� ����
	{
		first.y = -line.m_coeffi[CONSTANT] / line.m_coeffi[Y_];
		second.y = first.y;
		EQUATION temp = circle;
		temp.m_coeffi[CONSTANT] += temp.m_coeffi[Y_] * first.y + first.y*first.y;
		Formula(temp, first.x, second.x, X_); // ���ҷ��°� x
	}
	else// y, x�� ����� 0�� �ƴҶ�
	{
		xEqa.m_coeffi[CONSTANT] = -line.m_coeffi[CONSTANT] / line.m_coeffi[Y_];
		xEqa.m_coeffi[X_] = -line.m_coeffi[X_] / line.m_coeffi[Y_];
		GetX(circle, xEqa, first, second); //����x�� xEqa�� �����ؼ� y �� ����.
		first.y = xEqa.m_coeffi[X_] * first.x + xEqa.m_coeffi[CONSTANT];
		second.y = xEqa.m_coeffi[X_] * second.x + xEqa.m_coeffi[CONSTANT];
	}
	if (IsInside(otherCircle, first))
		return first;
	else if (IsInside(otherCircle, second))
		return second;

	return first;
}
void GetX(EQUATION circle, EQUATION line, POINT &first, POINT &second)
{
	// y = ax + c�� ���·� ���� ���� �����Ŀ� ������
	EQUATION lineResult;
	lineResult.m_coeffi[X_SQUARE] = line.m_coeffi[X_] * line.m_coeffi[X_];
	lineResult.m_coeffi[X_] = 2 * line.m_coeffi[CONSTANT] * line.m_coeffi[X_];
	lineResult.m_coeffi[X_] += line.m_coeffi[X_] * circle.m_coeffi[Y_];
	lineResult.m_coeffi[CONSTANT] = line.m_coeffi[CONSTANT] * line.m_coeffi[CONSTANT];
	lineResult.m_coeffi[CONSTANT] += line.m_coeffi[CONSTANT] * circle.m_coeffi[Y_];
	lineResult.m_coeffi[Y_] = 0;
	lineResult.m_coeffi[Y_SQUARE] = 0;
	for (int i = 0; i < 5; i++)
		circle.m_coeffi[i] += lineResult.m_coeffi[i];
	Formula(circle, first.x, second.x, X_); // x�� �Ӱ��� ����.
}
void Formula(EQUATION eqa, double &first, double &second, int type) //���� �������� �� ã��
{
	double inroot = sqrt(eqa.m_coeffi[type] * eqa.m_coeffi[type] - 4 * eqa.m_coeffi[type - 1] * eqa.m_coeffi[CONSTANT]);
	double up = -eqa.m_coeffi[type];
	double down = 2 * eqa.m_coeffi[type - 1];

	first = (up + inroot) / down;
	if (inroot == 0)
		second = first;
	else
		second = (up - inroot) / down;
}
double GetArea()
{
	double area;
	area = abs((g_threePoint[0].x*g_threePoint[1].y + g_threePoint[1].x*g_threePoint[2].y + g_threePoint[2].x*g_threePoint[0].y)
		- (g_threePoint[0].x*g_threePoint[2].y + g_threePoint[2].x*g_threePoint[1].y + g_threePoint[1].x*g_threePoint[0].y));
	area /= 2;
	return area;
}