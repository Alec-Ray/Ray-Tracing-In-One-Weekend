#include <iostream>
#include "vec3.h"
#include <fstream>
using namespace std;

int main() {
	ofstream outfile;
	outfile.open("IMG.txt");

	int nx = 800,ny=400;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--) 
	{
		for (int i = 0; i < nx; i++) 
		{
			vec3 vecTemp(float(i) / float(nx), float(j) / float(ny), 0.2);
			int iRed = int(255.99*vecTemp[0]);
			int iGreen = int(255.99*vecTemp[1]);
			int iBlue = int(255.99*vecTemp[2]);

			outfile << iRed << " " << iGreen << " " << iBlue << "\n";
		}
	}
	outfile.close();
	return 0;
}