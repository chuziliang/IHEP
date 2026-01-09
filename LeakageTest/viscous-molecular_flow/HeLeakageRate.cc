#define TotalTime 36000
void HeLeakageRate()
{
    double Ain[4] = {1E-7,1E-6,1E-5,1E-4};//  单位ATM*ml/s = 1E-1 Pa*m3/s  ATM*ml/s = 1e5Pa*1e-6m3/s = 1E-1 Pa*m3/s
    double Aout[4] = {1E-7,1E-6,1E-5,1E-4};// 单位ATM*ml/s
    double V[2] = {0.1,0.2};//单位ml
    double BarrelPressure[2] = {1.25,0};
    //double miu = 15; //粘滞系数，单位1e-6Pa*s，空气粘滞系数为18e-6Pa*s

    TFile *ofile = new TFile("HeLeakage.root","recreate");

    for (int nV = 0;nV<2;nV++)
    {
        for(int nBP = 1;nBP<2;nBP++)
        {
            for (int nin = 0; nin<4; nin++)
            {
                for (int nout = 0;nout<4;nout++)
                {

                    double MiddlePressure = 1;//双O圈间的总压力 压力单位：n倍ATM
                    double SF6Pressure = 0;//双O圈间SF6的压力 压力单位：n倍ATM
                    double leakage = 0;//每一步泄露的气体总量 单位ml*ATM
                    double SF6Leakage = 0;//每一步泄露的SF6气体量 单位ml*ATM
                    double MaxLeakage = 0;

                    TDatime T0(2003,01,01,00,00,00);
                    int X0 = T0.Convert();
                    gStyle->SetTimeOffset(X0);//设定时间坐标初始时间


                    TGraph *SF6Leakage2Time = new TGraph(TotalTime);
                    //SF6Leakage2Time->GetXaxis()->SetTimeDisplay(1);
                    SF6Leakage2Time->GetYaxis()->SetTitle("10E-1 Pa*m3/s");
                    TGraph *Leakage2Time = new TGraph(TotalTime);
                    //Leakage2Time->GetXaxis()->SetTimeDisplay(1);
                    Leakage2Time->GetYaxis()->SetTitle("10E-1 Pa*m3/s");
                    TGraph *MiddlePressure2Time = new TGraph(TotalTime);
                    MiddlePressure2Time->GetXaxis()->SetTimeDisplay(1);


                    cout<<TString::Format("Ain = %1.1e Pa*m3/s, Aout = %1.1e Pa*m3/s, Barrel Pressure = %1.3fMPa, Volume between rings = %1.1fmL",
                        Ain[nin]*1E-1,Aout[nout]*1E-1,0.1*BarrelPressure[nBP],V[nV])<<endl;

                    for (int i = 0; i < TotalTime; i++)
                    {
                        if (i>7200 && i<7800)
                        {
                            SF6Pressure += Ain[nin]*(1.0 - MiddlePressure*MiddlePressure)/V[nV];
                        }

                        MiddlePressure += Ain[nin]*(1.0 - MiddlePressure*MiddlePressure)/V[nV];
                        leakage = Aout[nout]*(MiddlePressure*MiddlePressure - 0.0);
                        SF6Leakage = (SF6Pressure/MiddlePressure)*leakage;
                        MiddlePressure -= leakage/V[nV];
                        SF6Pressure -= SF6Leakage/V[nV];

                        if (SF6Leakage > MaxLeakage)
                        {
                            MaxLeakage = SF6Leakage;
                        }

                        SF6Leakage2Time->SetPoint(i,i,SF6Leakage);
                        Leakage2Time->SetPoint(i,i,leakage);
                        MiddlePressure2Time->SetPoint(i,i,MiddlePressure);



                    }
                    
                    cout<<"Max Leakage rate = "<<MaxLeakage*0.1<<endl;

                    TCanvas *c1 = new TCanvas("c1","c1",1);
                    c1->SetName(TString::Format("Ai%1.0f_Ao%1.0f_P%1.2f_V%1.0f",
                        TMath::Log(Ain[nin]*1E-1)/TMath::Log(10),TMath::Log(Aout[nout]*1E-1)/TMath::Log(10),BarrelPressure[nBP],V[nV]));
                    for (Int_t i=0; i<SF6Leakage2Time->GetN(); i++)
                    {
                        SF6Leakage2Time->GetX()[i] *= 1.0/3600.0;
                    }
                    
                    for (Int_t i=0; i<SF6Leakage2Time->GetN(); i++)
                    {
                        SF6Leakage2Time->GetY()[i] *= 1E-1;
                    }
                    SF6Leakage2Time->GetYaxis()->SetTitle("Pa*m3/s");
                    //SF6Leakage2Time->SetTitle("Ain = 10E-8 Pa*m3/s, Aout = 10E-8 Pa*m3/s, Barrel Pressure = 0.125MPa");
                    SF6Leakage2Time->SetTitle(
                        TString::Format("Ain = %1.1e Pa*m3/s, Aout = %1.1e Pa*m3/s, Barrel Pressure = %1.3fMPa, Volume between rings = %1.1fmL",
                        Ain[nin]*1E-1,Aout[nout]*1E-1,0.1*BarrelPressure[nBP],V[nV]));
                    SF6Leakage2Time->GetXaxis()->SetTitle("hour");
                    SF6Leakage2Time->Draw();
                    c1->Update();

                    c1->Write();
                    c1->Close();
                    cout<<"-----------------------------------------------------------"<<endl;

                    TCanvas *c2 = new TCanvas("c2","c2",1);
                    c2->Divide(2,2);
                    c2->SetName(TString::Format("C2_Ai%1.0f_Ao%1.0f_P%1.2f_V%1.0f",
                        TMath::Log(Ain[nin]*1E-1)/TMath::Log(10),TMath::Log(Aout[nout]*1E-1)/TMath::Log(10),BarrelPressure[nBP],V[nV]));
                    c2->cd(1);
                    SF6Leakage2Time->SetTitle("SF6 Leakage rate");
                    SF6Leakage2Time->Draw();
                    c2->cd(2);
                    Leakage2Time->SetTitle("Total Leakage rate");
                    Leakage2Time->Draw();
                    c2->cd(3);
                    
                    MiddlePressure2Time->SetTitle("Middle Pressure");
                    MiddlePressure2Time->Draw();

                    c2->Write();
                    c2->Close();
                }
            }
        }
    }
    

}