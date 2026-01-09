//这个脚本用于产生不同PE数的输入信号，用于模拟CATIROC文章中注入的信号。
//这里用了半高全宽为10ns的高斯函数来模拟，edge为7ns，高于文章中的5ns。
void GausSignalGenerator()
{
    double Amplitude;

    TFile *output = new TFile ("GausSignals.root","recreate");
    TF1 *GausFunc = new TF1("GausFunc","gaus",0,1008);

    for (int i = 0; i < 10; i++)
    {
        TString name;
        name = TString::Format("Signal %1.1fPE",0.1*(i+1));
        TH1 *signal = new TH1F(name,name,1008,0,1008);
        
        Amplitude = 3*0.0752*(i+1)*0.4;
        GausFunc->SetParameters(Amplitude,500,4.246);
        GausFunc->SetNpx(1008);
        signal = GausFunc->CreateHistogram();
        signal->SetName(name);
        signal->SetTitle(name);

        double charge = 0;
        double SignalMaxBin =signal->GetMaximumBin();

		for (int i = SignalMaxBin - 30; i < SignalMaxBin + 70; i++)
        {
            charge += signal->GetBinContent(i) / 50;
        }

        cout<<"charge of "<<name<<" is "<<charge<<endl;

        signal->Write();
        delete signal;
    }
}