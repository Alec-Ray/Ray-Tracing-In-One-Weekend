#include <iostream>
#include <fstream>

using namespace std;

int main() 
{
	ofstream outfile;
	outfile.open("IMG.txt");

	int nx = 800;
	int ny = 400;
	outfile << "P3\n"<< nx << " " << ny << "\n255\n";
	for (int j = ny - 1; j >= 0; j--) 
	{
		for (int i = 0; i < nx; i++)
		{
			//从左到右，红色通道值增加
			float r = float(i) / float(nx);		

			//从上到下，绿色通道值减小
			float g = float(j) / float(ny);		

			//蓝色通道值不变
			float b = 0.2f;						

			int ir = int(255.99f*r);
			int ig = int(255.99f*g);
			int ib = int(255.99f*b);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	return 0;
}