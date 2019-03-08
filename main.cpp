#include <windows.h>
#include <cstdio>
#include <cmath>
#include "Zzc_SoftmaxRegression.h"
#include "Zzc_PicDraw.h"

void main()
{
	ZMatrix *data=NULL, *label=NULL;
	int c=load_data(data,label);       //读取文件数据
	printf("load data finish\n");
	ZSoftmaxRegression softmax(3,2);   //设置特征长度，及类别数
	softmax.GetSampleData(data);     //载入数据
	softmax.GetSampleLabel(label);    //载入标签
	printf("begin train\n");
	//参数对应： 学习率，训练次数， 批量更新（ =1 时为SGD  =n 为GD ，GD需要修改内部程序） 
	softmax.TrainModel(0.15, 10000,1, 100);
	softmax.PredictAllData();
	system("pause");
}
