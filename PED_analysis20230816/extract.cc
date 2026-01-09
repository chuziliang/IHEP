#include <iostream>
#include <string>
#include <fstream>
#include <memory>

#include <TCanvas.h>
#include <TTree.h>
#include <TFile.h>
#include <TString.h>
#include <TH1F.h>
#include <TLegend.h>
#include <TLine.h>
#include <TF1.h>

using namespace std;

void formatHist(TH1* h, int color) {
	h->SetTitle("");
	h->GetXaxis()->SetLabelSize(0.04);
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetLabelSize(0.04);
	h->GetYaxis()->SetTitleSize(0.05);
	h->SetLineWidth(1.0);
	h->SetLineColor(color);
}

void formatCanvas(TCanvas* c) {
	c->SetTopMargin(0.1);
	c->SetBottomMargin(0.1);
	c->SetRightMargin(0.1);
	c->SetLeftMargin(0.1);
}

class extractor {
	public:
		extractor();
		~extractor();
		void setInput(string);
		double* extract();
	private:
		unique_ptr<TFile> myFile;
};

extractor::extractor() {}

extractor::~extractor() {}

void extractor::setInput(string path) {
	myFile.reset(new TFile(TString(path)));
}

double* extractor::extract() {
	double* data = new double[1024];

	TH1F* tmpHist;
	TF1* gaus;
	double max;
	for (int i = 0; i < 128; i ++) {
		tmpHist = (TH1F*)myFile->Get(Form("Histo_Charge_HG_%d_ping", i));
		max = tmpHist->GetBinCenter(tmpHist->GetMaximumBin());
		gaus = new TF1("f1", "gaus", max - 20, max + 20);
		gaus->SetParameters(tmpHist->GetMaximum(), max, 40);
		tmpHist->Fit(gaus, "R");
		data[i * 8] = gaus->GetParameter(1);
		data[i * 8 + 1] = gaus->GetParameter(2);
		tmpHist = (TH1F*)myFile->Get(Form("Histo_Charge_HG_%d_pong", i));
		tmpHist->Fit(gaus, "R");
		data[i * 8 + 2] = gaus->GetParameter(1);
		data[i * 8 + 3] = gaus->GetParameter(2);

		tmpHist = (TH1F*)myFile->Get(Form("Histo_Charge_LG_%d_ping", i));
		data[i * 8 + 4] = tmpHist->GetMean();
		data[i * 8 + 5] = tmpHist->GetRMS();
		tmpHist = (TH1F*)myFile->Get(Form("Histo_Charge_LG_%d_pong", i));
		data[i * 8 + 6] = tmpHist->GetMean();
		data[i * 8 + 7] = tmpHist->GetRMS();
		delete gaus;
		gaus = NULL;
	}
	myFile->Close();

	return data;
}

int main(int argc, char** argv) {
	ifstream list(argv[1]);
	TString outPath(argv[2]);
	int ABCCH;
	// sscanf(argv[3], "%d", &ABCCH);
	if (!list.is_open()) {
		clog << "Wrong file's name" << endl;
		return 0;
	}

	vector<string> filePaths;
	string path;
	while (list.good()) {
		list >> path;
		if (!list.good())
			break;
		filePaths.push_back(path);
	}
	list.close();
	extractor* ext = new extractor();

	const int nFile = filePaths.size();

	TFile* outputfile = TFile::Open("/junofs/users/chuziliang125/PED_analysis20230816/install_result.root","UPDATE");
	outputfile->mkdir("hgPingMean");
	outputfile->mkdir("hgPongMean");
	outputfile->mkdir("hgPingRMS");
	outputfile->mkdir("hgPongRMS");
	outputfile->mkdir("lgPingMean");
	outputfile->mkdir("lgPongMean");
	outputfile->mkdir("lgPingRMS");
	outputfile->mkdir("lgPongRMS");

	TH1F* hgPingMean[nFile];
	TH1F* hgPongMean[nFile];
	TH1F* hgPingRMS[nFile];
	TH1F* hgPongRMS[nFile];

	TH1F* lgPingMean[nFile];
	TH1F* lgPongMean[nFile];
	TH1F* lgPingRMS[nFile];
	TH1F* lgPongRMS[nFile];

	TLine* lHgMean[nFile][7];
	TLine* lHgRMS[nFile][7];
	TLine* lLgMean[nFile][7];
	TLine* lLgRMS[nFile][7];

	for (int i = 0; i < nFile; i ++) {
		string name = filePaths[i].substr(filePaths[i].find_last_of("/") + 1);
		name = filePaths[i].substr(filePaths[i].find_last_of("/") + 1, filePaths[i].find_last_of(".") - filePaths[i].find_last_of("/") - 1);
		clog << name << endl;
		hgPingMean[i] = new TH1F(TString("HG_Ping_Mean_") + TString(name), TString(name) + ";Channel;Mean / ADCu", 128, 0, 128);
		hgPongMean[i] = new TH1F(TString("HG_Pong_Mean_") + TString(name), TString(name) + ";Channel;Mean / ADCu", 128, 0, 128);
		hgPingRMS[i] = new TH1F(TString("HG_Ping_RMS_") + TString(name), TString(name) + ";Channel;Standard Deviation / ADCu", 128, 0, 128);
		hgPongRMS[i] = new TH1F(TString("HG_Pong_RMS_") + TString(name), TString(name) + ";Channel;Standard Deviation / ADCu", 128, 0, 128);

		lgPingMean[i] = new TH1F(TString("LG_Ping_Mean_") + TString(name), TString(name) + ";Channel;Mean / ADCu", 128, 0, 128);
		lgPongMean[i] = new TH1F(TString("LG_Pong_Mean_") + TString(name), TString(name) + ";Channel;Mean / ADCu", 128, 0, 128);
		lgPingRMS[i] = new TH1F(TString("LG_Ping_RMS_") + TString(name), TString(name) + ";Channel;Standard Deviation / ADCu", 128, 0, 128);
		lgPongRMS[i] = new TH1F(TString("LG_Pong_RMS_") + TString(name), TString(name) + ";Channel;Standard Deviation / ADCu", 128, 0, 128);

		formatHist(hgPingMean[i], kBlue);
		formatHist(hgPingRMS[i], kBlue);
		formatHist(hgPongMean[i], kRed);
		formatHist(hgPongRMS[i], kRed);

		formatHist(lgPingMean[i], kBlue);
		formatHist(lgPingRMS[i], kBlue);
		formatHist(lgPongMean[i], kRed);
		formatHist(lgPongRMS[i], kRed);
	}
	// ofstream log(outPath + ".txt");
	TH1F* RMSs = new TH1F("", ";Std. deviiation / ADCu;Count", 20, 0, 10);
	for (int i = 0; i < nFile; i ++) {
		ext->setInput(filePaths[i]);
		double* tmpData = ext->extract();
		for (int j = 0; j < 1024; j ++) {
			/*
			if (!(j % 8) && (j / 8) >= (ABCCH * 16) && (j / 8) <= (ABCCH * 16 + 15)) {
				log << j / 8 << "\t"
					<< tmpData[j] << "\t"
					<< tmpData[j + 1] << "\t"
					<< tmpData[j + 2] << "\t"
					<< tmpData[j + 3] << "\t"
					<< tmpData[j + 4] << "\t"
					<< tmpData[j + 5] << "\t"
					<< tmpData[j + 6] << "\t"
					<< tmpData[j + 7] << endl;
			}
			*/
			switch (j % 8) {
				case 0:
					hgPingMean[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					break;
				case 1:
					hgPingRMS[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					RMSs->Fill(tmpData[j]);
					break;
				case 2:
					hgPongMean[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					break;
				case 3:
					hgPongRMS[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					break;
				case 4:
					lgPingMean[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					break;
				case 5:
					lgPingRMS[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					break;
				case 6:
					lgPongMean[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					break;
				case 7:
					lgPongRMS[i]->SetBinContent(j / 8 + 1, tmpData[j]);
					break;
				default:;
			}
		}
		delete[] tmpData;
	}
	auto cRMSs = new TCanvas();
	cRMSs->cd();
	RMSs->Draw();
	cRMSs->SaveAs(outPath + "/RMS.pdf");
	// log.close();

	double w = 6000;
	double h = 6000;
	auto cHgMean = new TCanvas("HG_Ping_Mean", "HG_Ping_Mean", w, h);
	auto cHgRMS = new TCanvas("HG_Ping_RMS", "HG_Ping_RMS", w, h);

	auto cLgMean = new TCanvas("LG_Ping_Mean", "LG_Ping_Mean", w, h);
	auto cLgRMS = new TCanvas("LG_Ping_RMS", "LG_Ping_RMS", w, h);

	cHgMean->Divide(4, 14);
	cHgRMS->Divide(4, 14);

	cLgMean->Divide(4, 14);
	cLgRMS->Divide(4, 14);

	double max, min;
	for (int i = 0; i < nFile; i ++) {
		cHgMean->cd(i + 1);
		min = hgPingMean[i]->GetMinimum();
		max = hgPingMean[i]->GetMaximum();
		hgPingMean[i]->GetYaxis()->SetRangeUser(min - 5, max + 5);
		hgPingMean[i]->Draw();
		hgPongMean[i]->Draw("same");
		for (int j = 0; j < 7; j ++) {
			lHgMean[i][j] = new TLine(16 * (j + 1), min - 5, 16 * (j + 1), max + 5);
			lHgMean[i][j]->SetLineColor(kBlack);
			lHgMean[i][j]->Draw("same");
		}

		cHgRMS->cd(i + 1);
		min = hgPingRMS[i]->GetMinimum();
		max = hgPingRMS[i]->GetMaximum();
		hgPingRMS[i]->GetYaxis()->SetRangeUser(min - 5, max + 5);
		hgPingRMS[i]->Draw();
		hgPongRMS[i]->Draw("same");
		for (int j = 0; j < 7; j ++) {
			lHgRMS[i][j] = new TLine(16 * (j + 1), min - 5, 16 * (j + 1), max + 5);
			lHgRMS[i][j]->SetLineColor(kBlack);
			lHgRMS[i][j]->Draw("same");
		}

		cLgMean->cd(i + 1);
		min = lgPingMean[i]->GetMinimum();
		max = lgPingMean[i]->GetMaximum();
		lgPingMean[i]->GetYaxis()->SetRangeUser(min - 5, max + 5);
		lgPingMean[i]->Draw();
		lgPongMean[i]->Draw("same");
		for (int j = 0; j < 7; j ++) {
			lLgMean[i][j] = new TLine(16 * (j + 1), min - 5, 16 * (j + 1), max + 5);
			lLgMean[i][j]->SetLineColor(kBlack);
			lLgMean[i][j]->Draw("same");
		}

		cLgRMS->cd(i + 1);
		min = lgPingRMS[i]->GetMinimum();
		max = lgPingRMS[i]->GetMaximum();
		lgPingRMS[i]->GetYaxis()->SetRangeUser(min - 5, max + 5);
		lgPingRMS[i]->Draw();
		lgPongRMS[i]->Draw("same");
		for (int j = 0; j < 7; j ++) {
			lLgRMS[i][j] = new TLine(16 * (j + 1), min - 5, 16 * (j + 1), max + 5);
			lLgRMS[i][j]->SetLineColor(kBlack);
			lLgRMS[i][j]->Draw("same");
		}

		outputfile->cd("/hgPingMean");
		hgPingMean[i]->Write();
		outputfile->cd("/hgPongMean");
		hgPongMean[i]->Write();
		outputfile->cd("/hgPingRMS");
		hgPingRMS[i]->Write();
		outputfile->cd("/hgPongRMS");
		hgPingRMS[i]->Write();
		outputfile->cd("/lgPingMean");
		lgPingMean[i]->Write();
		outputfile->cd("/lgPongMean");
		lgPongMean[i]->Write();
		outputfile->cd("/lgPingRMS");
		lgPingRMS[i]->Write();
		outputfile->cd("/lgPongRMS");
		lgPongRMS[i]->Write();


	}

	outputfile->Close();

	cHgMean->SaveAs(outPath + "/HgMean.pdf");
	cHgRMS->SaveAs(outPath + "/HgRMS.pdf");
	cLgMean->SaveAs(outPath + "/LgMeam.pdf");
	cLgRMS->SaveAs(outPath + "/LgRMS.pdf");
	return 1;
}
