class resultChecker
{
public:

	double accept,error;
	void setAcceptNErr(double accept,double err);
	double checkCorrectRate(double result[],double target[],int num);
};