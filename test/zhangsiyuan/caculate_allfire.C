using namespace std;
void caculate_allfire()
{
	double energy=0;
	double wave_1MeV=17612;
	double storage_1wave=2;
	double frequency=0;
	double time_min=0;
	//double time_s=time_min*60;
	double point=1;
	cout << "input"<<endl;
	cout << "energy:"<<endl;
	cin >> energy;
	cout << "frequency:"<<endl;
	cin >>frequency;
	cout << "time_min:" <<endl;
	cin >> time_min;
	//cout << "point:" <<endl;
	//cin >> point;
	cout <<"energy="<<energy<<";frequency="<<frequency<<";data_taking_time_min="<<time_min<<endl;
//";point="<<point<<endl;
	double time_s=time_min*60;
	double datasize_kB=0;
	double datasize_TB=0;
	datasize_kB=energy*wave_1MeV*frequency*storage_1wave*time_s*point;
	datasize_TB=datasize_kB/1000000000;
	cout << "datasize=" << datasize_TB << "TB"<<endl;
	
}


