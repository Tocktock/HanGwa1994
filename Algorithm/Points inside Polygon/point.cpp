#include <fstream>
#include <cmath>

#define DIRECTION0 0 // 일직선상
#define	DIRECTIONLEFT 1
#define DIRECTIONRIGHT 2

using namespace std;

enum COEFFI // 계수
{
	COEFFI_X,
	COEFFI_Y,
	COEFFI_CONS,
	COEFFI_END
};
struct POINT
{
	double x;
	double y;
};

class EQUATION
{
public:
	double m_coeffi[COEFFI_END];
};
class RECTANGLE
{
public:
	double left;
	double right;
	double top;
	double bottom;
};
void MainAlgorithm();
void Input();
void Output();
bool IsIntersecting(POINT pt1, POINT pt2, POINT polygonPt1, POINT polygonPt2);
bool IsOnLine(POINT pt, POINT polygonPt1, POINT polygonPt2);
char Direction(POINT pt1, POINT pt2, POINT polygonPt);
bool IsBoxIntersecting(POINT pt1, POINT pt2, POINT polygonPt1, POINT polygonPt2);
double GetArea(POINT pt1, POINT pt2, POINT pt3);
RECTANGLE MakeRectangle(POINT pt1, POINT pt2);
bool CollisionCheck(RECTANGLE rec1, RECTANGLE rec2);
bool CollisionCheck(RECTANGLE rec, POINT pt);
EQUATION g_lineEqa[1000000]; // 직선의 방정식

POINT g_polygonPoint[100000]; //인풋 값 정보
POINT g_testPoint[100000]; // 테스트할 정보

int g_polygonPointNum;
int g_testPointNum;
int g_minX = 10001;
int g_maxX = -10001;
int g_minY = 10001;
int g_maxY = -10001;
bool g_answer;
ifstream fIn;
ofstream fOut;

int main()
{
	fIn.open("point.inp");
	fOut.open("point.out");

	Input();
	MainAlgorithm();

	fIn.close();
	fOut.close();
}


void MainAlgorithm()
{
	POINT standPt; // maxX,maxY 보다 1씩 큰애들 먼저하고 그다음 maxX, minY로 선택.
	int intersectCount = 0;
	int answerResult = 0;
	RECTANGLE rec;
	bool flag = false;
	for (int i = 0; i < g_testPointNum; i++)
	{
		intersectCount = 0;
		answerResult = 0;
		standPt.x = g_maxX + 1;
		standPt.y = g_maxY + 1;
		flag = false;
		for (int j = 0; j < g_polygonPointNum;j++)
		{
			if (IsOnLine(g_testPoint[i], g_polygonPoint[j], g_polygonPoint[(j + 1) % g_polygonPointNum]))
			{
				flag = true;
				g_answer = true;
				Output();
				break;
			}
		}
		if (flag == true)
			continue;
		for (int j = 0; j < 10; j++) // k번 실행
		{
			for (int k = 0; k < g_polygonPointNum; k++)
			{
				if (IsIntersecting(standPt, g_testPoint[i], g_polygonPoint[k], g_polygonPoint[(k + 1)% g_polygonPointNum]))
					intersectCount++;
			}
			if (intersectCount % 2 == 1)
				answerResult++;
			intersectCount = 0;
			standPt.x++;
		}
		if (answerResult >= 5)
			g_answer = true;
		else
			g_answer = false;
		Output();

	}
}
void Input()
{
	fIn >> g_polygonPointNum;
	for (int i = 0; i < g_polygonPointNum; i++)
	{
		fIn >> g_polygonPoint[i].x;
		fIn >> g_polygonPoint[i].y;

		if (g_polygonPoint[i].x < g_minX)
			g_minX = g_polygonPoint[i].x;
		if (g_polygonPoint[i].x > g_maxX)
			g_maxX = g_polygonPoint[i].x;

		if (g_polygonPoint[i].y < g_minY)
			g_minY = g_polygonPoint[i].y;
		if (g_polygonPoint[i].y > g_maxY)
			g_maxY = g_polygonPoint[i].y;
	}

	fIn >> g_testPointNum;

	for (int i = 0; i < g_testPointNum; i++)
	{
		fIn >> g_testPoint[i].x;
		fIn >> g_testPoint[i].y;
	}
}
void Output()
{
	if (g_answer == true)
		fOut << 1 << endl;
	else
		fOut << 0 << endl;
}

char Direction(POINT pt1, POINT pt2, POINT polygonPt) // true 좌회전, false 우회전
{
	double area = GetArea(pt1, pt2, polygonPt);

	if (area == 0)
		return DIRECTION0;
	else if (area < 0)
		return DIRECTIONRIGHT; //시계방향
	else if (area > 0)
		return DIRECTIONLEFT; //반시계방향

	return DIRECTION0;
}

bool IsBoxIntersecting(POINT pt1, POINT pt2, POINT polygonPt1, POINT polygonPt2) //true 교차, false 비교차 직선상일때 체크 
{
	RECTANGLE test = MakeRectangle(pt1, pt2);
	RECTANGLE polygon = MakeRectangle(polygonPt1, polygonPt2);

	if (CollisionCheck(polygon, test))
		return true;

	return false;
}
bool IsIntersecting(POINT pt1, POINT pt2, POINT polygonPt1, POINT polygonPt2) // 교차하는지 체크하는 함수. 
{
	//direction이 다르다면 교차 가능성이 있고 intersect 체크 해야함.
	char direction1 = Direction(pt1, pt2, polygonPt1);
	char direction2 = Direction(pt1, pt2, polygonPt2);
	if (direction1 == direction2 && direction1 != DIRECTION0) //같은 디렉션이고 일직 선상이 아니다
		return false;
	direction1 = Direction(polygonPt1, polygonPt2, pt1);
	direction2 = Direction(polygonPt1, polygonPt2, pt2);
	if (direction1 == direction2 && direction1 != DIRECTION0)
		return false;
	
	return true;
}

double GetArea(POINT pt1, POINT pt2, POINT pt3) //절대값 안취한거임.
{
	double area;
	double a = (pt1.x*pt2.y + pt2.x*pt3.y + pt3.x*pt1.y);
	double b = (pt1.x*pt3.y + pt3.x*pt2.y + pt2.x*pt1.y);
	area = (pt1.x*pt2.y + pt2.x*pt3.y + pt3.x*pt1.y)
		- (pt1.x*pt3.y + pt3.x*pt2.y + pt2.x*pt1.y);
	area /= 2;
	return area;
}

RECTANGLE MakeRectangle(POINT pt1, POINT pt2)
{
	RECTANGLE rec;

	if (pt1.x < pt2.x)
	{
		rec.left = pt1.x;
		rec.right = pt2.x;
	}
	else
	{
		rec.left = pt2.x;
		rec.right = pt1.x;
	}
	if (pt1.y < pt2.y)
	{
		rec.top = pt2.y;
		rec.bottom = pt1.y;
	}
	else
	{
		rec.top = pt1.y;
		rec.bottom = pt2.y;
	}

	return rec;
}
bool CollisionCheck(RECTANGLE rec1, RECTANGLE rec2)
{
	//rec1을 기준으로 충돌 체크
	if (rec1.left <= rec2.right && rec1.right >= rec2.left
		&& rec1.top >= rec2.bottom && rec1.bottom <= rec2.top)
		return true;

	return false;
}
bool CollisionCheck(RECTANGLE rec, POINT pt)
{
	if (rec.left <= pt.x && rec.right >= pt.x && rec.top >= pt.y && rec.bottom <= pt.y)
		return true;
	
	return false;
}
bool IsOnLine(POINT pt, POINT polygonPt1, POINT polygonPt2)
{
	if (Direction(pt, polygonPt1, polygonPt2) == DIRECTION0)
	{
		RECTANGLE rec = MakeRectangle(polygonPt1, polygonPt2);
		if (CollisionCheck(rec, pt))
			return true;
	}
	
	return false;
}