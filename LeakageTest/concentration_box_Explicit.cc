// 采用隐形差分方法，封闭边界
// 计算应该从r=1mm处开始，防止r=0处的奇点

#define D 0.9475e-8  //扩散系数，单位m3/s
#define TotalTime 8640  //一共多少秒
#define tbin 864000   //时间共多少bin
#define LeakageRate 1.0  //Pa*m3/s
#define Length 0.1  //一共多少m，+1mm
#define rbin 100  //长度共多少bin
#define LeakPointR 0.01 //计算开始的位置，漏孔的半径，这里为1mm

void concentration_box_Explicit()
{
    double TotalSF6_test = 0;//这一变量用于计算盒子内的总浓度，与释放的浓度做对比
    vector<double> TotalSF6Totime;//这一变量用于统计SF6浓度的变化
    vector<double> Time;//这一变量用于统计SF6浓度的变化
    vector<double> SF6IncreaseRatio;//这一变量用于统计SF6浓度的变化

    double Ci[rbin];//这一步的密度分布
    double Ciminus[rbin];//上一步的密度分布
    double dh = (double)Length/rbin;//单位m
    double dt = (double)TotalTime/tbin; //单位s
    double Ci_min[rbin];//每一分钟平均的密度分布
    
    TH2 *con = new TH2F("Concentration","SF_{6} concentration",TotalTime/60,0,TotalTime/3600,rbin,0,Length);
    con->GetXaxis()->SetTitle("Time(hour)");
    con->GetYaxis()->SetTitle("Distance to leak point(m)");
    con->GetZaxis()->SetTitle("SF_{6}(PPM)");

    TH1 *Test_1cm = new TH1F("test_1","SF_{6} concentration at 1cm",tbin,0,TotalTime/3600);
    Test_1cm->GetXaxis()->SetTitle("Time(hour)");
    Test_1cm->GetYaxis()->SetTitle("SF_{6}(PPM)");

    TH1 *Test_0_5cm = new TH1F("test_0_5","SF_{6} concentration at 0.5cm",tbin,0,TotalTime/3600);
    Test_0_5cm->GetXaxis()->SetTitle("Time(hour)");
    Test_0_5cm->GetYaxis()->SetTitle("SF_{6}(PPM)");

    
    for (int i = 1; i<rbin; i++)
    {
        Ci[i] = 0;
        Ciminus[i] = 0;
        Ci_min[i] = 0;
    }

    for (int ti = 0; ti < tbin; ti++)
    {
        for (int ri = 0; ri <rbin; ri++)//在每一秒开始之前，先更新这一秒的数据
        {
            Ci_min[ri] += Ci[ri];
            Ciminus[ri] = Ci[ri];
            Ci[ri] = 0;
            if (ti%(60*tbin/TotalTime) == 0)
            {
                Ci_min[ri] /= 60*tbin/TotalTime;
                con->SetBinContent(ti/(60*tbin/TotalTime)+1,ri+1,Ci_min[ri]);
                Ci_min[ri] = 0;
            }

            if (ri == 5 )
            {
                Test_0_5cm->SetBinContent(ti+1,Ciminus[ri]);
            }
            if (ri == 10 )
            {
                Test_1cm->SetBinContent(ti+1,Ciminus[ri]);
            }
            
            
        }
        for (int i = 0; i < 2; i++)
        {
            Ciminus[i] +=  LeakageRate*10*dt/((4.0*TMath::Pi()/3.0)*(pow(LeakPointR+2*dh,3)-pow(LeakPointR,3)));//更新完数据之后，在中心的bin内加入溢出的SF6
        }

        Ci[0] = D*dt*( ((-Ciminus[3]+4.0*Ciminus[2]-5.0*Ciminus[1]+2.0*Ciminus[0])/pow(dh,2)) + (-Ciminus[2]+4*Ciminus[1]-3*Ciminus[0])/(dh*LeakPointR) ) + Ciminus[0];
        for(int ri = 1;ri < rbin-1; ri++)
        {
            Ci[ri] = D*dt*( ((Ciminus[ri+1]-2.0*Ciminus[ri]+Ciminus[ri-1])/pow(dh,2)) + (Ciminus[ri+1]-Ciminus[ri-1])/(dh*(ri*dh+LeakPointR)) ) + Ciminus[ri];
        }
        Ci[rbin-1] = D*dt*( ((-Ciminus[rbin-4]+4.0*Ciminus[rbin-3]-5.0*Ciminus[rbin-2]+2.0*Ciminus[rbin-1])/pow(dh,2)) + (3*Ciminus[rbin-1]-4*Ciminus[rbin-2]+Ciminus[rbin-3])/(dh*((rbin-1)*dh+LeakPointR)) ) + Ciminus[rbin-1];
        
        TotalSF6_test = 0;
        for (int i = 0; i < rbin; i++)
        {
            TotalSF6_test += Ci[i]*0.1*(4.0*TMath::Pi()/3.0)*(pow(dh*(i+1)+LeakPointR,3)-pow(dh*(i)+LeakPointR,3));//这里需要将PPM转换为Pa*m3
        }
        TotalSF6Totime.push_back(TotalSF6_test);
        Time.push_back(dt*(ti+1));
        SF6IncreaseRatio.push_back((TotalSF6Totime[TotalSF6Totime.size()-1]-TotalSF6Totime[TotalSF6Totime.size()-2])/dt);
    }

    TGraph *Graph_TotalSF6Totime = new TGraph(Time.size(),&Time[0],&TotalSF6Totime[0]);
    TGraph *Graph_RatioTotime = new TGraph(Time.size(),&Time[0],&SF6IncreaseRatio[0]);

    TFile *output = TFile::Open("Concentration_box.root","recreate");
    con->Write("COLZ");

    // con->Draw("COLZ");
//-----------------------------2024年10日更新，增加验证----------------
	// TCanvas* c2 = new TCanvas("c2","c2",1);
	// c2->cd();
	Test_0_5cm->Write();
    Test_1cm->Write();
    Graph_TotalSF6Totime->Write();
    Graph_RatioTotime->Write();

    // // 关闭文件
    // outfile.close();
}
