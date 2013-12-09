#include "preprocess.h"

bool preProcessor::loadFile(const char inFile[])
{
	fin.open(inFile);
	eof=fin.eof();
	return(fin.is_open());
}

void preProcessor::setCor(double c0,double c1,double c2)
{
	corres[0]=c0;
	corres[1]=c1;
	corres[2]=c2;
}

void preProcessor::setCorT(double no,double yes)
{
	corresT[0]=no;
	corresT[1]=yes;
}

void preProcessor::setUn(double un)
{
	unKnown=un;
}

void preProcessor::setSize(int s)
{
	size=s;
}

void preProcessor::setUnT(bool un)
{
	unKnownT=un;
}

double preProcessor::nextA()
{
	char temp=0;
	fin>>temp;
	while((temp<'0' || temp>'2') && temp!='?' && !fin.eof())
	{
		fin>>temp;
	}
	if (temp=='?') return (unKnown);
	else if(temp!=0) 
		return(corres[temp-'0']);
	else
	{
		eof=true;
		return 0;
	}
}

bool preProcessor::nextL(double data[])
{
	int count=0;
	while(!eof && count<size)
	{
		data[count]=nextA();
		count++;
	}
	return(!eof);
}

double preProcessor::nextT()
{
	char temp=0;
	fin>>temp;
	while((temp<'0' || temp>'1') && temp!='?' && !fin.eof())
	{
		fin>>temp;
	}
	if (temp=='?') return (unKnownT);
	else if(temp!=0) 
		if(temp=='0') return corresT[0];
		else return corresT[1];
	else
	{
		eof=true;
		return 0;
	}
}

bool preProcessor::end()
{
	return eof;
}

void preProcessor::closeFile()
{
	fin.close();
	fout.close();
}
