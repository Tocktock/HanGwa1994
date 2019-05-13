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
bool IsEqualPoint(POINT a, POINT b);

void MakeConvexHull(vector<POINT> vecPt, vector<POINT> &vecConvexHull);
void MakeStand(vector<POINT> &vecMergedPt);
void GetCrossPoint(int &standIndex, int &objectIndex, vector<POINT> standConvexHull, vector<POINT> objectConvexHull, char type);
void MakePolygon(vector<POINT> &polygon, vector<POINT> polygonP, vector<POINT> polygonQ,
	POINT pa, POINT pb, POINT qs, POINT qt);
double GetMultilateralArea(vector<POINT> vectorPt);

vector<POINT> g_polygonPPt, g_polygonQPt; //��ǲ �� ����
vector<POINT> g_convexHullP, g_convexHullQ;
POINT g_minXPt;
double g_answer;

ifstream fIn;
ofstream fOut;

int main()
{
	fIn.open("tangent2.inp");
	fOut.open("tangent2.out");
	int testCase;
	fIn >> testCase;

	for (int i = 0; i < testCase; i++)
	{
		Input();
		if (i == 15)
		{
			fOut << g_polygonPPt.size() << endl;
			for (int i = 0; i < g_polygonPPt.size(); i++)
				fOut << g_polygonPPt[i].x << " " << g_polygonPPt[i].y << endl;
			fOut << g_polygonQPt.size() << endl;
			for (int i = 0; i < g_polygonQPt.size(); i++)
				fOut << g_polygonQPt[i].x << " " << g_polygonQPt[i].y << endl;
		}
		MainAlgorithm();

		Output();
	}

	fIn.close();
	fOut.close();
}

void Input()
{
	int inputNum;
	fIn >> inputNum;
	if (fIn.eof())
		return;

	g_polygonPPt.clear();
	g_polygonQPt.clear();
	g_convexHullP.clear();
	g_convexHullQ.clear();
	POINT pt;
	for (int i = 0; i < inputNum; i++)
	{
		fIn >> pt.x;
		fIn >> pt.y;
		g_polygonPPt.push_back(pt);
	}
	fIn >> inputNum;
	for (int i = 0; i < inputNum; i++)
	{
		fIn >> pt.x;
		fIn >> pt.y;
		g_polygonQPt.push_back(pt);
	}
}
void Output()
{
	fOut.precision(1);
	fOut << fixed << g_answer << endl;
}


void MainAlgorithm()
{
	g_minXPt.x = 50005;
	MakeConvexHull(g_polygonPPt, g_convexHullP);
	g_minXPt.x = 50005;
	MakeConvexHull(g_polygonQPt, g_convexHullQ);
	POINT pa, pb, qs, qt;
	pa = g_convexHullP[0];
	pb = g_convexHullP[0];
	qs = g_convexHullQ[0];
	qt = g_convexHullQ[0];
	int pIndex, qIndex;
	pIndex = 0;
	qIndex = 0;
	while (true)
	{
		GetCrossPoint(pIndex, qIndex, g_convexHullP, g_convexHullQ, DIRECTIONLEFT);//P���� pb�� Q���� qt�� ���� ã�´�.
		GetCrossPoint(qIndex, pIndex, g_convexHullQ, g_convexHullP, DIRECTIONLEFT);//GetCross �Լ����� ����� ��ǥ�� �����Ѵ�.

		if (IsEqualPoint(pb, g_convexHullP[pIndex]) && IsEqualPoint(qt, g_convexHullQ[qIndex]))
			break;//���� �˻��ѰŶ� ������ �ݺ��� ������.
		else
		{
			pb = g_convexHullP[pIndex];
			qt = g_convexHullQ[qIndex];
		}
	}
	pIndex = 0;
	qIndex = 0;
	while (true)
	{
		GetCrossPoint(pIndex, qIndex, g_convexHullP, g_convexHullQ, DIRECTIONRIGHT);//P���� pa�� Q���� qs�� ���� ã�´�.
		GetCrossPoint(qIndex, pIndex, g_convexHullQ, g_convexHullP, DIRECTIONRIGHT);//GetCross �Լ����� ����� ��ǥ�� �����Ѵ�.
		if (IsEqualPoint(pa, g_convexHullP[pIndex]) && IsEqualPoint(qs, g_convexHullQ[qIndex]))
			break;
		else
		{
			pa = g_convexHullP[pIndex];
			qs = g_convexHullQ[qIndex];
		}
	}
	vector<POINT> answerPolygon;
	MakePolygon(answerPolygon, g_polygonPPt, g_polygonQPt, pa, pb, qs, qt);
	g_answer = GetMultilateralArea(answerPolygon);
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
	int count = vecMergedPt.size();
	for (int i = 0; i < count; i++)
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
		a.x < b.x;

	return aGraident < bGraident; //���Ⱑ ������ ����
}
void GetCrossPoint(int &standIndex, int &objectIndex, vector<POINT> standConvexHull, vector<POINT> objectConvexHull, char type)
{
	int count = objectConvexHull.size();
	vector<POINT>temp;
	char dir;
	for (int i = 0; i < count - 1; i++)
	{
		dir = Direction(standConvexHull[standIndex], objectConvexHull[objectIndex], objectConvexHull[(objectIndex + 1 + i) % count]);
		if (dir != type && dir == DIRECTION0)
		{
			objectIndex = (objectIndex + 1 + i) % count;
			return;
		}
	}

}
void MakePolygon(vector<POINT> &polygon, vector<POINT> polygonP, vector<POINT> polygonQ,
	POINT pa, POINT pb, POINT qs, POINT qt)
{

	int index = 0;
	polygon.push_back(pa); //pa���� ���� qt - qs - pb ������ ã������.
	polygon.push_back(qt);
	int count;
	count = polygonQ.size();
	for (int i = 0; i < count; i++)
	{
		if (IsEqualPoint(qt, polygonQ[i]))
		{
			index = i; // qt�� index�� ã��.
			break;
		}
	}
	count = polygonQ.size();
	for (int i = 1; i < count; i++)
	{
		polygon.push_back(polygonQ[(count + index - i) % count]); //���������� qs�� ��.
		if (IsEqualPoint(qs, polygonQ[(count + index - i) % count])) //qs�� ������ �����.
			break;
	}
	index = 0;
	count = polygonP.size();
	for (int i = 0; i < count; i++)
	{
		if (IsEqualPoint(pb, polygonP[i]))
		{
			index = i; // pa�� index�� ã��.
			break;
		}
	}
	for (int i = 0; i < count; i++)
	{
		polygon.push_back(polygonP[(count + index - i) % count]); //���������� qs�� ��.
		if (IsEqualPoint(pa, polygonP[(count + index - i) % count])) //qb�� ������ �����.
			break;
	}
}
double GetMultilateralArea(vector<POINT> vectorPt)
{
	double area = 0;
	POINT standPt = vectorPt[0];
	int count = vectorPt.size();
	for (int i = 0; i < count; i++)
		area += GetArea(standPt, vectorPt[i], vectorPt[(i + 1) % count]);

	return area;
}
bool IsEqualPoint(POINT a, POINT b)
{
	if (a.x == b.x && a.y == b.y)
		return true;

	return false;
}