using namespace std;

class node
{
public:
	double value,di,bias;
	int layer;
public:
	double activF(double);
	double getValue();
	double getDi();
	void setLayer(int);
};

class connect
{
	public:
	node *conNode;
	double *conWeight;
	double getWeight();
};

class oNode:public node
{
public:
	connect inCon[50];
	int numIn;
public:
	void calValue();
	void setInNode(int num, connect cont[]);
	void updateDi(double);
};

class ierNode: public node
{
public:
	connect inCon[50],outCon[50];
	int numIn,numOut;	
public:
	void calValue();
	void setInNode(int num, connect cont[]);
	void setOutNode(int num, connect cont[]);
	void updateDi(double);
	int numO();
};

class inNode:public node
{
public:
	connect outCon[50];
	int numOut,numInput;
	double input;
public:	
	void setInput(double in);  //set the input of this input node
	void calValue();
	void setOutNode(int num, connect cont[]);
};

class nNet
{
public:
	double weight[10][50][50],alpha;
	inNode inN[50];
	ierNode ierN[10][50];
	oNode oN;
	int strucL[10],numLayer;
	void initNode(int,int[]);
	void initWeight(int,int[],double);
	void initWeight(int,int[]);
	double calOutput(double[]);
	void displayValue();
	void setAlpha(double a);
	void learn(double[],double);
	double calErr(double[],double);
	double calErr(double,double);
	void displayWeight(char[]);
	bool saveWeight(char []);
	bool loadWeight(char []);
};