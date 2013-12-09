#include "NNet.h"
#include "resultChecker.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

#define NUM_ATTR 30
#define TRAIN 0.7

vector<pair<double *, double> > data;

int main(int argc, char **argv) {
	if (argc != 6 && argc != 7) return 0;

	FILE *fdata = fopen(argv[1], "r");
	if (!fdata) return 1;

	FILE *fans = fopen(argv[2], "r");
	if (!fans) return 1;

	int max_iter = atoi(argv[4]), mode = atoi(argv[5]);

	nNet net;
	resultChecker check;

	double t;
	char s[111];
	int i;
	while (fscanf(fans, "%s", s) == 1) {
		if (*s == '?') continue;
		sscanf(s, "%lf", &t);
		data.push_back(make_pair(new double[NUM_ATTR], t));
		for (i = 0; i < NUM_ATTR; i++) {
			fscanf(fdata, "%lf", data.back().first + i);
		}
	}
	fclose(fdata);
	fclose(fans);

	int struc[] = {NUM_ATTR, 20, 6, 1}, layer = 4;
	net.initNode(layer, struc);
	net.setAlpha(0.05);
	check.setAcceptNErr(1, 0.5);
	if(argc == 6 || !net.loadWeight(argv[7])) net.initWeight(layer, struc);

	double maxx = 0;
	int train_num = (int) (data.size() * TRAIN), test_num = data.size() - train_num;
	double *test_rst = new double[test_num], *test_tar = new double[test_num];
	double co;
 	for (int j = 0; j < max_iter; j++) {
		random_shuffle(data.begin(), data.begin() + train_num);
		for(i = 0; i < train_num; i++) {
			net.learn(data[i].first, data[i].second);
		}
		for(i = train_num; (unsigned) i < data.size(); i++) {
			test_rst[i - train_num] = net.calOutput(data[i].first);
			test_tar[i - train_num] = data[i].second;
		}
		co = check.checkCorrectRate(test_rst, test_tar, test_num);
		if (maxx < co) maxx = co; 
//		printf("#%lf\n", co);
	}
	net.saveWeight(argv[3], maxx);
	printf("%lf\n", maxx);
	delete[] test_rst;
	delete[] test_tar;
}
