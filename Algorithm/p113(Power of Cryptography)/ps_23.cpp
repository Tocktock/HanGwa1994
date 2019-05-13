#include<fstream>
#include<algorithm>
#include<math.h>
using namespace std;

ifstream fIn;
ofstream fOut;


int main()
{
	fIn.open("ps_23.inp");
	fOut.open("ps_23.out");

	double n, p, k;
	while (fIn >> n >> p && (n, p) != EOF)
	{
		k = pow(p, 1 / n);
		fOut << k<<endl;
	}

	fIn.close();
	fOut.close();
	return 0;
}

