#include <iostream>
#include <TFile.h>
#include <TKey.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TString.h>

void CombineHistograms(const char* file1, const char* file2) {
    TFile* rootFile1 = TFile::Open(file1);
    TFile* rootFile2 = TFile::Open(file2);

    if (!rootFile1 || !rootFile2 || rootFile1->IsZombie() || rootFile2->IsZombie()) {
        std::cerr << "Error opening files: " << file1 << ", " << file2 << std::endl;
        return;
    }

    TCanvas* canvas = new TCanvas("canvas", "Combined Histograms", 800, 600);
    // TPad* pad1 = new TPad("pad1", "Histogram 1", 0.05, 0.1, 0.45, 0.9);
    // TPad* pad2 = new TPad("pad2", "Histogram 2", 0.55, 0.1, 0.95, 0.9);
    // pad1->Draw();
    // pad2->Draw();

    TIter nextkeyDir(rootFile1->GetListOfKeys());
    TKey* keyDir;
    // canvas->Print("HgMean.pdf[");
    // canvas->Print("HgRMS.pdf[");
    // canvas->Print("LgMean.pdf[");
    // canvas->Print("LgRMS.pdf[");
    while ((keyDir = (TKey*)nextkeyDir())) {
        TObject* obj1 = keyDir->ReadObj();
        if (obj1->IsA()->InheritsFrom("TDirectory")) {
            TDirectory* dir1 = (TDirectory*)obj1;
            TString dirName1(dir1->GetName());
            TDirectory* dir2 = (TDirectory*)rootFile2->Get(dirName1);
            if (!dir2) {
                std::cerr << "Directory " << dirName1 << " does not exist in " << file2 << std::endl;
                continue;
            }
            canvas->Print(dirName1+".pdf[");


            TIter nextkey1(dir1->GetListOfKeys());
            TKey* key1;
            while ((key1 = (TKey*)nextkey1())) {
                TIter nextkey2(dir2->GetListOfKeys());
                TKey* key2;
                while ((key2 = (TKey*)nextkey2()))
                {
                    TObject* objHist1 = key1->ReadObj();
                    TObject* objHist2 = key2->ReadObj(); 
                    if (objHist1->InheritsFrom("TH1") && objHist2->InheritsFrom("TH1")) {
                        TH1* hist1 = (TH1*)objHist1;
                        TH1* hist2 = (TH1*)objHist2;
                        TString histName1(hist1->GetName());
                        TString histName2(hist2->GetName());
                        TObjArray* tokens1 = histName1.Tokenize("_");
                        TObjArray* tokens2 = histName2.Tokenize("_");
                        TString prefix1;
                        TString prefix2;
                        for (Int_t i = 0; i < 4; ++i) {
                            TObjString* token1 = (TObjString*)tokens1->At(i);
                            prefix1.Append(token1->GetString());
                            TObjString* token2 = (TObjString*)tokens2->At(i);
                            prefix2.Append(token2->GetString());
                        }

                        if (prefix1 == prefix2) {

                            // pad1->cd();
                            // hist1->Draw();
                            // pad2->cd();
                            // hist2->Draw();
                            hist1->SetLineColor(kRed);
                            hist1->SetStats(0);
                            hist2->SetLineColor(kBlue);
                            hist2->SetStats(0);
                            hist1->SetTitle(prefix1);
                            
                            canvas->cd();
                            hist1->Draw();
                            hist2->Draw("same");

                            TLegend *legend = new TLegend(.75,.80,.95,.95);
                            legend->AddEntry(hist1,histName1);
                            legend->AddEntry(hist2,histName2);
                            legend->Draw();
                            canvas->Print(dirName1+".pdf");
                            //canvas->Print(Form("%s_%s.pdf", prefix1.Data(), histName1.Data()));
                        }
                    }

                    
                }
               
            }
            canvas->Print(dirName1+".pdf]");
        }
    }

    rootFile1->Close();
    rootFile2->Close();
}

int matiHist() {
    const char* file1 = "20230816result.root";
    const char* file2 = "install_result.root";
    CombineHistograms(file1, file2);

    return 0;
}