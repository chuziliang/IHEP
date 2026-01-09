TString path("result.txt");

void fit()
{
	Double_t HHVL[3],HHVHV[3],HHVTTS[3],HHVeTTS[3],HHVrise[3],HHVerise[3],HHVfall[3],HHVefall[3],HHVFWHW[3],HHVeFWHW[3];
	Double_t LHVL[3],LHVHV[3],LHVTTS[3],LHVeTTS[3],LHVrise[3],LHVerise[3],LHVfall[3],LHVefall[3],LHVFWHW[3],LHVeFWHW[3];
	Double_t L1E7[3],HV1E7[3],TTS1E7[3],eTTS1E7[3],rise1E7[3],erise1E7[3],fall1E7[3],efall1E7[3],FWHW1E7[3],eFWHW1E7[3];
	Double_t L3E6[3],HV3E6[3],TTS3E6[3],eTTS3E6[3],rise3E6[3],erise3E6[3],fall3E6[3],efall3E6[3],FWHW3E6[3],eFWHW3E6[3];
	Double_t l,hv,TTS,eTTS,rise,erise,fall,efall,FWHW,eFWHW;
	ifstream infile;
	infile.open(path);
	if(infile.is_open())
	{
		for(int i = 0;i<3;i++)
		{
			infile>>HHVL[i]>>HHVHV[i]>>HHVTTS[i]>>HHVeTTS[i]>>HHVrise[i]>>HHVerise[i]>>HHVfall[i]>>HHVefall[i]>>HHVFWHW[i]>>HHVeFWHW[i];
		}
		for(int i = 0;i<3;i++)
		{
			infile>>LHVL[i]>>LHVHV[i]>>LHVTTS[i]>>LHVeTTS[i]>>LHVrise[i]>>LHVerise[i]>>LHVfall[i]>>LHVefall[i]>>LHVFWHW[i]>>LHVeFWHW[i];
		}
		for(int i = 0;i<3;i++)
		{
			infile>>L1E7[i]>>HV1E7[i]>>TTS1E7[i]>>eTTS1E7[i]>>rise1E7[i]>>erise1E7[i]>>fall1E7[i]>>efall1E7[i]>>FWHW1E7[i]>>eFWHW1E7[i];
		}
		for(int i = 0;i<3;i++)
		{
			infile>>L3E6[i]>>HV3E6[i]>>TTS3E6[i]>>eTTS3E6[i]>>rise3E6[i]>>erise3E6[i]>>fall3E6[i]>>efall3E6[i]>>FWHW3E6[i]>>eFWHW3E6[i];
			cout<<HV3E6[i]<<endl;
		}
	}
	infile.close();
	TCanvas *c1 = new TCanvas("c1");
	auto HHVTTSgr = new TGraphErrors(3,HHVL,HHVTTS,0,HHVeTTS);
	HHVTTSgr->SetLineColor(2);
	auto LHVTTSgr = new TGraphErrors(3,LHVL,LHVTTS,0,LHVeTTS);
	LHVTTSgr->SetLineColor(4);
	TMultiGraph *HVTTSgr = new TMultiGraph();
	HVTTSgr->Add(HHVTTSgr);
	HVTTSgr->Add(LHVTTSgr);
	HVTTSgr->SetTitle("HV TTS");
	HVTTSgr->Draw("ALP");
	TCanvas *c2 = new TCanvas("c2");
	auto TTS1E7gr = new TGraphErrors(3,L1E7,TTS1E7,0,eTTS1E7);
	TTS1E7gr->SetLineColor(2);
	auto TTS3E6gr = new TGraphErrors(3,L3E6,TTS3E6,0,eTTS3E6);
	TTS3E6gr->SetLineColor(4);
	TMultiGraph *GainTTSgr = new TMultiGraph();
	GainTTSgr->Add(TTS1E7gr);
	GainTTSgr->Add(TTS3E6gr);
	GainTTSgr->SetTitle("Gain TTS");
	GainTTSgr->Draw("ALP");
	TCanvas *c3 = new TCanvas("c3");
	auto HHVrisegr = new TGraphErrors(3,HHVL,HHVrise,0,HHVerise);
	HHVrisegr->SetLineColor(2);
	auto LHVrisegr = new TGraphErrors(3,LHVL,LHVrise,0,LHVerise);
	LHVrisegr->SetLineColor(4);
	TMultiGraph *HVrisegr = new TMultiGraph();
	HVrisegr->Add(HHVrisegr);
	HVrisegr->Add(LHVrisegr);
	HVrisegr->SetTitle("HV rise");
	HVrisegr->Draw("ALP");
	TCanvas *c4 = new TCanvas("c4");
	auto rise1E7gr = new TGraphErrors(3,L1E7,rise1E7,0,erise1E7);
	rise1E7gr->SetLineColor(2);
	auto rise3E6gr = new TGraphErrors(3,L3E6,rise3E6,0,erise3E6);
	rise3E6gr->SetLineColor(4);
	TMultiGraph *Gainrisegr = new TMultiGraph();
	Gainrisegr->Add(rise1E7gr);
	Gainrisegr->Add(rise3E6gr);
	Gainrisegr->SetTitle("Gain rise");
	Gainrisegr->Draw("ALP");
	TCanvas *c5 = new TCanvas("c5");
	auto HHVfallgr = new TGraphErrors(3,HHVL,HHVfall,0,HHVefall);
	HHVfallgr->SetLineColor(2);
	auto LHVfallgr = new TGraphErrors(3,LHVL,LHVfall,0,LHVefall);
	LHVfallgr->SetLineColor(4);
	TMultiGraph *HVfallgr = new TMultiGraph();
	HVfallgr->Add(HHVfallgr);
	HVfallgr->Add(LHVfallgr);
	HVfallgr->SetTitle("HV fall");
	HVfallgr->Draw("ALP");
	TCanvas *c6 = new TCanvas("c6");
	auto fall1E7gr = new TGraphErrors(3,L1E7,fall1E7,0,efall1E7);
	fall1E7gr->SetLineColor(2);
	auto fall3E6gr = new TGraphErrors(3,L3E6,fall3E6,0,efall3E6);
	fall3E6gr->SetLineColor(4);
	TMultiGraph *Gainfallgr = new TMultiGraph();
	Gainfallgr->Add(fall1E7gr);
	Gainfallgr->Add(fall3E6gr);
	Gainfallgr->SetTitle("Gain fall");
	Gainfallgr->Draw("ALP");
	TCanvas *c7 = new TCanvas("c7");
	auto HHVFWHWgr = new TGraphErrors(3,HHVL,HHVFWHW,0,HHVeFWHW);
	HHVFWHWgr->SetLineColor(2);
	auto LHVFWHWgr = new TGraphErrors(3,LHVL,LHVFWHW,0,LHVeFWHW);
	LHVFWHWgr->SetLineColor(4);
	TMultiGraph *HVFWHWgr = new TMultiGraph();
	HVFWHWgr->Add(HHVFWHWgr);
	HVFWHWgr->Add(LHVFWHWgr);
	HVFWHWgr->SetTitle("HV FWHW");
	HVFWHWgr->Draw("ALP");
	TCanvas *c8 = new TCanvas("c8");
	auto FWHW1E7gr = new TGraphErrors(3,L1E7,FWHW1E7,0,eFWHW1E7);
	FWHW1E7gr->SetLineColor(2);
	auto FWHW3E6gr = new TGraphErrors(3,L3E6,FWHW3E6,0,eFWHW3E6);
	FWHW3E6gr->SetLineColor(4);
	TMultiGraph *GainFWHWgr = new TMultiGraph();
	GainFWHWgr->Add(FWHW1E7gr);
	GainFWHWgr->Add(FWHW3E6gr);
	GainFWHWgr->SetTitle("Gain FWHW");
	GainFWHWgr->Draw("ALP");
}
