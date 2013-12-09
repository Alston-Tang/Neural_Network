#include "resultChecker.h"

void resultChecker::setAcceptNErr(double acc,double err)
{
	accept=acc;
	error=err;
}
double resultChecker::checkCorrectRate(double result[],double target[],int num)
{
	int correct=0,incorrect=0;
	for(int i=0; i<num; i++)
	{
		int isCorrect;
		if (result[i]-accept>error || accept-result[i]>error)
		{
			if(target[i]==0) correct++;
			else incorrect++;
		}
		else
		{
			if(target[i]==1) correct++;
			else incorrect++;
		}
	}
	return(static_cast<double>(correct)/static_cast<double>(correct+incorrect));
}