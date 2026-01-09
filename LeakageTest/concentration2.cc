// 采用隐形差分方法，开放边界

#define D 0.9475e-5  //扩散系数，单位m3/s
#define TotalTime 86400  //一共多少秒
#define tbin 864000   //时间共多少bin
#define LeakageRate 1E-9  //Pa*m3/s
#define Length 0.5  //一共多少m
#define rbin 500  //长度共多少bin

void concentration2()
{
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

    double a[rbin];
    double b[rbin];
    double c[rbin];
    double d[rbin];
    double beta[rbin];
    double y[rbin];
    double l[rbin];

    for (int k = 1; k < rbin-1; k++ )
    {
        a[k] = 1.0/(k+1) - 1;
        b[k] = dh*dh/(dt*D) + 2;
        c[k] = -1.0/(k+1) - 1;
        Ci[k] = 0;//初始条件，浓度为0
        Ciminus[k] = 0;
    }
    a[0] = 0;
    b[0] = (-3+dh*dh/(dt*D))+(3.0/c[1])*a[1];
    c[0] = 6 +(3.0/c[1])*b[1];
    Ci[0] = 0;
    Ciminus[0] = 0;
    a[rbin-1] = (2+2.0/rbin) + (1.0/a[rbin-2])*b[rbin-2];
    b[rbin-1] = (dh*dh/(dt*D)-1-2.0/rbin) + (1.0/a[rbin-2])*c[rbin-2];
    c[rbin-1] = 0;
    Ci[rbin-1] = 0;
    Ciminus[rbin-1] = 0;

    beta[0] = b[0];

    for (int i = 1; i<rbin; i++)
    {
        l[i] = a[i]/beta[i-1];
        beta[i] = b[i] - l[i]*c[i-1];
        Ci_min[i] = 0;
    }

    // // 打开文件用于写入数据
    // std::ofstream outfile("Ci_min_values.txt");
    // if (!outfile.is_open())
    // {
    //     std::cerr << "无法打开文件用于写入Ci_min数据" << std::endl;
    //     return;
    // }

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
                // 将当前步的Ci_min写入文件
                // for (int ri = 0; ri < rbin; ri++)
                // {
                //     outfile << std::setw(15) << Ci_min[ri];
                // }
                // outfile << std::endl;
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
            Ciminus[i] +=  LeakageRate*10*dt/((4.0*TMath::Pi()/3.0)*dh*dh*dh*8.0);//更新完数据之后，在中心的bin内加入溢出的SF6
        }

        for (int k = 1; k < rbin-1; k++ )
        {
            d[k] = dh*dh*Ciminus[k]/(dt*D);
            //if (ti == 0 || ti == 1 || ti == 2) cout<<"d["<<k<<"] = "<<d[k]<<endl;
        }
        d[0] = dh*dh*Ciminus[0]/(dt*D) + (3.0/c[1])*d[1];
        d[rbin-1] = dh*dh*Ciminus[rbin-1]/(dt*D) + (1.0/a[rbin-2])*d[rbin-2];

        y[0] = d[0];
        for (int i = 1; i<rbin; i++)
        {
            y[i] = d[i] - l[i]*y[i-1];
            //if (ti == 0 || ti == 1 || ti == 2) cout<<y[i]<<" = "<<d[i]<<" - "<<l[i]<<"*"<<y[i-1]<<endl;
        }

        Ci[rbin-1] = y[rbin-1]/beta[rbin-1];
        for(int i = rbin-2;i >= 0; i--)
        {
            Ci[i] = (y[i]- c[i]*Ci[i+1])/beta[i];
            //if (ti == 0 || ti == 1 || ti == 2) cout<<Ci[i]<<" = ("<<y[i]<<" - "<<c[i]<<"*"<<Ci[i+1]<<")/"<<beta[i]<<endl;
        }
        //if (ti == 0 || ti == 1 || ti == 2) cout<<"------------------------------------------------------------"<<endl;
       
    }

    // for (int ri = 0; ri < rbin; ri++ )
    // {
    //     con->SetBinContent(tbin,ri+1,Ciminus[ri]);
    // }

    TFile *output = TFile::Open("Concentration.root","recreate");
    con->Write("COLZ");

    // con->Draw("COLZ");
//-----------------------------2024年10日更新，增加验证----------------
	// TCanvas* c2 = new TCanvas("c2","c2",1);
	// c2->cd();
	Test_0_5cm->Write();
    Test_1cm->Write();

    // // 关闭文件
    // outfile.close();
}
