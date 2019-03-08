#include "Zzc_SoftmaxRegression.h"
#include "Zzc_Matrix.h"
#include "Zzc_PicDraw.h"
#include <cstdio>
#include <cstring>
#include <cmath>
#include <windows.h>
#include <ctime>
#include <cstdlib>
const double inf = 1e8;
const long long maxn = 1e6;
double cost[maxn];
bool vis[maxn];
ZSoftmaxRegression::ZSoftmaxRegression(int l, int c)
{
	FeatureVec_Length = l;
	Class_Num = c;
	ZMatrix* SampleData = NULL;
	ZMatrix* SampleLabel = NULL;
	ZMatrix* Weight = NULL;
	ZMatrix* Prob = NULL;

}
ZSoftmaxRegression::~ZSoftmaxRegression()
{
	if (SampleData)delete SampleData;
	if (SampleLabel)delete SampleLabel;
	if (Weight)delete Weight;
	if (Prob)delete Prob;
}
void ZSoftmaxRegression::GetSampleData(ZMatrix*data)
{
	if (SampleData)delete SampleData;
	SampleData = new ZMatrix(data->row, data->col);
	int i, j, r = data->row, c = data->col;
	for ( i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			*(SampleData->mat + i*SampleData->col + j) = *(data->mat + i*data->col + j);
		}
	}
}
void ZSoftmaxRegression::GetSampleLabel(ZMatrix*label)
{
	if (SampleLabel)delete SampleLabel;
	SampleLabel = new ZMatrix(label->row, label->col);
	int i, j, r = label->row, c = label->col;
	for (i = 0; i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			*(SampleLabel->mat + i*SampleLabel->col + j) = *(label->mat + i*label->col + j);
		}
	}
}
void debug(ZMatrix *t)
{
	int r = t->row, c = t->col;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < c; j++)
		{
			printf("%lf ", t->mat[i*c + j]);
		}
		printf("\n");
	}
}
void ZSoftmaxRegression::CalProb()
{
	ZMatrix::ZMatMUL(*SampleData, *Weight,*Prob);
	/*printf("data:\n");
	debug(SampleData);
	printf("weight\n");
	debug(Weight);
	printf("data mul w:\n");;
	debug(Prob);*/
	int i, j, r = SampleData->row, c = Weight->col;
	for (i = 0;i < r; i++)
	{
		for (j = 0; j < c; j++)
		{
			*(Prob->mat + i*Prob->col + j) = exp(*(Prob->mat + i*Prob->col + j));
		}
	}
	/*printf("e\n");
	debug(Prob);*/
	double sum = 0;
	for (i= 0; i <r; i++)
	{
		sum = 0;
		for (j = 0; j < c; j++)
		{
			sum += *(Prob->mat + i*Prob->col + j);
		}

		for (j = 0; j < c; j++)
		{
			*(Prob->mat + i*Prob->col + j) = *(Prob->mat + i*Prob->col + j) / sum;
		}
	}
	/*printf("pro:\n");
	debug(Prob);*/
}

void ZSoftmaxRegression::TrainModelOnce(long long train_num_per)
{
	//srand((int)time(0));
	int i, j, k, r = SampleData->row, c = SampleData->col,count=0,tmp;
	ZMatrix* Vec = new ZMatrix(FeatureVec_Length, 1);
	/*printf("label: \n");
	debug(SampleLabel);
	printf("prob: \n");
	debug(Prob)*/;
	ZMatrix::ZMatSUB(*SampleLabel, *Prob, *Prob);
	/*printf("1-prob:\n");
	debug(Prob);*/
	memset(vis, 0, sizeof(vis));
	while (count < train_num_per)
	{
		tmp = rand() % r;
		if (vis[tmp])continue;
		vis[tmp] = true;
		count++;
	}
	for ( k = 0; k < Class_Num-1; k++)
	{
		ZMatrix::Reset(*Vec);
		for (i = 0; i < r; i++)           //*当批量更新样本数为全部或较大时，可能导致随机数生成填充数组较慢，需要修改程序,
		{									//*即注释*标注行，并释放+标注行
			if (!vis[i])continue;			//*
	    
		//for (i = 0; i < r; i++)           //+
		//	{								//+
		//		if (vis[i])printf("%d ", i);//+
			for (j = 0; j < c; j++)
			{
				*(Vec->mat + j) += (*(SampleData->mat + i*SampleData->col + j) * (*(Prob->mat + i*Prob->col + k)));
			}
		//	}								//+
		}									//*
		ZMatrix::ZMatScaleMul(*Vec, *Vec, (double)alpha / SampleData->row);

		/*printf("Vec: \n");
		debug(Vec);*/


		for (i= 0; i < c; i++)
		{
			*(Weight->mat + i*Weight->col + k) += (*(Vec->mat + i));
		}
		/*printf("Weight: \n");
		debug(Weight);*/
		
	}
}
void ZSoftmaxRegression::TrainModel(double al, long long num, long long train_num_per, double error)
{
	if (train_num_per <= 0 || train_num_per > SampleData->row){
		printf("train_num_per > total num!\n");
		return;
	}
	alpha = al;
	Num_Learn = num;
	allow_error = error;
	Weight = new ZMatrix(FeatureVec_Length, Class_Num);
	Prob = new ZMatrix(SampleData->row, Class_Num);
	printf("SampleData->row: %d  FeatureVec_Length: %d  Class_Num:  %d\n", SampleData->row, FeatureVec_Length, Class_Num);

	//debug(SampleLabel);
	int n = 0, i, j, r = Prob->row, c = Prob->col;
	double e = inf,tmp=0,x1=0,x2=0,y1=0,y2=0,mxcost=-1,micost=inf;
	for (n = 0; n<num; n++)
	{
		tmp = 0;
		TrainModelOnce(train_num_per);
		CalProb();
		/*printf("Prob:\n ");
		debug(Prob);*/
		for (i = 0; i < r; i++)
		{
			for (j = 0; j < c; j++)
			{
				if ((int)(*SampleLabel->mat + i*SampleLabel->col + j)==1)
					tmp += log(*(Prob->mat + i*Prob->col + j)*(*(SampleLabel->mat + i*SampleLabel->col + j)));	
			}
	
		}
		printf("(n = %d) :  pro = %lf\n ", n, tmp);
		cost[n] = tmp;
		if (tmp > mxcost)mxcost = tmp;
		if (tmp < micost)micost = tmp;
		//system("pause");
	}
	for (int n = 0; n < num; n++)
	{
		cost[n] = (cost[n]-micost)/(mxcost-micost);
	}
	Z_Draw_Cost(cost, num);
}
int ZSoftmaxRegression::Predict(double* WaitData)
{
	double sum = 0, mx = -1e8;int  mxi = 0;
	for (int i = 0; i < Class_Num; i++)
	{
		sum = 0;
		for (int j = 0; j < FeatureVec_Length; j++)
		{
			sum += (*(Weight->mat + j*Class_Num + i)*WaitData[j]);
		}
		if (sum > mx)
		{
			mx = sum;
			mxi = i;
		}
	}
	return mxi;
}
void ZSoftmaxRegression::PredictAllData()
{
	int t,TrueCount = 0, Total = SampleData->row;
	for (int i = 0; i < SampleData->row; i++)
	{
		t = Predict(SampleData->mat + i*SampleData->col);
		if (*(SampleLabel->mat + i*SampleLabel->col + t) >0.5)
		{
			TrueCount++;
		}
	}
	printf("True: %d  total : %d  rate:  %lf\n", TrueCount, Total, (double)TrueCount / Total);

	double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	y1 = (-1)*Weight->mat[2 * 2] / Weight->mat[2 * 1];
	x2 = (-1)*Weight->mat[2 * 2] / Weight->mat[2 * 0];

	//debug(Weight);
	printf("x1: %lf  y1: %lf  x2:  %lf  y2:  %lf\n", x1, y1, x2, y2);
	Z_Draw_Line(x1, y1, x2, y2);
}
