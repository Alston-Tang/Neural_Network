#include <iostream>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include <fstream>
#include "NNet.h"


double connect::getWeight()
{
	return(*conWeight);
}

double node::activF(double in)
{
	return(1/(1+exp(-in)));
}

double node::getValue()
{
	return value;
}

void node::setLayer(int lay)
{
	layer=lay;
}

double node::getDi()
{
	return di;
}


void oNode::calValue()
{
	double in=0;
	for (int i=0; i<numIn; i++)
	{
		in+=(inCon[i].conNode->getValue()* inCon[i].getWeight());
		in+=bias;
	}
	value=activF(in);
}

void oNode::setInNode(int num, connect cont[])
{
	numIn=num;
	for(int i=0; i<num; i++)
	{
		inCon[i].conNode=cont[i].conNode;
		inCon[i].conWeight=cont[i].conWeight;
	}
}

void oNode::updateDi(double ndi)
{
	di=ndi;
}


void ierNode::calValue()
{
	double in=0;
	for (int i=0; i<numIn; i++)
	{
		in+=(inCon[i].conNode->getValue()*inCon[i].getWeight());
		in+=bias;
	}
	value=activF(in);
}
	
void ierNode::setInNode(int num, connect cont[])
{
	numIn=num;
	for(int i=0; i<num; i++)
	{
		inCon[i].conNode=cont[i].conNode;
		inCon[i].conWeight=cont[i].conWeight;
	}
}

void ierNode::setOutNode(int num, connect cont[])
{
	numOut=num;
	for(int i=0; i<num; i++)
	{
		outCon[i].conNode=cont[i].conNode;
		outCon[i].conWeight=cont[i].conWeight;
	}
}

void ierNode::updateDi(double ndi)
{
	di=ndi;
}

int ierNode::numO()
{
	return numOut;
}

void inNode::setInput(double in)
{
	input=in;
}
	
void inNode::calValue()
{
	value=activF(input);
}

void inNode::setOutNode(int num, connect cont[])
{
	numOut=num;
	for(int i=0; i<num; i++)
	{
		outCon[i].conNode=cont[i].conNode;
		outCon[i].conWeight=cont[i].conWeight;
	}
}



void nNet::initNode(int numL,int struc[])
{
	connect temp[50];
	int cur=0;
	numLayer=numL;
	for(int i=0; i<numL; i++)
	{
		strucL[i]=struc[i];
	}
	while(cur<numL)  //initial each node layer by layer from input layer
	{
		if(cur==0)
		{
			for(int i=0; i<struc[cur]; i++)
			{
				inN[i].setLayer(cur);
				for(int j=0; j<struc[cur+1]; j++)
				{
					if(cur<numL-2)  //next layer isn't output layer
						temp[j].conNode=&ierN[0][j];
					else 
						temp[j].conNode=&oN;
					temp[j].conWeight=&weight[0][i][j];
				}
				inN[i].setOutNode(struc[cur+1],temp);
			}
		}
		else if (cur==numL-1)
		{
			oN.setLayer(cur);
			for(int j=0; j<struc[cur-1]; j++)
			{
				if(cur>1)  //previous layer isn't input layer
					temp[j].conNode=&ierN[cur-2][j];
				else
					temp[j].conNode=&inN[j];
				temp[j].conWeight=&weight[cur-1][j][0];
			}
			oN.setInNode(struc[cur-1],temp);
			oN.bias=0;
		}
		else
		{
			for(int i=0; i<struc[cur]; i++)
			{
				ierN[cur-1][i].setLayer(cur);
				for(int j=0; j<struc[cur-1]; j++)
				{
					if(cur>1)  //previous layer isn't input layer
						temp[j].conNode=&ierN[cur-2][j];
					else
						temp[j].conNode=&inN[j];
					temp[j].conWeight=&weight[cur-1][j][i];
				}
				ierN[cur-1][i].setInNode(struc[cur-1],temp);
				for(int j=0; j<struc[cur+1]; j++)
				{
					if(cur<numL-2)  //next layer isn't output layer
						temp[j].conNode=&ierN[cur][j];
					else 
						temp[j].conNode=&oN;
					temp[j].conWeight=&weight[cur][i][j];
				}
				ierN[cur-1][i].setOutNode(struc[cur+1],temp);
				ierN[cur-1][i].bias=0;
			}
		}
		cur++;
	}
}

void nNet::initWeight(int numL, int struc[], double initWei)
{
	int cur=0;
	while(cur<numL-1)
	{
		int thisL=struc[cur];
		int nextL=struc[cur+1];
		for(int i=0; i<thisL; i++)
		{
			for(int j=0; j<nextL; j++)
			{
				weight[cur][i][j]=initWei;
			}
		}
		cur++;
	}
}

void nNet::initWeight(int numL, int struc[])
{
	srand(1);
	int cur=0;
	while(cur<numL-1)
	{
		int thisL=struc[cur];
		int nextL=struc[cur+1];
		for (int i=0; i<thisL; i++)
		{
			for(int j=0; j<nextL; j++)
			{
				double randomNum=(double)(rand()%10000)/(double)10000;
				int postive=rand()%2;
				if (!postive) randomNum*=-1;
				weight[cur][i][j]=randomNum;
			}
		}
		cur++;
	}
}

double nNet::calOutput(double data[])
{
	int cur=0;
	for(int i=0; i<strucL[0]; i++)
	{
		inN[i].setInput(data[i]);
	}
	while(cur<numLayer)
	{
		if(cur==0)
		{
			for(int i=0; i<strucL[0]; i++)
			{
				inN[i].calValue();
			}
		}
		else if(cur==numLayer-1)
		{
			oN.calValue();
		}
		else
		{
			for(int i=0; i<strucL[cur]; i++)
			{
				ierN[cur-1][i].calValue();
			}
		}
		cur++;
	}
	return(oN.getValue());
}

void nNet::displayValue()
{
	int cur=0;
	while(cur<numLayer)
	{
		cout<<"Layer: "<<cur<<"\n";
		if(cur==0)
		{
			for(int i=0; i<strucL[cur]; i++)
			{
				cout<<"InputNode: "<<i<<"  "<<inN[i].getValue()<<"\n";
			}
		}
		else if(cur==numLayer-1)
		{
			cout<<"OutputNode: "<<0<<"  "<<oN.getValue()<<"\n";
		}
		else
		{
			for(int i=0; i<strucL[cur]; i++)
			{
				cout<<"InnerNode: "<<i<<"  "<<ierN[cur-1][i].getValue()<<"\n";
			}
		}
		cur++;
	}
}

void nNet::setAlpha(double a)
{
	alpha=a;
}

void nNet::learn(double data[],double target)
{
	double outP,delta;
	int cur;
	calOutput(data);
	outP=oN.getValue();
	cur=numLayer-1;
	while(cur>0)
	{
		if(cur==numLayer-1)
		{
			double value=oN.getValue();
			oN.di=((target-outP)*value*(1-value));
			delta=oN.di*alpha;
			for(int i=0; i<oN.numIn; i++)
			{
				*oN.inCon[i].conWeight+=oN.inCon[i].conNode->getValue()*delta;
			}
			oN.bias+=delta;
		}
		else
		{
			double err,delta;
			for (int i=0; i<strucL[cur]; i++)
			{
				double value=ierN[cur-1][i].getValue();
				err=0;
				for(int j=0; j<ierN[cur-1][i].numOut; j++)
				{
					err+=ierN[cur-1][i].outCon[j].conNode->di*(*ierN[cur-1][i].outCon[j].conWeight);
				}
				ierN[cur-1][i].di=err*value*(1-value);
				delta=ierN[cur-1][i].di*alpha;
				for(int j=0; j<ierN[cur-1][i].numIn; j++)
				{
					*ierN[cur-1][i].inCon[j].conWeight+=ierN[cur-1][i].inCon[j].conNode->getValue()*delta;
				}
				ierN[cur-1][i].bias+=delta;
			}
		}
		cur--;
	}
}

double nNet::calErr(double  data[], double target)
{
	double err;
	calOutput(data);
	err=target-oN.getValue();
	if(err<0) err*=-1;
	return(err);
}

double nNet::calErr(double outPut,double target)
{
	double err=target-outPut;
	if(err<0) err*=-1;
	return (err);
}

void nNet::displayWeight(char fileName[])
{
	int cur=0,nextCur;
	ofstream fout;
	fout.open(fileName);
	while(cur<numLayer-1)
	{
		nextCur=cur+1;
		fout<<"From layer "<<cur<<" to layer "<<nextCur<<"\n\n";
		for(int i=0; i<strucL[cur]; i++)
		{
			for(int j=0; j<strucL[nextCur]; j++)
			{
				fout<<"From node "<<i<<" to node "<<j<<" weight:"<<weight[cur][i][j]<<"\n";
			}
		}
		fout<<"\n";
		cur++;
	}
}

bool nNet::saveWeight(char fileToSave[])
{
	ofstream file;
	file.open(fileToSave);
	if (!file.is_open()) return false;

	for(int i=0; i<numLayer; i++)
	{
		file<<strucL[i];
		if(i!=numLayer-1) file<<" ";
	}
	file<<"\n";
	int cur=0;
	while(cur<numLayer-1)
	{
		int nextLayer=cur+1;
		if(cur==0)
		{
			file<<"I "<<strucL[cur];
			if(nextLayer<numLayer-1)
			{
				file<<" H "<<strucL[nextLayer]<<"\n";
				for(int i=0; i<strucL[nextLayer]; i++)
				{
					for(int j=0; j<strucL[cur]; j++)
					{
						file<<weight[cur][j][i]<<" ";
					}
					file<<ierN[nextLayer-1][i].bias<<"\n";
				}
			}
			else
			{
				file<<" O "<<strucL[nextLayer]<<"\n";
				for(int i=0; i<strucL[nextLayer]; i++)
				{
					for(int j=0; j<strucL[cur]; j++)
					{
						file<<weight[cur][j][i]<<" ";
					}
					file<<oN.bias<<"\n";
				}
			}
		}
		else
		{
			file<<"H "<<strucL[cur];
			if(nextLayer<numLayer-2)
			{
				file<<" H "<<strucL[nextLayer]<<"\n";
				for(int i=0; i<strucL[nextLayer]; i++)
				{
					for(int j=0; j<strucL[cur]; j++)
					{
						file<<weight[cur][j][i]<<" ";
					}
					file<<ierN[nextLayer-1][i].bias<<"\n";
				}
			}
			else
			{
				file<<" O "<<strucL[nextLayer]<<"\n";
				for(int i=0; i<strucL[nextLayer]; i++)
				{
					for(int j=0; j<strucL[cur]; j++)
					{
						file<<weight[cur][j][i]<<" ";
					}
					file<<oN.bias<<"\n";
				}
			}
		}
		cur++;
	}
	file.close();
	return true;
}
bool nNet::loadWeight(char weightFile[])
{
	ifstream file;
	file.open(weightFile);
	if (!file.is_open()) return false;

	int l;
	char c;
	for(int i=0; i<numLayer; i++)
	{
		file>>l;
		if (l!=strucL[i]) return false;
	}
	int cur=0;
	while (cur<numLayer-1)
	{
		int nextLayer=cur+1;
		file>>c>>l>>c>>l;
		for (int i=0; i<strucL[nextLayer]; i++)
		{

			for (int j=0; j<strucL[cur]; j++)
			{
				file>>weight[cur][j][i];
			}
			if (nextLayer<numLayer-1) file>>oN.bias;
			else file>>ierN[nextLayer-1][i].bias;
		}
		cur++;
	}
	file.close();
	return true;
}