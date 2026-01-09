// 采用隐形差分方法，封闭边界
// 计算应该从r=1mm处开始，防止r=0处的奇点

#define D 0.9475e-5  //扩散系数，单位m3/s
#define TotalTime 8640  //一共多少秒
#define tbin 8640   //时间共多少bin
#define LeakageRate 1.0  //Pa*m3/s
#define Length 0.1  //一共多少m，+1mm
#define rbin 10000   //长度共多少bin
#define LeakPointR 0.01 //计算开始的位置，漏孔的半径，这里为1mm

void concentration_box()
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

    double a[rbin];    //矩阵元
    double b[rbin];    //矩阵元
    double c[rbin];    //矩阵元
    double d[rbin];    //方程右侧的向量
    double beta[rbin]; //Gauss消去之后的矩阵元
    double y[rbin];    //Gauss消去之后右侧的向量
    double l[rbin];

    for (int k = 1; k < rbin-1; k++ )// 非边界区域的矩阵元
    {
        a[k] = 1.0/(k+(LeakPointR/dh)) - 1;
        b[k] = dh*dh/(dt*D) + 2;
        c[k] = -1.0/(k+(LeakPointR/dh)) - 1;
        Ci[k] = 0;//初始条件，浓度为0
        Ciminus[k] = 0;
    }

    // 下面确定边界处的矩阵元
    double a0, b0, c0; //r=LeakPointR处边界的矩阵元，需要消去a0，化为三对角矩阵。改变边界条件时，需要改变这些参数。
    // 这里是使用积分插值法确定的插值。
    // a0 = (pow(LeakPointR+dh,2)/(2*dh)-pow(LeakPointR,3)/(3.0*dt*D));
    // b0 = pow(LeakPointR+dh,3)/(3*dt*D);
    // c0 = -pow(LeakPointR+dh,2)/(2*dh);

    // 这里是直接计算的插值，一阶导数为0
    a0 = - 1 + dh*dh/(dt*D);
    b0 =  2;
    c0 = - 1;

    double am, bm, cm; //r最大处边界的矩阵元，需要消去cm，化为三对角矩阵。改变边界条件时，需要改变这些参数。
    // 这里是假设开口时的插值
    am = -1;
    bm = (2+2.0*dh/((rbin-1)*dh + LeakPointR));
    cm = (dh*dh/(dt*D)-1-2.0*dh/((rbin-1)*dh + LeakPointR));

    // 这里假设一阶导数为0
    // am = - 1;
    // bm =  2;
    // cm = -1 + dh*dh/(dt*D);

    // a[0] = 0;
    // b[0] = (-3 + dh*dh/(dt*D) + 3*dh/LeakPointR)+((3.0-dh/LeakPointR)/c[1])*a[1];
    // c[0] = (6 - 4*dh/LeakPointR) +((3.0-dh/LeakPointR)/c[1])*b[1];
    a[0] = 0;
    b[0] = a0 - (c0/c[1])*a[1];
    c[0] = b0 - (c0/c[1])*b[1];
    Ci[0] = 0;
    Ciminus[0] = 0;
    // a[rbin-1] = (2+2.0*dh/((rbin-1)*dh + LeakPointR)) + (1.0/a[rbin-2])*b[rbin-2];
    // b[rbin-1] = (dh*dh/(dt*D)-1-2.0*dh/((rbin-1)*dh + LeakPointR)) + (1.0/a[rbin-2])*c[rbin-2];
    // c[rbin-1] = 0;
    a[rbin-1] = bm - (am/a[rbin-2])*b[rbin-2];
    b[rbin-1] = cm - (am/a[rbin-2])*c[rbin-2];
    c[rbin-1] = 0;
    Ci[rbin-1] = 0;
    Ciminus[rbin-1] = 0;

    //下面确定高斯消去之后的矩阵元
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
            Ciminus[i] +=  LeakageRate*10*dt/((4.0*TMath::Pi()/3.0)*(pow(LeakPointR+2*dh,3)-pow(LeakPointR,3)));//更新完数据之后，在中心的bin内加入溢出的SF6
        }

        for (int k = 1; k < rbin-1; k++ )
        {
            d[k] = dh*dh*Ciminus[k]/(dt*D);
            //if (ti == 0 || ti == 1 || ti == 2) cout<<"d["<<k<<"] = "<<d[k]<<endl;
        }
        d[0] = dh*dh*Ciminus[0]/(dt*D) - (c0/c[1])*d[1]; //改变边界条件时，需要改变这些参数。
        // d[0] = pow(LeakPointR+dh,3)*Ciminus[1]/(3*dt*D) - pow(LeakPointR,3)*Ciminus[0]/(3*dt*D) + D*LeakageRate*10/(4.0*TMath::Pi()) - (c0/c[1])*d[1]; //改变边界条件时，需要改变这些参数。
        // d[0] = pow(LeakPointR+dh,3)*Ciminus[1]/(3*dt*D) - pow(LeakPointR,3)*Ciminus[0]/(3*dt*D) - (c0/c[1])*d[1]; //改变边界条件时，需要改变这些参数。这里是使用积分插值法得到的结果

        d[rbin-1] = dh*dh*Ciminus[rbin-1]/(dt*D) - (am/a[rbin-2])*d[rbin-2]; //改变边界条件时，需要改变这些参数。

        y[0] = d[0];
        for (int i = 1; i<rbin; i++)
        {
            y[i] = d[i] - l[i]*y[i-1];
            //if (ti == 0 || ti == 1 || ti == 2) cout<<y[i]<<" = "<<d[i]<<" - "<<l[i]<<"*"<<y[i-1]<<endl;
        }

        Ci[rbin-1] = y[rbin-1]/beta[rbin-1];
        // Ci[rbin-2] = Ci[rbin-1];
        for(int i = rbin-2;i >= 0; i--)
        {
            Ci[i] = (y[i]- c[i]*Ci[i+1])/beta[i];
            //if (ti == 0 || ti == 1 || ti == 2) cout<<Ci[i]<<" = ("<<y[i]<<" - "<<c[i]<<"*"<<Ci[i+1]<<")/"<<beta[i]<<endl;
        }
        //if (ti == 0 || ti == 1 || ti == 2) cout<<"------------------------------------------------------------"<<endl;
        // for (int i = 0; i < 2; i++)
        // {
        //     Ci[i] +=  LeakageRate*10*dt/((4.0*TMath::Pi()/3.0)*(pow(LeakPointR+2*dh,3)-pow(LeakPointR,3)));//更新完数据之后，在中心的bin内加入溢出的SF6
        // }
        
        TotalSF6_test = 0;
        for (int i = 0; i < rbin; i++)
        {
            TotalSF6_test += Ci[i]*0.1*(4.0*TMath::Pi()/3.0)*(pow(dh*(i+1)+LeakPointR,3)-pow(dh*(i)+LeakPointR,3));//这里需要将PPM转换为Pa*m3
        }
        TotalSF6Totime.push_back(TotalSF6_test);
        Time.push_back(dt*(ti+1));
        SF6IncreaseRatio.push_back((TotalSF6Totime[TotalSF6Totime.size()-1]-TotalSF6Totime[TotalSF6Totime.size()-2])/dt);
    }

    // for (int ri = 0; ri < rbin; ri++ )
    // {
    //     con->SetBinContent(tbin,ri+1,Ciminus[ri]);
    // }
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
