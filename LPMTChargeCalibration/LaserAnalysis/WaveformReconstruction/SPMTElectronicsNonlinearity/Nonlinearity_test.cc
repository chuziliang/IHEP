#include <set>
double MeanRatio = 1;


void Nonlinearity_test()
{
    int energy[]={1000,100,10,1500,150,1,200,20,2,300,30,3,4,500,50,5,6,700,7,8,9};
    int numbers[]={100,50,50,200,50,50,50,10,50,100,20,50,50,100,50,50,50,100,50,50,50};
    TH2 *output = new TH2F("Calib charge to true charge","Calib charge to true charge",200,0,200,2000,0,200);
    TH1 *CalibHist;
    TString histname = TString::Format("calib charge");
    CalibHist = new TH1F(histname,histname,1000,0,21.93); 
    std::set<double> unique_values;
    
    for (int i = 0; i < 3; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/SPMTElectronicsNonlinearity/%1dMevNonlinearity.root",energy[i]),"read");
        for (int j = 0; j < 10; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double TrueChargeSPMT;
            double CalibChargeSPMT; 
            if (!charge)
            {
                continue;
            }

            charge->SetBranchAddress("TrueCharge",&TrueChargeSPMT);
            charge->SetBranchAddress("CalibCharge",&CalibChargeSPMT);
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                if (CalibChargeSPMT ==0)
                {
                    continue;
                }
                unique_values.insert(CalibChargeSPMT);
                output->Fill(TrueChargeSPMT,CalibChargeSPMT);
                CalibHist->Fill(CalibChargeSPMT);
            }
            delete charge;
        }

        inputfile->Close();

        cout<<i<<" Finished."<<endl;
    }

        if (!unique_values.empty()) {
        cout<<"size of unique_values:"<<unique_values.size()<<endl;
        //将unique_values从小到大排列
        std::vector<double> sorted_values(unique_values.begin(), unique_values.end());
        std::sort(sorted_values.begin(), sorted_values.end());
        // 收集所有相邻差值
        std::set<double> diffs;
        auto it = unique_values.begin();
        double prev = *it;
        ++it;
        for (; it != unique_values.end(); ++it) {
            diffs.insert(*it - prev);
            // cout<<"sorted_values:"<<*it<<endl;
            // cout<<"diffs:"<<*it - prev<<endl;
            prev = *it;
        }
        cout<<"size of diffs:"<<diffs.size()<<endl;
        // cout<<"diffs:"<<diffs[0]<<endl;

        std::vector<double> sorted_diffs(diffs.begin(), diffs.end());
        std::sort(sorted_diffs.begin(), sorted_diffs.end());

        // 计算最小间隔
        const int scale = 10000; // 精度到小数点后6位
        std::vector<int> scaled_diffs;
        for (double d : sorted_diffs) {
            cout<<"sorted_diffs:"<<d<<endl;
            scaled_diffs.push_back(static_cast<int>(std::round(d * scale)));
        }

        cout<<"size of scaled_diffs:"<<scaled_diffs.size()<<endl;
        // cout<<"scaled_diffs:"<<scaled_diffs[0]<<endl;

        // 计算最大公约数
        auto gcd = [](int a, int b) {
            while (b != 0) {
                int temp = b;
                b = a % b;
                a = temp;
            }
            return a;
        };

        int base = scaled_diffs[0];
        for (size_t i = 1; i < scaled_diffs.size(); i++) {
            base = gcd(base, scaled_diffs[i]);
        }

        double min_interval = static_cast<double>(base) / scale;

        // 验证所有差值是否都是基础间隔的整数倍
        bool valid = true;
        for (int sd : scaled_diffs) {
            if (sd % base != 0) {
                valid = false;
                break;
            }
        }

        if (valid) {
            std::cout << "基础间隔: " << min_interval 
                     << " (" << (base/scale) << ")" << std::endl;
        } else {
            // 如果存在非整数倍情况，取最小实际差值
            auto min_it = std::min_element(diffs.begin(), diffs.end());
            std::cout << "最小实际间隔: " << *min_it << std::endl;
        }
    }
    
    CalibHist->Draw();
}