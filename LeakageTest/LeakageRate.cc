#define TotalTime 72000   //单位
void LeakageRate()
{
    double Ain[3] = {1E-7,5E-7,1E-6};//  单位ATM*ml/s = 1E-1 Pa*m3/s  ATM*ml/s = 1e5Pa*1e-6m3/s = 1E-1 Pa*m3/s
    double Aout[3] = {1E-7,5E-7,1E-6};// 单位ATM*ml/s
    double V[2] = {0.1,0.2};//单位ml
    double BarrelPressure[2] = {1.25,1.5};
    //double miu = 15; //粘滞系数，单位1e-6Pa*s，空气粘滞系数为18e-6Pa*s

    TFile *ofile = new TFile("SF6LeakageAndConcentrationTest20220719.root","recreate");

    for (int nV = 1;nV<2;nV++)
    {
        for(int nBP = 0;nBP<1;nBP++)
        {
            for (int nin = 0; nin<3; nin++)
            {
                for (int nout = 0;nout<3;nout++)
                {

                    double MiddlePressure = 1;//双O圈间的总压力 压力单位：n倍ATM
                    double SF6Pressure = 0;//双O圈间SF6的压力 压力单位：n倍ATM
                    double leakage = 0;//每一步泄露的气体总量 单位ml*ATM
                    double SF6Leakage = 0;//每一步泄露的SF6气体量 单位ml*ATM
                    double OutSF6 = 0;// 单位ml = 1e-6m3 = PPM*m3
                    
                    bool P001PPMFlag = 1;
                    bool P01PPMFlag = 1;
                    bool P1PPMFlag = 1;
                    bool e10Flag = 1;
                    bool e9Flag = 1;
                    bool e8Flag = 1;

                    TDatime T0(2003,01,01,00,00,00);
                    int X0 = T0.Convert();
                    gStyle->SetTimeOffset(X0);
                    TGraph *SF6Leakage2Time = new TGraph(TotalTime);
                    //SF6Leakage2Time->GetXaxis()->SetTimeDisplay(1);
                    SF6Leakage2Time->GetYaxis()->SetTitle("10E-1 Pa*m3/s");
                    TGraph *Leakage2Time = new TGraph(TotalTime);
                    //Leakage2Time->GetXaxis()->SetTimeDisplay(1);
                    Leakage2Time->GetYaxis()->SetTitle("10E-1 Pa*m3/s");
                    TGraph *OutSF62Time = new TGraph(TotalTime);
                    //OutSF62Time->GetXaxis()->SetTimeDisplay(1);
                    OutSF62Time->GetYaxis()->SetTitle("PPM*m3");
                    TGraph *MiddlePressure2Time = new TGraph(TotalTime);
                    //MiddlePressure2Time->GetXaxis()->SetTimeDisplay(1);

                    cout<<TString::Format("Ain = %1.1e Pa*m3/s, Aout = %1.1e Pa*m3/s, Barrel Pressure = %1.3fMPa, Volume between rings = %1.1fmL",
                        Ain[nin]*1E-1,Aout[nout]*1E-1,0.1*BarrelPressure[nBP],V[nV])<<endl;

                    for (int i = 0; i < TotalTime; i++)
                    {
                        SF6Pressure += Ain[nin]*(BarrelPressure[nBP]*BarrelPressure[nBP] - MiddlePressure*MiddlePressure)/V[nV];
                        MiddlePressure += Ain[nin]*(BarrelPressure[nBP]*BarrelPressure[nBP] - MiddlePressure*MiddlePressure)/V[nV];
                        leakage = Aout[nout]*(MiddlePressure*MiddlePressure - 1);
                        SF6Leakage = (SF6Pressure/MiddlePressure)*leakage;
                        MiddlePressure -= leakage/V[nV];
                        SF6Pressure -= SF6Leakage/V[nV];
                        OutSF6 += SF6Leakage;

                       
                        SF6Leakage2Time->SetPoint(i,i,SF6Leakage);
                        Leakage2Time->SetPoint(i,i,leakage);
                        
                        
                        
                        OutSF62Time->SetPoint(i,i,OutSF6);
                        MiddlePressure2Time->SetPoint(i,i,MiddlePressure);

                        if (i == 7200)
                        {
                            cout<<"2 hour SF6 outside = "<<OutSF6<<" PPM*m3"<<endl;
                        }
                        if (i == 71999)
                        {
                            cout<<"20 hour SF6 outside = "<<OutSF6<<" PPM*m3"<<endl;
                        }
                        
                        

                        // if (SF6Leakage*1E-1 > 1e-10 && e10Flag)
                        // { 
                        //     cout<<"SF6 leakage > 1e-10 Pa*m3/s        time = "<<(float)i/3600<<"h"<<endl;
                        //     e10Flag = false;
                        // }
                        // if (SF6Leakage*1E-1 > 1e-9 && e9Flag)
                        // {
                        //     cout<<"SF6 leakage > 1e-9 Pa*m3/s         time = "<<(float)i/3600<<"h"<<endl;
                        //     e9Flag = false;
                        // }
                        // if (SF6Leakage*1E-1 > 1e-8 && e8Flag)
                        // {
                        //     cout<<"SF6 leakage > 1e-8 Pa*m3/s         time = "<<(float)i/3600<<"h"<<endl;
                        //     e8Flag = false;
                        // }

                        // if (OutSF6 > 0.001 && P001PPMFlag)
                        // {
                        //     cout<< "SF6 concentration > 0.001PPM*m3    time = "<<(float)i/3600<<"h"<<endl;
                        //     P001PPMFlag = false;
                        // }
                        // if (OutSF6 > 0.01 && P01PPMFlag)
                        // {
                        //     cout<< "SF6 concentration > 0.01PPM*m3     time = "<<(float)i/3600<<"h"<<endl;
                        //     P01PPMFlag = false;
                        // }
                        // if (OutSF6 > 0.1 && P1PPMFlag)
                        // {
                        //     cout<< "SF6 concentration > 0.1PPM*m3      time = "<<(float)i/3600<<"h"<<endl;
                        //     P1PPMFlag = false;
                        // }

                    }
                    TCanvas *c1 = new TCanvas("c1","c1",1);
                    c1->SetName(TString::Format("Ai%1.0f_Ao%1.0f_P%1.2f_V%1.0f",
                        TMath::Log(Ain[nin]*1E-1)/TMath::Log(10),TMath::Log(Aout[nout]*1E-1)/TMath::Log(10),BarrelPressure[nBP],V[nV]));

                    for (Int_t i=0; i<SF6Leakage2Time->GetN(); i++)
                    {
                        SF6Leakage2Time->GetY()[i] *= 1E-1;
                    }
                    for (Int_t i=0; i<Leakage2Time->GetN(); i++)
                    {
                        Leakage2Time->GetY()[i] *= 1E-1;
                    }
                    for (Int_t i=0; i<MiddlePressure2Time->GetN(); i++)
                    {
                        MiddlePressure2Time->GetY()[i] *= 1E-1;
                    }
                    MiddlePressure2Time->GetYaxis()->SetTitle("MPa");


                    SF6Leakage2Time->GetYaxis()->SetTitle("Pa*m3/s");
                    Leakage2Time->GetYaxis()->SetTitle("Pa*m3/s");
                    //SF6Leakage2Time->SetTitle("Ain = 10E-8 Pa*m3/s, Aout = 10E-8 Pa*m3/s, Barrel Pressure = 0.125MPa");
                    SF6Leakage2Time->SetTitle(
                        TString::Format("Ain = %1.1e Pa*m3/s, Aout = %1.1e Pa*m3/s, Barrel Pressure = %1.3fMPa, Volume between rings = %1.1fmL",
                        Ain[nin]*1E-1,Aout[nout]*1E-1,0.1*BarrelPressure[nBP],V[nV]));
                    SF6Leakage2Time->GetXaxis()->SetTitle("hour");
                    SF6Leakage2Time->Draw();
                    c1->Update();
                    Float_t rightmax = 1.1*OutSF6;
                    Float_t scale    = gPad->GetUymax()/rightmax;
                    OutSF62Time->SetLineColor(kRed);
                    // for (Int_t i=0; i<OutSF62Time->GetN(); i++)
                    // {
                    //     OutSF62Time->GetY()[i] *= scale;
                    // } 
                    OutSF62Time->Draw("same");
                    c1->Update();
                    TGaxis*axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
                                            gPad->GetUxmax(),gPad->GetUymax(),
                                            0,rightmax,510,"+L");
                    axis->SetLineColor(kRed);
                    axis->SetLabelColor(kRed);
                    axis->SetTitle("PPM*m3");
                    axis->SetTitleColor(kRed);
                    axis->Draw();

                    TLegend *legendangle =new TLegend(0.6,0.15,0.88,0.35);
                    legendangle->SetTextFont(72);
                    legendangle->SetTextSize(0.04);
                    legendangle->AddEntry(SF6Leakage2Time,"SF6 leakage rate","lpe");
                    legendangle->AddEntry(OutSF62Time,"SF6 concentration","lpe");
                    legendangle->Draw();
                    
                    c1->Write();

                    c1->Close();


                    cout<<"-----------------------------------------------------------"<<endl;

                    TCanvas *c2 = new TCanvas("c2","c2",1);
                    c2->Divide(2,2);
                    c2->SetName(TString::Format("C2_Ai%1.0f_Ao%1.0f_P%1.2f_V%1.0f",
                        TMath::Log(Ain[nin]*1E-1)/TMath::Log(10),TMath::Log(Aout[nout]*1E-1)/TMath::Log(10),BarrelPressure[nBP],V[nV]));
                    c2->cd(1);
                    SF6Leakage2Time->SetTitle("SF6 Leak rate");
                    SF6Leakage2Time->Draw();
                    c2->cd(2);
                    Leakage2Time->SetTitle("Total Leak rate");
                    Leakage2Time->GetXaxis()->SetTitle("hour");
                    Leakage2Time->Draw();
                    c2->cd(3);
                    OutSF62Time->SetTitle("SF6 concentration outside");
                    OutSF62Time->GetXaxis()->SetTitle("hour");
                    OutSF62Time->Draw();
                    c2->cd(4);
                    MiddlePressure2Time->SetTitle("Pressure between two O-rings");
                    MiddlePressure2Time->GetXaxis()->SetTitle("hour");
                    MiddlePressure2Time->Draw();

                    c2->Write();
                    c2->Close();
                }
            }
        }
    }
    

}