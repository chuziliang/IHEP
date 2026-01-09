//这个脚本用于产生不同PE数的输入信号，用于模拟CATIROC文章中注入的信号。
//这里用了半高全宽为10ns的梯形波来模拟，edge为5ns。
void TrapezoidalWaveGenerator()
{
	double Amplitude;

	TFile *output = new TFile ("TrapezoidalWave.root","recreate");

	for (int i = 0; i < 10; i++)
	{
		TString name;
		name = TString::Format("Signal %1.1fPE",0.1*(i+1));
		TH1 *signal = new TH1F(name,name,1008,0,1008);        
		Amplitude = 0.24*(i+1);
		signal->SetName(name);
		signal->SetTitle(name);
		for	(int j = 1; j <= 1008; j++)
		{
			if ( j <= 500)
			{
				signal->SetBinContent(j,0);
			}
			if (j > 500 && j <= 505)
			{
				signal->SetBinContent(j,(j-500)*Amplitude/5);
			}
			if (j > 505 && j <= 509)
			{
				signal->SetBinContent(j,Amplitude);
			}
			if (j > 509 && j <= 515)
			{
				signal->SetBinContent(j,(515 - j)*Amplitude/5);
			}
			if (j>515)
			{
				signal->SetBinContent(j,0);
			}
		}
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