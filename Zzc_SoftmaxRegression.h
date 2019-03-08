#ifndef ZZC_SOFTMAXREGRESSION_H
#define ZZC_SOFTMAXREGRESSION_H
#include"Zzc_Matrix.h"
#include <windows.h>
class ZSoftmaxRegression{
private:
	ZMatrix* SampleData;
	ZMatrix* SampleLabel;
	int FeatureVec_Length;
	int Class_Num;

	ZMatrix* Weight;
	ZMatrix* Prob;

	double alpha;
	long long Num_Learn;
	double allow_error;

	//HWND hWnd;

	void TrainModelOnce(long long train_num_per);
	void CalProb();
public:
	ZSoftmaxRegression(int l, int c);
	~ZSoftmaxRegression();
	void GetSampleData(ZMatrix*data);
	void GetSampleLabel(ZMatrix*label);

	void TrainModel(double al,long long num,long long train_num_per,double error);
	int Predict(double *WaitData);
	void PredictAllData();
	

};
void debug(ZMatrix *t);
#endif