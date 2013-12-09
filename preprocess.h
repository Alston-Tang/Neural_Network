#include <fstream>

using namespace std;

class preProcessor
{
private:
	ifstream fin;
	ofstream fout;
	bool eof;
	double corres[3],corresT[2];
	double unKnown;
	bool unKnownT;
	int size;
public:
	void setCor(double,double,double);
	void setCorT(double,double);
	void setUn(double);
	void setSize(int);
	void setUnT(bool);
	bool loadFile(const char[]);
	double nextA();
	bool nextL(double[]);
	double nextT();
	bool end();
	void closeFile();
	bool checkResult(double res,double target);
};