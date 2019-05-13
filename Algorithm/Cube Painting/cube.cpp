
#include <iostream>
#include <fstream>
#include<string.h>
using namespace std;

bool Input();
bool MainAlgoritm();
bool IsMatch();

void MakeCompare();

ofstream fOut;
ifstream fIn;
char g_Input[16];
char g_diceOrigin[6];
char g_diceCompare[6];

struct COMPARE
{
	char one;
	char two;
	char three;
	char four;
	char five;
	char six;
};

COMPARE g_compare[6][4];


int main()
{
	char endCheck;

	fIn.open("cube.inp");
	fOut.open("cube.out");

	char temp1[5] = "abcd";
	char temp2[5] = "abcd";

	int temp = strcmp(temp1, temp2);

	while (true)
	{
		if (Input() == false)
			break;
		if (MainAlgoritm() == true)
			fOut << "TRUE" << endl;
		else
			fOut << "FALSE" << endl;

	}

	fIn.close();
	fOut.close();
	return 0;
}

bool Input()
{

	fIn >> g_Input;
	int endCheck = g_Input[0];
	for (int i = 0; i < 12; i++)
	{
		if (g_Input[i] != endCheck)
			break;
		if (i == 11)
		{
			fOut << "TRUE" << endl;
			return false;
		}
	}
	memcpy(g_diceOrigin, g_Input, 6);
	memcpy(g_diceCompare, &g_Input[6], 6);

	return true;
}
bool MainAlgoritm()
{
	//마주보는 면은 1-6,  2-5,  3-4
	if (IsMatch() == true)
		return true;

	return false;
}

bool IsMatch()
{
	MakeCompare();

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (g_diceCompare[0] == g_compare[i][j].one &&
				g_diceCompare[1] == g_compare[i][j].two &&
				g_diceCompare[2] == g_compare[i][j].three &&
				g_diceCompare[3] == g_compare[i][j].four &&
				g_diceCompare[4] == g_compare[i][j].five &&
				g_diceCompare[5] == g_compare[i][j].six)
				return true;
		}
	}
	return false;
}


void MakeCompare()
{

	//16, 25, 34, 43, 52, 61

	g_compare[0][0].one = g_diceOrigin[0];
	g_compare[0][0].two = g_diceOrigin[1];
	g_compare[0][0].three = g_diceOrigin[2];
	g_compare[0][0].four = g_diceOrigin[3];
	g_compare[0][0].five = g_diceOrigin[4];
	g_compare[0][0].six = g_diceOrigin[5];


	g_compare[0][1].one = g_diceOrigin[0];
	g_compare[0][1].two = g_diceOrigin[3];
	g_compare[0][1].three = g_diceOrigin[1];
	g_compare[0][1].four = g_diceOrigin[4];
	g_compare[0][1].five = g_diceOrigin[2];
	g_compare[0][1].six = g_diceOrigin[5];

	g_compare[0][2].one = g_diceOrigin[0];
	g_compare[0][2].two = g_diceOrigin[4];
	g_compare[0][2].three = g_diceOrigin[3];
	g_compare[0][2].four = g_diceOrigin[2];
	g_compare[0][2].five = g_diceOrigin[1];
	g_compare[0][2].six = g_diceOrigin[5];

	g_compare[0][3].one = g_diceOrigin[0];
	g_compare[0][3].two = g_diceOrigin[2];
	g_compare[0][3].three = g_diceOrigin[4];
	g_compare[0][3].four = g_diceOrigin[1];
	g_compare[0][3].five = g_diceOrigin[3];
	g_compare[0][3].six = g_diceOrigin[5];
	//////////////////////
	g_compare[1][0].one = g_diceOrigin[1];
	g_compare[1][0].two = g_diceOrigin[5];
	g_compare[1][0].three = g_diceOrigin[2];
	g_compare[1][0].four = g_diceOrigin[3];
	g_compare[1][0].five = g_diceOrigin[0];
	g_compare[1][0].six = g_diceOrigin[4];

	g_compare[1][1].one = g_diceOrigin[1];
	g_compare[1][1].two = g_diceOrigin[3];
	g_compare[1][1].three = g_diceOrigin[5];
	g_compare[1][1].four = g_diceOrigin[0];
	g_compare[1][1].five = g_diceOrigin[2];
	g_compare[1][1].six = g_diceOrigin[4];

	g_compare[1][2].one = g_diceOrigin[1];
	g_compare[1][2].two = g_diceOrigin[0];
	g_compare[1][2].three = g_diceOrigin[3];
	g_compare[1][2].four = g_diceOrigin[2];
	g_compare[1][2].five = g_diceOrigin[5];
	g_compare[1][2].six = g_diceOrigin[4];

	g_compare[1][3].one = g_diceOrigin[1];
	g_compare[1][3].two = g_diceOrigin[2];
	g_compare[1][3].three = g_diceOrigin[0];
	g_compare[1][3].four = g_diceOrigin[5];
	g_compare[1][3].five = g_diceOrigin[3];
	g_compare[1][3].six = g_diceOrigin[4];
	//////////////////
	g_compare[2][0].one = g_diceOrigin[2];
	g_compare[2][0].two = g_diceOrigin[1];
	g_compare[2][0].three = g_diceOrigin[5];
	g_compare[2][0].four = g_diceOrigin[0];
	g_compare[2][0].five = g_diceOrigin[4];
	g_compare[2][0].six = g_diceOrigin[3];

	g_compare[2][1].one = g_diceOrigin[2];
	g_compare[2][1].two = g_diceOrigin[0];
	g_compare[2][1].three = g_diceOrigin[1];
	g_compare[2][1].four = g_diceOrigin[4];
	g_compare[2][1].five = g_diceOrigin[5];
	g_compare[2][1].six = g_diceOrigin[3];

	g_compare[2][2].one = g_diceOrigin[2];
	g_compare[2][2].two = g_diceOrigin[4];
	g_compare[2][2].three = g_diceOrigin[0];
	g_compare[2][2].four = g_diceOrigin[5];
	g_compare[2][2].five = g_diceOrigin[1];
	g_compare[2][2].six = g_diceOrigin[3];

	g_compare[2][3].one = g_diceOrigin[2];
	g_compare[2][3].two = g_diceOrigin[5];
	g_compare[2][3].three = g_diceOrigin[4];
	g_compare[2][3].four = g_diceOrigin[1];
	g_compare[2][3].five = g_diceOrigin[0];
	g_compare[2][3].six = g_diceOrigin[3];
	////////////////
	g_compare[3][0].one = g_diceOrigin[3];
	g_compare[3][0].two = g_diceOrigin[0];
	g_compare[3][0].three = g_diceOrigin[4];
	g_compare[3][0].four = g_diceOrigin[1];
	g_compare[3][0].five = g_diceOrigin[5];
	g_compare[3][0].six = g_diceOrigin[2];

	g_compare[3][1].one = g_diceOrigin[3];
	g_compare[3][1].two = g_diceOrigin[1];
	g_compare[3][1].three = g_diceOrigin[0];
	g_compare[3][1].four = g_diceOrigin[5];
	g_compare[3][1].five = g_diceOrigin[4];
	g_compare[3][1].six = g_diceOrigin[2];

	g_compare[3][2].one = g_diceOrigin[3];
	g_compare[3][2].two = g_diceOrigin[5];
	g_compare[3][2].three = g_diceOrigin[1];
	g_compare[3][2].four = g_diceOrigin[4];
	g_compare[3][2].five = g_diceOrigin[0];
	g_compare[3][2].six = g_diceOrigin[2];

	g_compare[3][3].one = g_diceOrigin[3];
	g_compare[3][3].two = g_diceOrigin[4];
	g_compare[3][3].three = g_diceOrigin[5];
	g_compare[3][3].four = g_diceOrigin[0];
	g_compare[3][3].five = g_diceOrigin[1];
	g_compare[3][3].six = g_diceOrigin[2];
	////////////////
	g_compare[4][0].one = g_diceOrigin[4];
	g_compare[4][0].two = g_diceOrigin[3];
	g_compare[4][0].three = g_diceOrigin[0];
	g_compare[4][0].four = g_diceOrigin[5];
	g_compare[4][0].five = g_diceOrigin[2];
	g_compare[4][0].six = g_diceOrigin[1];

	g_compare[4][1].one = g_diceOrigin[4];
	g_compare[4][1].two = g_diceOrigin[5];
	g_compare[4][1].three = g_diceOrigin[3];
	g_compare[4][1].four = g_diceOrigin[2];
	g_compare[4][1].five = g_diceOrigin[0];
	g_compare[4][1].six = g_diceOrigin[1];

	g_compare[4][2].one = g_diceOrigin[4];
	g_compare[4][2].two = g_diceOrigin[2];
	g_compare[4][2].three = g_diceOrigin[5];
	g_compare[4][2].four = g_diceOrigin[0];
	g_compare[4][2].five = g_diceOrigin[3];
	g_compare[4][2].six = g_diceOrigin[1];

	g_compare[4][3].one = g_diceOrigin[4];
	g_compare[4][3].two = g_diceOrigin[0];
	g_compare[4][3].three = g_diceOrigin[2];
	g_compare[4][3].four = g_diceOrigin[3];
	g_compare[4][3].five = g_diceOrigin[5];
	g_compare[4][3].six = g_diceOrigin[1];
	////////////////
	g_compare[5][0].one = g_diceOrigin[5];
	g_compare[5][0].two = g_diceOrigin[1];
	g_compare[5][0].three = g_diceOrigin[3];
	g_compare[5][0].four = g_diceOrigin[2];
	g_compare[5][0].five = g_diceOrigin[4];
	g_compare[5][0].six = g_diceOrigin[0];

	g_compare[5][1].one = g_diceOrigin[5];
	g_compare[5][1].two = g_diceOrigin[2];
	g_compare[5][1].three = g_diceOrigin[1];
	g_compare[5][1].four = g_diceOrigin[4];
	g_compare[5][1].five = g_diceOrigin[3];
	g_compare[5][1].six = g_diceOrigin[0];

	g_compare[5][2].one = g_diceOrigin[5];
	g_compare[5][2].two = g_diceOrigin[4];
	g_compare[5][2].three = g_diceOrigin[2];
	g_compare[5][2].four = g_diceOrigin[3];
	g_compare[5][2].five = g_diceOrigin[1];
	g_compare[5][2].six = g_diceOrigin[0];

	g_compare[5][3].one = g_diceOrigin[5];
	g_compare[5][3].two = g_diceOrigin[3];
	g_compare[5][3].three = g_diceOrigin[4];
	g_compare[5][3].four = g_diceOrigin[1];
	g_compare[5][3].five = g_diceOrigin[2];
	g_compare[5][3].six = g_diceOrigin[0];
}