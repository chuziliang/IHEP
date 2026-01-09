#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;

int main(int argc, char** argv) {
	ifstream cntList("./cntList");
	ifstream cntInfo("./cntInfo");

	string cnt;
	double hv;
	map<string, double> cnt2hv;
	while (cntInfo.good()) {
		cntInfo >> cnt >> hv;
		if (!cntInfo.good())
			break;
		if (cnt[0] <= 57 && cnt[0] >= 48)
			cnt.insert(0, "Z");
		if (cnt[0] > 64)
			cnt[0] = 'Z';
		cnt2hv[cnt] = hv;
	}
	cntInfo.close();

	ofstream res("./result.txt");
	while (cntList.good()) {
		cntList >> cnt;
		if (!cntList.good())
			break;
		res << cnt << "\t" << cnt2hv[cnt] << endl;
		clog << cnt << "\t" << cnt2hv[cnt] << endl;
	}
	res.close();
	cntList.close();
}
