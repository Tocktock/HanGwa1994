#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>
#define DIRECTION0 0 // ��������
#define DIRECTIONLEFT 1
#define DIRECTIONRIGHT 2

using namespace std;

struct POINT
{
	double x;
	double y;
};

void Input();
void Output();
void MainAlgorithm();

char Direction(POINT pt1, POINT pt2, POINT polygonPt);
double GetArea(POINT pt1, POINT pt2, POINT pt3);
bool AngleComapre(POINT a, POINT b); // ������ �Ͼ�� �������� true�� ��ȯ�ؾ��� <�� ��������.

void MakeConvexHull(vector<POINT> vecPt, vector<POINT> &vecConvexHull);
void MakeStand(vector<POINT> &vecMergedPt);

vector<POINT> g_input; //��ǲ �� ����
vector<POINT> g_convexHull; //��ǲ �� ����

POINT g_minXPt;
ifstream fIn;
ofstream fOut;

int main()
{
	fIn.open("hull.inp");
	fOut.open("hull.out");
	Input();
	MainAlgorithm();
	Output();

	fIn.close();
	fOut.close();
}

void Input()
{
	g_minXPt.x = 30005;
	int inputNum;
	fIn >> inputNum;
	POINT pt;
	for (int i = 0; i < inputNum; i++)
	{
		fIn >> pt.x;
		fIn >> pt.y;
		g_input.push_back(pt);

		if (g_input[i].x < g_minXPt.x)
		{
			g_minXPt.x = g_input[i].x;
			g_minXPt.y = g_input[i].y;
		}
	}

}
void Output()
{
	fOut << g_convexHull.size() << endl;
	for (int i = 0; i < g_convexHull.size(); i++)
		fOut << g_convexHull[i].x << " " << g_convexHull[i].y << endl;
}


void MainAlgorithm()
{
	MakeConvexHull(g_input, g_convexHull);
}


char Direction(POINT pt1, POINT pt2, POINT polygonPt) // true ��ȸ��, false ��ȸ��
{
	double area = GetArea(pt1, pt2, polygonPt);

	if (area == 0)
		return DIRECTION0;
	else if (area < 0)
		return DIRECTIONRIGHT; //�ð����
	else if (area > 0)
		return DIRECTIONLEFT; //�ݽð����

	return DIRECTION0;
}

double GetArea(POINT pt1, POINT pt2, POINT pt3) //���밪 �����Ѱ���.
{
	double area;
	double a = (pt1.x*pt2.y + pt2.x*pt3.y + pt3.x*pt1.y);
	double b = (pt1.x*pt3.y + pt3.x*pt2.y + pt2.x*pt1.y);
	area = (pt1.x*pt2.y + pt2.x*pt3.y + pt3.x*pt1.y)
		- (pt1.x*pt3.y + pt3.x*pt2.y + pt2.x*pt1.y);
	area /= 2;
	return area;
}
void MakeConvexHull(vector<POINT> vecOrigin, vector<POINT> &vecConvexHull)
{
	//standPt�� x�� ���� ���� ��ǥ�� ��.
	MakeStand(vecOrigin);

	sort(vecOrigin.begin() + 1, vecOrigin.end(), AngleComapre);
	vecConvexHull.push_back(vecOrigin[0]);
	vecConvexHull.push_back(vecOrigin[1]);
	int vecOriginCount = vecOrigin.size();

	for (int i = 0; i < vecOriginCount - 1; i++) //��ȸ���� �Ǵ� ��θ� ã��. ���� ī���ʹ� ���� �ؾ��� ��.
	{
		int vecCount = vecConvexHull.size();
		while (Direction(vecConvexHull[(vecCount - 2) % vecOriginCount], vecConvexHull[(vecCount - 1) % vecOriginCount],
			vecOrigin[(i + 2) % vecOriginCount]) != DIRECTIONLEFT)
		{
			vecConvexHull.pop_back();
			vecCount--;
			if (vecCount == 1)
				break;
		}
		if (i == vecOrigin.size() - 2)
			break;
		vecConvexHull.push_back(vecOrigin[(i + 2) % vecOriginCount]);
	}
}

void MakeStand(vector<POINT> &vecMergedPt)
{
	//������ ����� ���� ���� x��ǥ�� �ε����� ������ �����Ѵ�
	POINT minXPt = vecMergedPt[0];

	int index = 0;
	for (int i = 0; i < vecMergedPt.size(); i++)
	{
		if (minXPt.x > vecMergedPt[i].x)
		{
			index = i;
			minXPt = vecMergedPt[i];
		}
		if (minXPt.x == vecMergedPt[i].x)
		{
			if (minXPt.y > vecMergedPt[i].y)
			{
				index = i;
				minXPt = vecMergedPt[i];
			}
		}
	}

	POINT temp; // ù��° ���� ��ȯ
	temp = vecMergedPt[0];
	vecMergedPt[0] = vecMergedPt[index];
	vecMergedPt[index] = temp;
	g_minXPt = vecMergedPt[0];
}
bool AngleComapre(POINT a, POINT b) // ������ �Ͼ�� �������� true�� ��ȯ�ؾ��� <�� ��������.
{

	if (a.x - g_minXPt.x == 0)
		return false;
	if (b.x - g_minXPt.x == 0)
		return true;

	double aGraident = (a.y - g_minXPt.y) / (a.x - g_minXPt.x);
	double bGraident = (b.y - g_minXPt.y) / (b.x - g_minXPt.x);

	if (aGraident == bGraident)
		return a.x < b.x;

	return aGraident < bGraident; //���Ⱑ ������ ����
}