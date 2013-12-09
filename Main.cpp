#include "NNet.h"
#include "preprocess.h"
#include "resultChecker.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <cstdlib>

using namespace std;

int main()
{
	double **data;
	double *target,result[2][2000];
	double errBef=0,errAft=0;
	double resultT[1000],targetT[1000];
	int trainSet[2000];
	int count=0;

	preProcessor pre;
	ofstream ftest;
	nNet net;
	resultChecker check;

	ftest.open("D:\\nnpro\\test.txt");

	target=new double[2000];
	data=new double*[2000];

	for (int i=0; i<2000; i++)
		data[i]=new double[40];

	int struc[30]={30,15,1};
	pre.setCor(0,1,2);
	pre.setCorT(0,1);
	pre.setUn(0);
	pre.setUnT(false);
	pre.setSize(30);
	pre.loadFile("D:\\nnpro\\raw-training.data");
	while (!pre.end())
	{
		pre.nextL(data[count]);
		target[count]=pre.nextT();
		if(!pre.end()) count++;
	}
	net.initNode(3,struc);
	net.setAlpha(0.05);
	check.setAcceptNErr(1,0.5);
	if(!net.loadWeight("D:\\nnpro\\weight"))
		net.initWeight(3,struc);

	for(int i=0; i<1120; i++)
	{
		trainSet[i]=i;
	}
	//net.learn(data[0],target[0]);
			double max=0;
 	for (int j=0; j<1000; j++)
	{
		for(int i=1119; i>=0; i--)
		{
			int choose;
			choose=rand()%(i+1);
			int temp=trainSet[choose];
			trainSet[choose]=trainSet[i];
			trainSet[i]=temp;
			net.learn(data[trainSet[choose]],target[trainSet[choose]]);
			result[0][trainSet[choose]]=net.oN.getValue();
		}
		for(int i=1120-1; i<1600; i++)
		{
			double temp=net.calOutput(data[i]);
			resultT[i-1119]=temp;
			targetT[i-1119]=target[i];
		}

		double co=check.checkCorrectRate(resultT,targetT,480);
		if (max<co) max=co;  
		cout<<"#"<<co<<"\n";
	}
	net.saveWeight("D:\\nnpro\\weight");
	cout<<max;

/*	for(int i=0; i<1000; i++)
	{
		for(int j=0; j<1120; j++)
		{
			net.learn(data[j],target[j]);
			result[0][j]=net.oN.getValue();
		}
		for(int i=1120-1; i<1600; i++)
		{
			double temp=net.calOutput(data[i]);
			resultT[i-1119]=temp;
			targetT[i-1119]=target[i];
		}
		cout<<"#"<<check.checkCorrectRate(result[0],target,1120)<<"\n";
		cout<<"#"<<check.checkCorrectRate(resultT,targetT,480)<<"\n\n";
	}
	
	for(int i=0; i<1120; i++) 
	{
		result[1][i]=net.calOutput(data[i]);
	}
	cout<<check.checkCorrectRate(result[1],target,1120)<<"\n";

	cout<<check.checkCorrectRate(resultT,targetT,480)<<"\n";
	net.saveWeight("D:\\nnpro\\weight.wet");
	pre.closeFile();
	for(int i=0; i<2000; i++) delete data[i];
	delete data;
	*/
}