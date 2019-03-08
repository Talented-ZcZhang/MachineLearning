#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <windows.h>
#include <cmath>
#include "Zzc_Matrix.h"
#include "Zzc_SoftmaxRegression.h"
#include "bmpFile.h"

const double ex = 200;

#define POS_X 37      // 坐标系原点 x 
#define POS_Y 450      // 坐标系原点 y 
#define POS_WIDTH 385   // 坐标系宽度

using namespace std;
const int maxn = 1000;
double data[maxn*2];
double label[maxn];
int count;
BYTE *pRGBImg;
BYTE *pRGBImg_Cost;
int width, height;

void Draw_one_point(int x, int y,int f)
{
	if (x < 1 || x >= width - 1 || y < 1 || y >= height- 1){
		printf("draw one point error\n");
		return;
	}
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++){
			if (f == 1){
				*(pRGBImg + ((y + i)*width + x + j) * 3) = 0;
				*(pRGBImg + ((y + i)*width + x + j) * 3 + 1) = 0;
			}
			else if (f == 0)
			{
				*(pRGBImg + ((y + i)*width + x + j) * 3 + 1) = 0;
				*(pRGBImg + ((y + i)*width + x + j) * 3 + 2) = 0;
			}
			else if (f == 2)
			{
				*(pRGBImg + ((y + i)*width + x + j) * 3) = 0;
				*(pRGBImg + ((y + i)*width + x + j) * 3 + 1) = 0;
				*(pRGBImg + ((y + i)*width + x + j) * 3 + 2) = 0;
			}
			else if (f == 3)
			{
				*(pRGBImg_Cost + ((y + i)*width + x + j) * 3) = 0;
				*(pRGBImg_Cost + ((y + i)*width + x + j) * 3 + 1) = 0;
				*(pRGBImg_Cost + ((y + i)*width + x + j) * 3 + 2) = 0;
			}
		}
	}
}
void Draw_Point(double *xy, double *label, int n)
{
	int x0, y0;
	for (int i = 0; i < n; i++)
	{
		if (xy[i * 2] < 0 || xy[i * 2]>1.1 || xy[i * 2 + 1] < 0 || xy[i * 2 + 1]>1.1){
			printf("Draw point error!\n");
			return;
		}
		x0 = ((int)(xy[i * 2] * POS_WIDTH + POS_X));
		y0 = ((int)(POS_Y - xy[i * 2 + 1] * POS_WIDTH));
		Draw_one_point(x0, y0, (int)label[i]);
	}
	RmwWrite24BitImg2BmpFile(pRGBImg, width, height, "..//point.bmp");

}

int load_data(ZMatrix *&da, ZMatrix *&la)
{
	FILE *px = fopen("..//ex4x.dat", "r");
	FILE *py = fopen("..//ex4y.dat", "r");
	if(px==NULL)
		printf("x Open failed\n");
	if(py == NULL)
		printf("y Open failed\n");

	count = 0;
	double mxx = -1e8, mix = 1e8, mxy = -1e8, miy = 1e8;
	while (!feof(px))
	{
		fscanf(px, "%lf", data + count * 2);
		fscanf(px, "%lf", data + count *2 + 1);
		if (mxx < data[count * 2] && data[count * 2]>0)mxx = data[count * 2];
		if (mix > data[count * 2] && data[count * 2] > 0)mix = data[count * 2];
		if (mxy < data[count * 2 + 1] && data[count * 2 + 1]>0)mxy = data[count * 2 + 1];
		if (miy > data[count * 2 + 1] && data[count * 2 + 1] > 0)miy = data[count * 2 + 1];
		fscanf(py, "%lf", label+ count );
		count++;//显示第i行数组里的数据，\t表示数与数间隔一个空位  
	}
	printf("mxx  mix: %lf %lf \n", mxx, mix);
	printf("mxy  miy: %lf %lf \n", mxy, miy);
	printf("count: %d\n", count);
	fclose(px);
	fclose(py);
	count -= 1;
	da = new ZMatrix(count, 3);
	la = new ZMatrix(count, 2);
	for (int i = 0; i < count; i++)
	{

	
		data[i * 2] = (data[i * 2] - mix) / (mxx - mix);
		data[i * 2 + 1] = (data[i * 2 + 1] - miy) / (mxy - miy);

		*(da->mat + i * 3) = data[i * 2];
		*(da->mat + i * 3 + 1) = data[i * 2 + 1];
		*(da->mat + i * 3 + 2) = 1;

		if (label[i])
			*(la->mat + i * 2) = 1;
		else
			*(la->mat + i * 2 + 1) = 1;
	}
	debug(da);


	pRGBImg = RmwRead24BitBmpFile2Img("..//1.bmp", &width, &height);
	pRGBImg_Cost = RmwRead24BitBmpFile2Img("..//1.bmp", &width, &height);
	printf("%d %d\n", width, height);
	Draw_Point(data, label, count);

	return count - 1;
}

void Draw_Line(int x1, int y1, int x2, int y2)
{
	
	for (int i = 0; i <height; i++)
	{
		for (int j = 0; j <width; j++)
		{
			if ((((i - y1)*(j - x2) - (i - y2)*(j - x1)) < ex)&&
				(((i - y1)*(j - x2) - (i - y2)*(j - x1)) > -ex))
			{
				Draw_one_point(j, i, 2);
			
			}

		}
	}
}
void Z_Draw_Line(double x1, double y1, double x2, double y2)
{
	int x01, y01, x02, y02;
	x01 = ((int)(x1 * POS_WIDTH + POS_X));
	y01 = ((int)(POS_Y - y1 * POS_WIDTH));
	x02 = ((int)(x2 * POS_WIDTH + POS_X));
	y02 = ((int)(POS_Y - y2 * POS_WIDTH));
	Draw_Line(x01, y01, x02, y02);
	RmwWrite24BitImg2BmpFile(pRGBImg, width, height, "..//line_10000_sgd.bmp");
}

void Z_Draw_Cost(double *cost, int num)
{
	int x0, y0, step = num / POS_WIDTH, c = 0, cstep = 1;
	if (!step)step = 1;
	if (num<POS_WIDTH){
		cstep = POS_WIDTH / num;
	}
	for (int i = 0; i < num; i+=step)
	{
		y0 = ((int)(POS_Y - cost[i] * POS_WIDTH));
		x0 = POS_X + c;
		Draw_one_point(x0,y0,3);
		c += cstep;
	}
	RmwWrite24BitImg2BmpFile(pRGBImg_Cost, width, height, "..//cost_10000_sgd.bmp");
}

