#include <TH1.h>
#include <TF1.h>
#include <TFile.h>
#include <fstream>
#include <iostream>
#include "math.h"
#include <TCanvas.h>
#include "TMath.h"
#include <vector>
#include "TF1.h"
#include "TString.h"

using namespace std;

double scale = 1.0;

Double_t mygaus(Double_t* x, Double_t* par) {
	Double_t xx = x[0];
	Double_t f = par[0] * exp(- 0.5 * ((xx - par[1]) / par[2]) * ((xx - par[1]) / par[2]));
	return f;
}

Double_t specFit(Double_t* x, Double_t* par) {
	double f1, f2, f3, f4, f5, f6;
	double f = 0;
	for (int n = 0; n <= 3; n ++) {
		f1 = (1 - par[5]) * exp(-0.5 * (x[0] - par[0] - n * par[2]) * (x[0] - par[0] - n * par[2]) / (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * TMath::Pi() * (par[1] * par[1] + n * par[3] * par[3])));
		f2 = 0.5 * par[4] * exp(-1 * par[4] * (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f3 = TMath::Erf(TMath::Abs(n * par[2] + par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f4 = TMath::Erf(TMath::Abs(x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f5 = TMath::Sign(1.0, (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f6 = TMath::Poisson(n, par[6]);
		f += f6 * (f1 + par[5] * (f2 * (f3 + f5 * f4)));
	}
	return f * par[7];
}

Double_t Pedestal(Double_t* x, Double_t* par) {
	double f1, f2, f3, f4, f5, f6;
	double f = 0;
	for (int n = 0; n < 1; n ++) {
		f1 = (1 - par[5]) * exp(-0.5 * (x[0] - par[0] - n * par[2]) * (x[0] - par[0] - n * par[2]) / (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * TMath::Pi() * (par[1] * par[1] + n * par[3] * par[3])));
		f2 = 0.5 * par[4] * exp(-1 * par[4] * (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f3 = TMath::Erf(TMath::Abs(n * par[2] + par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f4 = TMath::Erf(TMath::Abs(x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f5 = TMath::Sign(1.0, (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f6 = TMath::Poisson(n, par[6]);
		f += f6 * (f1 + par[5] * (f2 * (f3 + f5 * f4)));
	}
	return f * par[7];
}

Double_t SinglePeak(Double_t* x, Double_t* par) {
	double f1, f2, f3, f4, f5, f6;
	double f = 0;
	for (int n = 1; n < 2; n ++) {
		f1 = (1 - par[5]) * exp(-0.5 * (x[0] - par[0] - n * par[2]) * (x[0] - par[0] - n * par[2]) / (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * TMath::Pi() * (par[1] * par[1] + n * par[3] * par[3])));
		f2 = 0.5 * par[4] * exp(-1 * par[4] * (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f3 = TMath::Erf(TMath::Abs(n * par[2] + par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f4 = TMath::Erf(TMath::Abs(x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f5 = TMath::Sign(1.0, (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f6 = TMath::Poisson(n, par[6]);
		f += f6 * (f1 + par[5] * (f2 * (f3 + f5 * f4)));
	}
	return f * par[7];
}

Double_t DoublePeak(Double_t* x, Double_t* par) {
	double f1, f2, f3, f4, f5, f6;
	double f = 0;
	for (int n = 2; n < 3; n ++) {
		f1 = (1 - par[5]) * exp(-0.5 * (x[0] - par[0] - n * par[2]) * (x[0] - par[0] - n * par[2]) / (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * TMath::Pi() * (par[1] * par[1] + n * par[3] * par[3])));
		f2 = 0.5 * par[4] * exp(-1 * par[4] * (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f3 = TMath::Erf(TMath::Abs(n * par[2] + par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f4 = TMath::Erf(TMath::Abs(x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])) / (TMath::Sqrt(2 * (par[1] * par[1] + n * par[3] * par[3]))));
		f5 = TMath::Sign(1.0, (x[0] - par[0] - n * par[2] - par[4] * (par[1] * par[1] + n * par[3] * par[3])));
		f6 = TMath::Poisson(n, par[6]);
		f += f6 * (f1 + par[5] * (f2 * (f3 + f5 * f4)));
	}
	return f * par[7];
}

TH1F* getSpec(char **argv) {
	TString name(argv[1]);
	ifstream in(name);
	TH1F* spec = new TH1F("ChargeSpectrum", "", 1000, -1, 9);
	while (in.good()) {
		double charge = 0, baseline = 0;
		double ori, nan;
		TH1F* tmp = new TH1F("tmp", "", 1008, 0, 1008);
		for (int i = 0; i < 1008; i ++) {
			in >> ori >> nan >> nan >> nan;
			tmp->SetBinContent(i + 1, ori/10.0);
			// if (i > 500 and i <=530)
			// 	baseline += ori;
			// if (i < 550 || i >= 620)
			// 	continue;
			// if (i == 550)
			// 	baseline /= 30;
			// charge += (baseline - ori) * scale / 50;
			//cout << charge << endl;
		}
		for (int i = 580; i < 610; i ++) {
			baseline += tmp->GetBinContent(i);
		}
		baseline /= 30;
		for (int i = 1; i <= 1008; i ++) {
			tmp->SetBinContent(i, baseline - tmp->GetBinContent(i));
			if (i >= 630 && i < 730)
				charge += tmp->GetBinContent(i) * scale / 50;
		}
		// int peakPos = tmp->GetMaximumBin();
		// if (peakPos < 630 || peakPos >= 730) {
		// 	charge = 0;
		// 	for (int i = 530; i < 630; i ++) {
		// 		charge += tmp->GetBinContent(i) * scale / 50;
		// 	}
		// }
		spec->Fill(charge);
		delete tmp;
	}
	in.close();
	return spec;
}

int main(int argc, char **argv) {
	if (argc != 4) {
		cout << "Wrong number of arguments." << endl;
		return 0;
	}
	TString name(argv[1]);
	TH1F* spec = getSpec(argv);
	
	double rl, rh;
	sscanf(argv[2], "%lf", &rl);
	sscanf(argv[3], "%lf", &rh);
	double Q0 = (double)spec->GetMaximumBin() * 0.04 - 1;
	cout << "Maximum bin: " << Q0 << endl;
	
	Q0 = 0;

	TF1* func = new TF1("SpecModel", specFit, rl, rh, 8);
	func->SetParameters(Q0, 0.03, 0.8, 0.3, 0.034, -20, 0.5, 7);
	func->SetParNames("Q_{0}", "#sigma_{0}", "Q_{1}", "#sigma_{1}", "#alpha", "w", "u", "A");

	double para[8];

	spec->Draw();
	spec->Fit(func, "R");
	func->GetParameters(para);

	double valley = func->GetMinimum(para[0], para[2] + para[0]);
	double tmpX = para[0] + para[2];
	double peak = specFit(&tmpX, para);
	double PV = peak / valley;
	double res = para[3] / para[2];
	double gain = para[2] / 0.16;

	TF1* ped = new TF1("Pedestal", Pedestal, para[0] - 0.2, para[0] + 1, 8);
	cout << "gain: " << gain << endl;
	cout <<"gain error: " << func->GetParError(2) / 0.16 << endl;
	ped->SetParameters(para);
	ped->Draw("same");

	TF1* single = new TF1("Single", SinglePeak, para[0] + para[2] - 1, para[0] + para[2] + 1, 8);
	single->SetParameters(para);
	single->Draw("same");

	TF1* dou = new TF1("Double", DoublePeak, para[0] + 2 * para[2] - 1, para[0] + 2 * para[2] + 1, 8);
	dou->SetParameters(para);
	dou->Draw("same");

	TFile* save = new TFile("./test.root", "recreate");
	save->cd();
	spec->Write();
	ped->Write();
	single->Write();
	dou->Write();
	save->Close();




	return 1;
/*
	while (in.good()) {
		in >> count;
		for (int i = 0; i < 15; i ++)
			in >> useless;
		if (count > count_peak) {
			count_peak = count;
		}
		if (!count_start && count > 0)
			count_start = pos;
		if (last_count && !count) 
			count_end = pos;
		last_count = count;
		pos ++;
	}
	in.close();
	in.open(path);
	TH1D* QSpec = new TH1D("ChargeSpecIn", "Charge Spectrum", count_end - count_start, 0, (count_end - count_start) * 25);
	TH1D* QOut = new TH1D("ChargeSpecOut", "Charge Spectrum", count_end - count_start, 0, (count_end - count_start) * 25);
	pos = 0;
	while (in.good()) {
		in >> count;
		for (int i = 0; i < 15; i ++)
			in >> useless;
		if (pos >= count_start && pos <= count_end)
			QSpec->SetBinContent(pos - count_start + 1, count);
		pos ++;
	}

	// in->GetObject("Charge Spectrum", QSpec);
	int pedestal = QSpec->GetMaximumBin();
	int peak = QSpec->GetBinContent(pedestal);
	int start = 0;
	for (int i = pedestal; i > 0; i --) {
		if (QSpec->GetBinContent(i) < peak * 0.02) {
			start = i;
			break;
		}
	}
	int HalWidth = pedestal - start;
	int end = pedestal + HalWidth * 0.9;
	TF1* GausFunc = new TF1("gaus", "gaus", start * 25, end * 25);
	QSpec->Fit(GausFunc, "R");
	Double_t pars[3];
	GausFunc->GetParameters(pars);
	const Double_t* PedParsEr = GausFunc->GetParErrors();
	TH1D* test = new TH1D("test", "test", count_end - count_start, 0, (count_end - count_start) * 25);
	for (int i = 0; i < (count_end - count_start); i ++) {
		Double_t x[1];
		x[0] = i * 25;
		Double_t Spe = QSpec->GetBinContent(i) - mygaus(x, pars) < 0 ? 0 : QSpec->GetBinContent(i) - mygaus(x, pars);
		QOut->SetBinContent(i, Spe);
		// cout << "Debug: " << QSpec->GetBinContent(i) << "\t" << mygaus(x, pars) << endl;
		test->SetBinContent(i, mygaus(x, pars));
		// cout << "Debug: " << i << "\t" << mygaus(x, pars) << endl;
	}
	int Spe_pos = QOut->GetMaximumBin();
	int Spe_peak = QOut->GetBinContent(Spe_pos);
	if (Spe_pos < pedestal + HalWidth) {
		for (int i = Spe_pos; i > 0; i --) {
			if (QOut->GetBinContent(i) < Spe_peak * 0.1) {
				start = i;
				break;
			}
		}
		HalWidth = Spe_pos - start;
		end = Spe_pos + HalWidth;
		cout << start << "\t" << end << endl;
		TF1* tmp = new TF1("tmp_gaus", "gaus", start * 25, end * 25);
		QOut->Fit(tmp, "R");
		Double_t tmp_pars[3];
		tmp->GetParameters(tmp_pars);
		for (int i = 0; i < (count_end - count_start); i ++) {
			Double_t x[1];
			x[0] = i * 25;
			Double_t Spe = QOut->GetBinContent(i) - mygaus(x, tmp_pars);
			Spe = Spe < 0 ? 0 : Spe;
			QOut->SetBinContent(i, Spe);
		}
	}
	Spe_pos = QOut->GetMaximumBin();
	Spe_peak = QOut->GetBinContent(Spe_pos);
	for (int i = Spe_pos; i < (count_end - count_start); i ++) {
		if (QOut->GetBinContent(i) < Spe_peak * 0.2) {
			end = i;
			break;
		}
	}
	HalWidth = end - Spe_pos;
	start = Spe_pos - HalWidth * 0.5;
	TF1* SpeFunc = new TF1("Spe_gaus", "gaus", start * 25, end * 25);
	QOut->Fit(SpeFunc, "R");
	Double_t SpePars[3];
	SpeFunc->GetParameters(SpePars);
	const Double_t* SpeParsEr = SpeFunc->GetParErrors();
	Spe_pos = SpePars[1] / 25;
	TH1D* tmp = new TH1D("tmp", "tmp", Spe_pos - pedestal, 0, (Spe_pos - pedestal) * 25);
	for (int i = pedestal; i < Spe_pos; i ++) {
		tmp->SetBinContent(i - pedestal + 1, QSpec->GetBinContent(i));
	}
	double valley = tmp->GetBinContent(tmp->GetMinimumBin());
	// double peakV = QOut->GetBinContent(QOut->GetMaximumBin());
	double peakV = SpePars[0];
	double peakEr = SpeParsEr[0];
	Double_t valleyEr = tmp->GetBinError(tmp->GetMinimumBin());
	QOut->SetLineColor(kRed);
	QSpec->SetLineColor(kGreen);
	double Gain = (SpePars[1] - pars[1]) * 0.01 / 1.6;
	double GainEr = (SpeParsEr[1] + PedParsEr[1]) * 0.01 / 1.6;
	double PV = peakV / valley;
	double PVEr = peakEr / valley + valleyEr * PV / valley;
	double Resolution = SpePars[2] / (SpePars[1] - pars[1]);
	double ResEr = SpeParsEr[2] / (SpePars[1] - pars[1])\
					+ SpeParsEr[1] * SpePars[2] / (SpePars[1] - pars[1]) / (SpeParsEr[1] - pars[1])\
					+ PedParsEr[1] * SpePars[2] / (SpePars[1] - pars[1]) / (SpeParsEr[1] - pars[1]);	
	cout << "Gain: " << (SpePars[1] - pars[1]) * 0.01 / 1.6 << endl;
	cout << "P-V: " << peakV / valley << endl;
	cout << "Resolution: " << SpePars[2] / (SpePars[1] - pars[1]) << endl;
	outfile << name << "\t" << ratio << "\t"
			<< HV << "\t" << GainEr << "\t"
			<< PV << "\t" << PVEr << "\t"
			<< Resolution << "\t" << ResEr << endl;
	in.close();
	outfile.close();
	auto c = new TCanvas();
	c->cd();
	QSpec->Draw();
	// QOut->Draw("same");
	test->Draw("same");
	QOut->Draw("same");
	return 1;
*/
}
