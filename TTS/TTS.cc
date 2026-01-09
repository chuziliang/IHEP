
TString path("./TTS");

const int Length = 1;

const int V = 1468;


Double_t waveFit(Double_t* x, Double_t* par){
    Double_t xx = x[0];
    Double_t f1 = par[0] + par[1] * TMath::Exp(-0.5 * pow(TMath::Log((xx - par[2]) / par[3]) / par[4], 2));
    Double_t f2 = par[5] * TMath::Exp(-0.5 * pow((xx - par[6]) / par[7], 2));
    Double_t f3 = par[8] * 1.0 / (TMath::Exp((par[9] - xx) / 10.0) + 1) * TMath::Exp(-xx / par[10]);
    return f1 - f2 - f3;
}

Double_t wgaus(Double_t* x, Double_t* par){
	Double_t xx = x[0];
	Double_t f1, f2;
	if(xx < par[0])
		f1 = TMath::Exp(-0.5 * pow(xx - par[0], 2) / par[1]);
	else
		f2 = TMath::Exp(-0.5 * pow(xx - par[0], 2) / par[2]);
	return (f1 + f2) * par[3] + par[4];
}

void GetAmp() {
	TH1F* amp = new TH1F("Amplitude", "", 150, 0, 150);
	path = path +  Length + "m" + V + "V94587.txt";
	ifstream in(path);
	double tri, sig, nan;
	while (in.good()) {
		TH1F* gate = new TH1F("triGate", "", 1008, 0, 1008);
		TH1F* pulse = new TH1F("sig", "", 1008, 0, 1008);
		for (int i = 1; i <= 1008; i ++) {
			in >> sig >> tri >> nan >> nan;
			//tri *= 1000;
			//sig *= 1000;
			gate->SetBinContent(i, tri);
			pulse->SetBinContent(i, sig / 10);
		}
		double baseline = 0;
		for (int i = 400; i <= 500; i ++)
			baseline += pulse->GetBinContent(i);
		baseline /= 101;
		amp->Fill(baseline - pulse->GetBinContent(pulse->GetMinimumBin()));
    // cout << baseline - pulse->GetBinContent(pulse->GetMinimumBin()) << endl;
		gate->Delete();
		pulse->Delete();
	}
	amp->Draw();
}

void GetTTS(double thr) {
	double scale = 0.2;
	path = path +  Length + "m" + V + "V94465.txt";
	TH1F* tt = new TH1F("TT", "", 500, 0, 200);
	//TH1F* tt1 = new TH1F("TT1", "", 500, 200, 400);
	TH2D* waves = new TH2D("Waveforms", "", 1400, 0, 1400, 100, -5, 25);
	TH2D* partt = new TH2D("parameter0_tt", "", 200, 0, 2, 500, 0, 200);
	ifstream in(path);
	double tri, sig;
	int count = 0;
  int c = 0;
	double rl = 0, rh = 1400;
	bool saveFlag = true;
	while (in.good()) {
    cout << "==================== " << c << " ====================" << endl;
    c ++;
		TH1F* gate = new TH1F("TriggerGate", "", 1400, 0, 1400);
		TH1F* pulse = new TH1F("Signal", "", 1400, 0, 1400);
		for (int i = 0; i < 1400; i ++) {
			in >> sig >> tri;
			tri *= 1000;
			sig *= 1000;
			// tri /= 10;
			// sig /= 10;
			gate->SetBinContent(i + 1, tri);
			pulse->SetBinContent(i + 1, sig);
		}
		double baseline = 0;
		for (int i = 400; i <= 500; i ++)
			baseline += pulse->GetBinContent(i);
		baseline /= 101;
		double sigAmp = baseline - pulse->GetBinContent(pulse->GetMinimumBin());
		//double t1;
   		//for (int i = pulse->GetMinimumBin(); i > 0; i --) {
		//	if (baseline - pulse->GetBinContent(i) < 0.5*sigAmp) {
		//		t1 = i * scale;
		//		break;
		//	}
		//}
		if (sigAmp < thr) {
			gate->Delete();
		pulse->Delete();
			count ++;
			continue;
		}
		if (pulse->GetMinimumBin() < rl || pulse->GetMinimumBin() > rh) {
			delete gate;
			delete pulse;
			continue;
		}
		for (int i = 1; i <= 1400; i ++) {
			pulse->SetBinContent(i, baseline - pulse->GetBinContent(i));
			waves->Fill(pulse->GetBinCenter(i), pulse->GetBinContent(i));
		}
		TF1* func = new TF1("func", "TMath::Landau(x,[0],[1],0)*[2]+[3]", 0, 1400);
		func->SetParameters(pulse->GetMaximumBin(), sigAmp, 1, 0);
		pulse->Fit(func, "R");
		double error=func->GetParError(0);
		if (saveFlag && func->GetParameter(0) > rl && func->GetParameter(0) < rh) {
			auto cc = new TCanvas();
			cc->cd();
			pulse->Draw();
			saveFlag = false;
			cc->SaveAs("Wave.pdf");
			delete cc;
		}
		if (func->GetParameter(1) < 2 or error >10) {
			gate->Delete();
			pulse->Delete();
			delete func;
			count ++;
			continue;
		}
		double t = scale * func->GetParameter(0);
		baseline = 0;
		for (int i = 0; i <= 50; i ++) {
			baseline += gate->GetBinContent(i);
		}
		baseline /= 51;
		for (int i = 1; i <= 1400; i ++) {
			gate->SetBinContent(i, baseline - gate->GetBinContent(i));
			waves->Fill(gate->GetBinCenter(i), 0.01 * gate->GetBinContent(i));
		}
		// for (int i = 1; i <= 1400; i ++) {
		// 	if (pulse->GetBinContent(i) >= 200) {
		// 		t = i;
		// 		break;
		// 	}
		// }
		int gatePos = gate->GetMaximumBin();
		double tmp = gate->GetBinContent(gatePos);
		cout << "Trigger amplitude: " << tmp << endl;
		for (int i = 1; i <= 1400; i ++) {
			double tmpAmp = gate->GetBinContent(i);
			if (tmpAmp >= 200) {
				t -= scale * (double)i;
		//		t1 -= scale * (double)i;
				partt->Fill(func->GetParError(0), t);
				break;
			}
		}
		// t -= gate->GetMinimumBin() * scale;
		cout << "============================== TT: " << t << " ==============================" << endl;
		tt->Fill(t);
		//tt1->Fill(t1);
		gate->Delete();
		pulse->Delete();
		delete func;
		cout << "**************************** " << count << " ******************************" << endl;
	}
	double unit = 0.4;
	TF1* gaus = new TF1("ttFit", "gaus", 0, 1400);
	gaus->SetParameters(tt->GetBinContent(tt->GetMaximumBin()), tt->GetMaximumBin() * unit, tt->GetRMS());
	auto c1 = new TCanvas();
	tt->Fit(gaus, "R");
	//tt1->Fit(gaus, "R");
	double tts = gaus->GetParameter(2);
	double ttsEr = gaus->GetParError(2);
	cout << "============================== TTS: " << tts << " ==============================" << endl;
	cout << "============================== TTS error: " << ttsEr << " ==============================" << endl;
	tt->Draw();
	auto c2 = new TCanvas();
	waves->Draw("colz");
	auto c3 = new TCanvas();
	partt->DrawClone("colz");
	//auto c4 = new TCanvas();
	//tt1->DrawClone("colz");
	// TFile* out = new TFile("./outTTS.root", "recreate");
	// out->cd();
	// tt->Write();
	// out->Close();
	in.close();
	TFile* fout = new TFile(path + ".root", "recreate");
	fout->cd();
	tt->Write();
	//tt1->Write();
	waves->Write();
	partt->Write();
	fout->Close();
}

void GetTTSDT(double thr) {
	double scale = 1;
	path = path +  Length + "m" + V + "v94465.txt";
	TH1F* tt = new TH1F("TT", "", 500, 200, 400);
	TH1F* rise = new TH1F("rise", "", 100, 0, 10);
	TH1F* fall = new TH1F("fall", "", 100, 0, 50);
	TH1F* mid = new TH1F("mid", "", 100, 0, 50);
	//TH1F* tt1 = new TH1F("TT1", "", 500, 200, 400);
	TH2D* waves = new TH2D("Waveforms", "", 1008, 0, 1008, 100, -5, 25);
	TH2D* partt = new TH2D("parameter0_tt", "", 200, 0, 2, 500, 200, 400);
	ifstream in(path);
	double tri, sig, nan;
	int count = 0;
  int c = 0;
	double rl = 500, rh = 700;
	bool saveFlag = true;
	while (in.good()) {
    cout << "==================== " << c << " ====================" << endl;
    c ++;
		TH1F* gate = new TH1F("TriggerGate", "", 1008, 0, 1008);
		TH1F* pulse = new TH1F("Signal", "", 1008, 0, 1008);
		for (int i = 0; i < 1008; i ++) {
			in >> sig >> tri >> nan >> nan;
			// tri /= 10;
			sig /= 10;
			gate->SetBinContent(i + 1, tri);
			pulse->SetBinContent(i + 1, sig);
		}
		double baseline = 0;
		for (int i = 480;i <= 500; i ++)
			baseline += pulse->GetBinContent(i);
		baseline /= 21;
		double sigAmp = baseline - pulse->GetBinContent(pulse->GetMinimumBin());
		//cout << pulse->GetMinimumBin() << endl;
		//double t1;
   		//for (int i = pulse->GetMinimumBin(); i > 0; i --) {
		//	if (baseline - pulse->GetBinContent(i) < 0.5*sigAmp) {
		//		t1 = i * scale;
		//		break;
		//	}
		//}
		if (sigAmp < thr) {
			gate->Delete();
		pulse->Delete();
			count ++;
			continue;
		}
		if (pulse->GetMinimumBin() < rl || pulse->GetMinimumBin() > rh) {
			delete gate;
			delete pulse;
			continue;
		}
		for (int i = 1; i <= 1008; i ++) {
			pulse->SetBinContent(i, baseline - pulse->GetBinContent(i));
			waves->Fill(pulse->GetBinCenter(i), pulse->GetBinContent(i));
		}
		TF1* func = new TF1("func", "TMath::Landau(x,[0],[1],0)*[2]+[3]", pulse->GetMaximumBin() - 100, pulse->GetMaximumBin() + 100);
		func->SetParameters(pulse->GetMaximumBin(), sigAmp, 1, 0);
		pulse->Fit(func, "R");
		double error=func->GetParError(0);
		if (saveFlag && func->GetParameter(0) > rl && func->GetParameter(0) < rh) {
			auto cc = new TCanvas();
			cc->cd();
			pulse->Draw();
			saveFlag = false;
			cc->SaveAs("Wave.pdf");
			delete cc;
		}
		if (func->GetParameter(1) < 1 or error >10) {
			gate->Delete();
			pulse->Delete();
			delete func;
			count ++;
			continue;
		}
		double t = scale * func->GetParameter(0);
		double peak = func->GetParameter(0);
		double amp = func->GetParameter(1);
		double riseS = 0, riseE = 0, midS = 0;
		double riseTime, fallTime, FWHM;
		riseS = func->GetX(amp * 0.1, peak - 50, peak);
		riseE = func->GetX(amp * 0.9, peak - 50, peak);
		midS = func->GetX(amp * 0.5, peak - 50, peak);
		riseTime = (riseE - riseS) * scale;

		double fallS = 0, fallE = 0, midE = 0;
		fallE = func->GetX(amp * 0.1, peak, peak + 50);
		fallS = func->GetX(amp * 0.9, peak, peak + 50);
		midE = func->GetX(amp * 0.5, peak, peak + 50);
		fallTime = (fallE - fallS) * scale;
		FWHM = (midE - midS) * scale;
		baseline = 0;
		for (int i = 0; i <= 50; i ++) {
			baseline += gate->GetBinContent(i);
		}
		baseline /= 51;
		for (int i = 1; i <= 1008; i ++) {
			gate->SetBinContent(i, baseline - gate->GetBinContent(i));
			waves->Fill(gate->GetBinCenter(i), 0.01 * gate->GetBinContent(i));
		}
		// for (int i = 1; i <= 1008; i ++) {
		// 	if (pulse->GetBinContent(i) >= 200) {
		// 		t = i;
		// 		break;
		// 	}
		// }
		int gatePos = gate->GetMaximumBin();
		double tmp = gate->GetBinContent(gatePos);
		cout << "Trigger amplitude: " << tmp << endl;
		for (int i = 1; i <= 1008; i ++) {
			double tmpAmp = gate->GetBinContent(i);
			if (tmpAmp >= 200) {
				t -= scale * (double)i;
		//		t1 -= scale * (double)i;
				partt->Fill(func->GetParError(0), t);
				break;
			}
		}
		// t -= gate->GetMinimumBin() * scale;
		cout << "============================== TT: " << t << " ==============================" << endl;
		tt->Fill(t);
		rise->Fill(riseTime);
		fall->Fill(fallTime);
		mid->Fill(FWHM);
		//tt1->Fill(t1);
		gate->Delete();
		pulse->Delete();
		delete func;
		cout << "**************************** " << count << " ******************************" << endl;
	}
	double unit = 0.4;
	TF1* gaus = new TF1("ttFit", "gaus", tt->GetMaximumBin() * 0.4 - 8 + 200, tt->GetMaximumBin() * 0.4 + 3 + 200);
	gaus->SetParameters(tt->GetBinContent(tt->GetMaximumBin()), tt->GetMaximumBin() * unit, 2);
	auto c1 = new TCanvas();
	tt->Fit(gaus, "R");
	//tt1->Fit(gaus, "R");
	double tts = gaus->GetParameter(2);
	double ttsEr = gaus->GetParError(2);
	cout << "============================== TTS: " << tts << " ==============================" << endl;
	cout << "============================== TTS error: " << ttsEr << " ==============================" << endl;
	double risemean = rise->GetMean();
	double fallmean = fall->GetMean();
	double midmean = mid->GetMean();
	ofstream out("t_related", std::ios::app);
	out << Length << " " << V << " "
	    << gaus->GetParameter(2) << " " << gaus->GetParError(2) << " "
	    << risemean << " " << rise->GetMeanError() << " "
	    << fallmean << " " << fall->GetMeanError() << " "
	    << midmean << " " << mid->GetMeanError() << endl;
	out.close();
	tt->Draw();
	auto c2 = new TCanvas();
	waves->Draw("colz");
	auto c3 = new TCanvas();
	partt->DrawClone("colz");
	auto c4 = new TCanvas();
	rise->DrawClone("colz");
	auto c5 = new TCanvas();
	fall->DrawClone("colz");
	auto c6 = new TCanvas();
	mid->DrawClone("colz");
	//auto c4 = new TCanvas();
	//tt1->DrawClone("colz");
	// TFile* out = new TFile("./outTTS.root", "recreate");
	// out->cd();
	// tt->Write();
	// out->Close();
	in.close();
	TFile* fout = new TFile(path + ".root", "recreate");
	fout->cd();
	tt->Write();
	rise->Write();
	fall->Write();
	mid->Write();
	//tt1->Write();
	waves->Write();
	partt->Write();
	fout->Close();
}

void GetTTSfit(double thr) {
	double scale = 1;
	path = path +  Length + "m" + V + "V94587.txt";
	TH1F* tt = new TH1F("TT", "", 500, 200, 400);
	//TH1F* tt1 = new TH1F("TT1", "", 500, 200, 400);
	TH2D* waves = new TH2D("Waveforms", "", 1008, 0, 1008, 100, -5, 25);
	TH2D* partt = new TH2D("parameter0_tt", "", 200, 0, 2, 500, 200, 400);
	ifstream in(path);
	double tri, sig, nan;
	int count = 0;
  int c = 0;
	double rl = 0, rh = 1008;
	bool saveFlag = true;
	while (in.good()) {
    cout << "==================== " << c << " ====================" << endl;
    c ++;
		TH1F* gate = new TH1F("TriggerGate", "", 1008, 0, 1008);
		TH1F* pulse = new TH1F("Signal", "", 1008, 0, 1008);
		for (int i = 0; i < 1008; i ++) {
			in >> sig >> tri >> nan >> nan;
			// tri /= 10;
			// sig /= 10;
			gate->SetBinContent(i + 1, tri);
			pulse->SetBinContent(i + 1, sig);
		}
		double baseline = 0;
		for (int i = 480;i <= 500; i ++)
			baseline += pulse->GetBinContent(i);
		baseline /= 21;
		double sigAmp = baseline - pulse->GetBinContent(pulse->GetMinimumBin());
		//cout << pulse->GetMinimumBin() << endl;
		//double t1;
   		//for (int i = pulse->GetMinimumBin(); i > 0; i --) {
		//	if (baseline - pulse->GetBinContent(i) < 0.5*sigAmp) {
		//		t1 = i * scale;
		//		break;
		//	}
		//}
		if (sigAmp < thr) {
			gate->Delete();
		pulse->Delete();
			count ++;
			continue;
		}
		if (pulse->GetMinimumBin() < rl || pulse->GetMinimumBin() > rh) {
			delete gate;
			delete pulse;
			continue;
		}
		for (int i = 1; i <= 1008; i ++) {
			pulse->SetBinContent(i, baseline - pulse->GetBinContent(i));
			waves->Fill(pulse->GetBinCenter(i), pulse->GetBinContent(i));
		}
		TF1* func = new TF1("func", wgaus, pulse->GetMaximumBin() - 50, pulse->GetMaximumBin() + 50, 5);
                func->SetParameters(pulse->GetMaximumBin(), 2.54, 11.08, sigAmp, 0);
                func->SetParNames("T_{0}", "#sigma_{r}", "#sigma_{f}", "A", "c");
		pulse->Fit(func, "R");
		if (saveFlag && func->GetParameter(0) > rl && func->GetParameter(0) < rh && func->GetParError(0) < 1 && func->GetParError(3) < 1) {
			auto cc = new TCanvas();
			cc->cd();
			pulse->Draw();
			saveFlag = false;
			cc->SaveAs("Wave.pdf");
			delete cc;
		}
		if (func->GetParameter(3) < 1) {
			gate->Delete();
			pulse->Delete();
			delete func;
			count ++;
			continue;
		}
		double t = scale * func->GetParameter(0);
		baseline = 0;
		for (int i = 0; i <= 50; i ++) {
			baseline += gate->GetBinContent(i);
		}
		baseline /= 51;
		for (int i = 1; i <= 1008; i ++) {
			gate->SetBinContent(i, baseline - gate->GetBinContent(i));
			waves->Fill(gate->GetBinCenter(i), 0.01 * gate->GetBinContent(i));
		}
		// for (int i = 1; i <= 1008; i ++) {
		// 	if (pulse->GetBinContent(i) >= 200) {
		// 		t = i;
		// 		break;
		// 	}
		// }
		int gatePos = gate->GetMaximumBin();
		double tmp = gate->GetBinContent(gatePos);
		cout << "Trigger amplitude: " << tmp << endl;
		for (int i = 1; i <= 1008; i ++) {
			double tmpAmp = gate->GetBinContent(i);
			if (tmpAmp >= 200) {
				t -= scale * (double)i;
		//		t1 -= scale * (double)i;
				partt->Fill(func->GetParError(0), t);
				break;
			}
		}
		// t -= gate->GetMinimumBin() * scale;
		cout << "============================== TT: " << t << " ==============================" << endl;
		tt->Fill(t);
		//tt1->Fill(t1);
		gate->Delete();
		pulse->Delete();
		delete func;
		cout << "**************************** " << count << " ******************************" << endl;
	}
	double unit = 0.4;
	TF1* gaus = new TF1("ttFit", "gaus", tt->GetMaximumBin() - 10, tt->GetMaximumBin() + 10);
	gaus->SetParameters(tt->GetBinContent(tt->GetMaximumBin()), tt->GetMaximumBin() * unit, 2);
	auto c1 = new TCanvas();
	tt->Fit(gaus, "R");
	//tt1->Fit(gaus, "R");
	double tts = gaus->GetParameter(2);
	double ttsEr = gaus->GetParError(2);
	cout << "============================== TTS: " << tts << " ==============================" << endl;
	cout << "============================== TTS error: " << ttsEr << " ==============================" << endl;
	tt->Draw();
	auto c2 = new TCanvas();
	waves->Draw("colz");
	auto c3 = new TCanvas();
	partt->DrawClone("colz");
	//auto c4 = new TCanvas();
	//tt1->DrawClone("colz");
	// TFile* out = new TFile("./outTTS.root", "recreate");
	// out->cd();
	// tt->Write();
	// out->Close();
	in.close();
	TFile* fout = new TFile(path + ".root", "recreate");
	fout->cd();
	tt->Write();
	//tt1->Write();
	waves->Write();
	partt->Write();
	fout->Close();
}



void TTS() {
	cout << "============================== GetAmp() ==============================" << endl;
        //GetAmp();
	cout << "Enter 'GetTTS(threshold)'" << endl
	     << "or 'GetTTSDT(threshold)'" << endl
	     << "or 'GetTTSfit(threshold)'" << endl;
}
