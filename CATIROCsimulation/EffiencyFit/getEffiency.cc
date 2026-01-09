void getEffiency()
{
    TFile* ifile = new TFile("SCurve.root","read");
    TH1* Curve;
    Curve = (TH1*) ifile -> Get("S-curve");
    Curve->Draw();
    double EFF = 1.0;
    for (int i = 800; i > 0; i--)
	{
		if (EFF>Curve->GetBinContent(i))
		{
			cout<<"DACu = "<<i<<"\ttrigger effiency = "<<Curve->GetBinContent(i)<<endl;
			EFF -= 0.05;
		}
	}
}