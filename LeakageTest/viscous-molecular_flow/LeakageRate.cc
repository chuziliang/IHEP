constexpr int TotalTime = 100000;//计算的总时间，单位 s
constexpr double miuSF6 = 1.53E-5;//SF6的粘度系数，单位 Pa*s
constexpr double miuAir = 1.81E-5;//空气的粘度系数，单位 Pa*s
constexpr double MAir = 28.96E-3;//空气的相对分子质量
constexpr double MSF6 = 146.055E-3;//SF6的相对分子质量
constexpr double Length = 2E-3;//漏孔长度（O圈直径），单位m  即2mm

//粘滞-分子流的漏率，均采用标准单位
double vmLeakRate(double ph, double pl, double d, double miu, double M, double L)//ph 高压压强；pl 低压压强；d 管道直径；miu 粘滞系数；M 气体摩尔质量；L 漏孔长度
{
    double R = 8.3143;//摩尔气体常数，8.3143J/(K*mol)
    double T = 300;//气体温度，K
    double pbar = (ph+pl)/2.0;
    double DeltaP = (ph-pl); 
    double pi = TMath::Pi();
    double ConstA = sqrt(2*pi*R*T/M);
    double ConstB = sqrt(M/(R*T));
    double LeakRate = DeltaP*(pi/128.0)*d*d*d*d*pbar/(miu*L)
                        + DeltaP*(ConstA/6.0)*(d*d*d/L)*((1+ConstB*d*pbar/miu)/(1+1.24*ConstB*d*pbar/miu));
    return LeakRate;
}
//计算SF6和空气混合的粘度系数
double viscosity(double SF6Pressure, double MiddlePressure)//气压单位任意
{
    double AirPressure = MiddlePressure - SF6Pressure;
    double MixViscosity;
    MixViscosity = miuSF6/(1.0+(AirPressure/SF6Pressure)*pow((1.0+sqrt(miuSF6/miuAir)*pow((MAir/MSF6),1.0/4.0)),2)/((4.0/sqrt(2.0))*sqrt(1+(MSF6/MAir))))
                 + miuAir/(1.0+(SF6Pressure/AirPressure)*pow((1.0+sqrt(miuAir/miuSF6)*pow((MSF6/MAir),1.0/4.0)),2)/((4.0/sqrt(2.0))*sqrt(1+(MSF6/MAir))));
    return MixViscosity;
}
//计算SF6和空气混合的摩尔质量
double MolecularWeight(double SF6Pressure, double MiddlePressure)//气压单位任意
{
    double AirPressure = MiddlePressure - SF6Pressure;
    double mass;
    mass = MSF6*SF6Pressure/MiddlePressure + MAir*AirPressure/MiddlePressure;
    return mass;
}

void LeakageRate()
{
    double Din[3] = {1E-6,1E-5,1.84635e-06};//  内层O圈直径，单位m
    double Dout[3] = {1E-6,1E-5,1.84635e-06};// 外层O圈直径，单位m
    double V[2] = {0.1,0.2};//单位ml
    double BarrelPressure[2] = {1.25E5,1.25E5};//UWB内SF6气压，单位Pa
    //double miu = 15; //粘滞系数，单位1e-6Pa*s，空气粘滞系数为18e-6Pa*s

    TFile *ofile = new TFile("SF6LeakageAndConcentrationMSFlow.root","recreate");

    for (int nV = 1;nV<2;nV++)
    {
        for(int nBP = 1;nBP<2;nBP++)
        {
            for (int nin = 2; nin<3; nin++)
            {
                for (int nout = 2;nout<3;nout++)
                {

                    double MiddlePressure = 1.0E5;//双O圈间的总压力 压力单位：Pa
                    double SF6Pressure = 0;//双O圈间SF6的压力 压力单位：Pa
                    double Airleakage = 0;//每一步泄漏的空气量 单位Pa*m3 = 1E6 Pa*ml
                    double SF6Leakage = 0;//每一步泄漏的SF6气体量 单位Pa*m3 = 1E6 Pa*ml
                    double OutSF6 = 0;// 泄漏到空气中的总SF6量 单位ml = 1e-6m3 = PPM*m3 = 1E-1 Pa*m3

                    //定义标准状态为漏孔两侧为真空和大气压的状态。
                    double Qinstand = 0;//标准状态下内层O圈空气漏率 单位Pa*m3/s
                    double Qoutstand = 0;//标准状态下外层O圈空气漏率 单位Pa*m3/s

                    Qinstand = (1341.0*0.5E10/2E-3)*Din[nin]*Din[nin]*Din[nin]*Din[nin]+(121.0*1E5/2E-3)*Din[nin]*Din[nin]*Din[nin]*((1+189*Din[nin]*0.5E5)/(1+234*Din[nin]*0.5E5));
                    // Qoutstand = (1341.0*0.5E10/2E-3)*Dout[nout]*Dout[nout]*Dout[nout]*Dout[nout]+(121.0*1E5/2E-3)*Dout[nout]*Dout[nout]*Dout[nout]*((1+189*Dout[nout]*0.5E5)/(1+234*Dout[nout]*0.5E5));
                    // Qinstand = vmLeakRate(1.0E5,0.0,Din[nin],miuAir,MAir,Length);
                    Qoutstand = vmLeakRate(1.0E5,0.0,Dout[nout],miuAir,MAir,Length);

                    
                    TGraph *SF6Leakage2Time = new TGraph(TotalTime);
                    SF6Leakage2Time->GetYaxis()->SetTitle("Pa*m3/s");
                    TGraph *Leakage2Time = new TGraph(TotalTime);
                    Leakage2Time->GetYaxis()->SetTitle("Pa*m3/s");
                    TGraph *OutSF62Time = new TGraph(TotalTime);
                    OutSF62Time->GetYaxis()->SetTitle("PPM*m3");
                    TGraph *MiddlePressure2Time = new TGraph(TotalTime);

                    cout<<TString::Format("Qin = %1.1e Pa*m3/s, Qout = %1.1e Pa*m3/s, Barrel Pressure = %1.3fMPa, Volume between rings = %1.1fmL",
                        Qinstand,Qoutstand,1E-6*BarrelPressure[nBP],V[nV])<<endl;

                    for (int i = 0; i < TotalTime; i++)
                    {
                        double SF6Leakin;//流入双O圈间的SF6量
                        SF6Leakin = vmLeakRate(BarrelPressure[nBP],MiddlePressure,Din[nin],miuSF6,MSF6,Length);//单位Pa*m3
                        SF6Pressure += 1.0E6*SF6Leakin/V[nV];
                        MiddlePressure += 1.0E6*SF6Leakin/V[nV];

                        double leakage;//流出双O圈间的气体量
                        leakage = vmLeakRate(MiddlePressure,1E5,Dout[nout],viscosity(SF6Pressure,MiddlePressure),MolecularWeight(SF6Pressure,MiddlePressure),Length);//单位Pa*m3
                        SF6Leakage = (SF6Pressure/MiddlePressure)*leakage;
                        MiddlePressure -= 1.0E6*leakage/V[nV];
                        SF6Pressure -= 1.0E6*SF6Leakage/V[nV];
                        OutSF6 += 10*SF6Leakage;//Pa*m3 = 10 PPM*m3

                       
                        SF6Leakage2Time->SetPoint(i,i,SF6Leakage);
                        Leakage2Time->SetPoint(i,i,leakage);
                        
                        
                        
                        OutSF62Time->SetPoint(i,i,OutSF6);
                        MiddlePressure2Time->SetPoint(i,i,MiddlePressure);

                        if (i == 72000)
                        {
                            cout<<"SF6 outside after 20 hours = "<<OutSF6<<" PPM*m3"<<endl;
                        }
                        

                    }
                    TCanvas *c1 = new TCanvas("c1","c1",1);
                    c1->SetName(TString::Format("Ai%1.0f_Ao%1.0f_P%1.2f_V%1.0f",
                        TMath::Log(Qinstand)/TMath::Log(10),TMath::Log(Qoutstand)/TMath::Log(10),1E-6*BarrelPressure[nBP],V[nV]));

                    // for (Int_t i=0; i<SF6Leakage2Time->GetN(); i++)
                    // {
                    //     SF6Leakage2Time->GetY()[i] *= 1E-1;
                    // }
                    // for (Int_t i=0; i<Leakage2Time->GetN(); i++)
                    // {
                    //     Leakage2Time->GetY()[i] *= 1E-1;
                    // }
                    // for (Int_t i=0; i<MiddlePressure2Time->GetN(); i++)
                    // {
                    //     MiddlePressure2Time->GetY()[i] *= 1E-1;
                    // }
                    // MiddlePressure2Time->GetYaxis()->SetTitle("MPa");


                    //SF6Leakage2Time->GetYaxis()->SetTitle("Pa*m3/s");
                    //Leakage2Time->GetYaxis()->SetTitle("Pa*m3/s");
                    //SF6Leakage2Time->SetTitle("Ain = 10E-8 Pa*m3/s, Aout = 10E-8 Pa*m3/s, Barrel Pressure = 0.125MPa");
                    SF6Leakage2Time->SetTitle(
                        TString::Format("Ain = %1.1e Pa*m3/s, Aout = %1.1e Pa*m3/s, Barrel Pressure = %1.3fMPa, Volume between rings = %1.1fmL",
                        Qinstand,Qoutstand,1E-6*BarrelPressure[nBP],V[nV]));
                    SF6Leakage2Time->GetXaxis()->SetTitle("s");
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
                        TMath::Log(Qinstand)/TMath::Log(10),TMath::Log(Qoutstand)/TMath::Log(10),1E-6*BarrelPressure[nBP],V[nV]));
                    c2->cd(1);
                    SF6Leakage2Time->SetTitle("SF6 Leak rate");
                    SF6Leakage2Time->Draw();
                    c2->cd(2);
                    Leakage2Time->SetTitle("Total Leak rate");
                    Leakage2Time->GetXaxis()->SetTitle("s");
                    Leakage2Time->Draw();
                    c2->cd(3);
                    OutSF62Time->SetTitle("SF6 concentration outside");
                    OutSF62Time->GetXaxis()->SetTitle("s");
                    OutSF62Time->Draw();
                    c2->cd(4);
                    MiddlePressure2Time->SetTitle("Pressure between two O-rings");
                    MiddlePressure2Time->GetXaxis()->SetTitle("s");
                    MiddlePressure2Time->Draw();

                    c2->Write();
                    c2->Close();
                }
            }
        }
    }
    

}