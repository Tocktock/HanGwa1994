#include <fstream>
#include <cmath>
#include <algorithm>
#include <vector>


class SnailInfo
{
public:
	int m_goalH;
	int m_sunClimb;
	int m_nightSleep;
	int m_missingClimb;
};
using namespace std;
bool Input();
void Output();
void MainAlgorithm();

SnailInfo g_snailInfo;
bool g_success;
int g_day;
ifstream fIn;
ofstream fOut;

int main()
{
	fIn.open("snail.inp");
	fOut.open("snail.out");
	while (Input())
	{
		MainAlgorithm();
		Output();
	}

	fIn.close();
	fOut.close();
}

bool Input()
{
	int percent;
	fIn >> g_snailInfo.m_goalH;
	fIn >> g_snailInfo.m_sunClimb;
	fIn >> g_snailInfo.m_nightSleep;
	fIn >> percent;

	g_snailInfo.m_goalH *= 100;
	g_snailInfo.m_sunClimb *= 100;
	g_snailInfo.m_nightSleep *= 100;
	g_snailInfo.m_missingClimb = g_snailInfo.m_sunClimb * percent / 100;

	if (g_snailInfo.m_goalH == 0 && g_snailInfo.m_sunClimb == 0 &&
		g_snailInfo.m_nightSleep == 0 && percent == 0)
		return false;

	return true;
}
void Output()
{
	if (g_success == true)
		fOut << "success on day ";
	else
		fOut << "failure on day ";

	fOut << g_day << endl;
}

void MainAlgorithm()
{
	double height = 0;
	g_day = 1;
	g_success = false;
	while (height <= g_snailInfo.m_goalH) // 올라간 높이가 H보다 작으면 반복
	{
		if (g_snailInfo.m_sunClimb <= 0)
			g_snailInfo.m_sunClimb = 0;
		height += g_snailInfo.m_sunClimb;
		if (height > g_snailInfo.m_goalH)
		{
			g_success = true;
			break;
		}

		height -= g_snailInfo.m_nightSleep;
		if (height <= 0)
			break;
		g_day++;
		g_snailInfo.m_sunClimb -= g_snailInfo.m_missingClimb;
	}
}
