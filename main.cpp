#include <windows.h>
#include <cstdio>
#include <cmath>
#include "Zzc_SoftmaxRegression.h"
#include "Zzc_PicDraw.h"

void main()
{
	ZMatrix *data=NULL, *label=NULL;
	int c=load_data(data,label);       //��ȡ�ļ�����
	printf("load data finish\n");
	ZSoftmaxRegression softmax(3,2);   //�����������ȣ��������
	softmax.GetSampleData(data);     //��������
	softmax.GetSampleLabel(label);    //�����ǩ
	printf("begin train\n");
	//������Ӧ�� ѧϰ�ʣ�ѵ�������� �������£� =1 ʱΪSGD  =n ΪGD ��GD��Ҫ�޸��ڲ����� 
	softmax.TrainModel(0.15, 10000,1, 100);
	softmax.PredictAllData();
	system("pause");
}
