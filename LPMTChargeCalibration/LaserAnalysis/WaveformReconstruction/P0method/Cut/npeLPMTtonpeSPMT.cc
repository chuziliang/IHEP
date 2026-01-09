void npeLPMTtonpeSPMT()
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};
    int numbers[]={50,50,50,50,50,50,50,50,50,50,10,20,50,50,50,50,100,100,100,100,200};

    TH1 *LPMTHist[21];
    for (int i = 0; i < 21; i++)
    {
        TString histname = TString::Format("%1d MeV LPMT hist",energy[i]);
        LPMTHist[i] = new TH1F(histname,histname,2000,0,200); 
    }

    TH1 *SPMTHist[6][21];
    for (int id = 0; id < 6; id++)
    {
        for (int i = 0; i < 21; i++)
        {
            TString histname = TString::Format("%1d PE SPMT_%1d hist",energy[i],id);
            SPMTHist[id][i] = new TH1F(histname,histname,300,0,30); 
        }
    }

    TH1 *TrueHist[21]; //
    for (int i = 0; i < 21; i++) //
    {
        TString histname = TString::Format("%1d MeV LPMT ElecSim hist",energy[i]);
        TrueHist[i] = new TH1F(histname,histname,2000,0,200); 
    }

    TH2* TrueLPMTCharge2SPMTCharge[6];
    TH2* LPMTCharge2SPMTCharge[6];
    for (int i = 0; i < 6; i++)
    {
        TString histname = TString::Format("True LPMT Charge to SPMT_%1d Charge",i);
        TrueLPMTCharge2SPMTCharge[i] = new TH2F(histname,histname,100,0,10,100,0,200);
        histname = TString::Format("Waveform reconstruction LPMT Charge to SPMT_%1d Charge",i);
        LPMTCharge2SPMTCharge[i] = new TH2F(histname,histname,100,0,10,100,0,200);
    }

    for (int i = 0; i < 21; i++)
    {
        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/WaveformReconstruction/ChargeSpectrum/Cut/%1dMevCharge.root",energy[i]));
        if (!inputfile)
        {
            continue;
        }
        
        for (int j = 0; j < numbers[i]; j++)
        {
            TTree *charge = (TTree*)inputfile->Get(TString::Format("Charge%1d",j));
            double LPMTPE = 0;
            double SPMTPE[6] = {0,0,0,0,0,0};
            double TruePE = 0; //
            if (!charge)
            {
                continue;
            }
            charge->SetBranchAddress("LPMTCharge",&LPMTPE);
            charge->SetBranchAddress("SPMTCharge",SPMTPE);
            charge->SetBranchAddress("TrueCharge",&TruePE); //
           
            for (int k = 0; k < charge->GetEntries(); k++)
            {
                charge->GetEntry(k);
                LPMTHist[i]->Fill(LPMTPE);
                TrueHist[i]->Fill(TruePE); //
                for (int id = 0; id < 6; id++)
                {
                    SPMTHist[id][i]->Fill(SPMTPE[id]);
                    TrueLPMTCharge2SPMTCharge[id]->Fill(SPMTPE[id],TruePE);
                    LPMTCharge2SPMTCharge[id]->Fill(SPMTPE[id],LPMTPE);
                }
            }
            delete charge;
        }
        inputfile->Close();
        cout<<i<<" Finished."<<endl;
    }
    TFile *outputfile = TFile::Open("npeLPMTtonpeSPMT.root","RECREATE");
    for (int i = 0; i < 6; i++)
    {
        TrueLPMTCharge2SPMTCharge[i]->Write();
        LPMTCharge2SPMTCharge[i]->Write();
    }
    outputfile->mkdir("Spectrum");
    
    double LPMTMeanCharge[21];
    double ErrorLPMTMeanCharge[21];
    double TrueLPMTMeanCharge[21]; //
    double ErrorTrueLPMTMeanCharge[21]; //
    double SPMTMeanCharge[6][21];
    double ErrorSPMTMeanCharge[6][21];
    double Nonlinearity[21];
    double ErrorNonlinearity[21];
    int n = 0;

    for (int i = 0; i < 21; i++)
    {
        
        outputfile->cd("Spectrum");
        LPMTHist[i]->Write();
        TrueHist[i]->Write(); //
        for (int id = 0; id < 6; id++)
        {
            SPMTHist[id][i]->Write();
        }
        
        double entries = LPMTHist[i]->GetEntries();
        bool P0LargerThan0 = false;
        bool SelectHist = true;
        for (int id = 0; id < 6; id++)
        {
            if (SPMTHist[id][i]->GetBinContent(1) > 0 )
            {
                P0LargerThan0 = true;
            }
            else
            {
                P0LargerThan0 = false;
            }
            SelectHist &= P0LargerThan0;
        }
        
        if (entries > 1 && SelectHist)
        {
            LPMTMeanCharge[n] = LPMTHist[i]->GetMean();
            ErrorLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*LPMTHist[i]->GetMeanError();
            TrueLPMTMeanCharge[n] = TrueHist[i]->GetMean(); //
            ErrorTrueLPMTMeanCharge[n] = (sqrt(entries)/sqrt(entries - 1))*TrueHist[i]->GetMeanError(); //

            Nonlinearity[n] =  (LPMTMeanCharge[n] - TrueLPMTMeanCharge[n])/TrueLPMTMeanCharge[n];
            ErrorNonlinearity[n] = Nonlinearity[n]*sqrt(ErrorLPMTMeanCharge[n]*ErrorLPMTMeanCharge[n]/(LPMTMeanCharge[n]*LPMTMeanCharge[n]) 
                                                        + ErrorTrueLPMTMeanCharge[n]*ErrorTrueLPMTMeanCharge[n]/(TrueLPMTMeanCharge[n]*TrueLPMTMeanCharge[n]));

            for (int id = 0; id < 6; id++)
            {
                double P0 = (double)SPMTHist[id][i]->GetBinContent(1)/SPMTHist[id][i]->GetEntries();
                SPMTMeanCharge[id][n] = -TMath::Log(P0);
                double miuP0 = P0*(1 - P0)/SPMTHist[id][i]->GetEntries();
                miuP0 = sqrt(miuP0);
                ErrorSPMTMeanCharge[id][n] = miuP0/P0;
            }
            n++;
        }

    }

    
    TGraphErrors *npeLPMTtonpeSPMT[6];
    TCanvas *Canvas[6];
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",id);
        npeLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],LPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorLPMTMeanCharge);
        npeLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeLPMTtonpeSPMT[id]->SetName(GraphName);
        npeLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        npeLPMTtonpeSPMT[id]->Fit("pol1");
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);


        TString CanvasName = TString::Format("C%1d",id);
        Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        Canvas[id]->cd();
        gPad->SetLogx();
        gPad->SetLogy();
        npeLPMTtonpeSPMT[id]->Draw("AP");
        outputfile->cd();
        npeLPMTtonpeSPMT[id]->Write();
    }

    TGraphErrors *npeTrueLPMTtonpeSPMT[6]; //
    for (int id = 0; id < 6; id++)
    {
        TString GraphName = TString::Format("True LPMT npe to SPMT_%1d npe",id);
        npeTrueLPMTtonpeSPMT[id] = new TGraphErrors(n,SPMTMeanCharge[id],TrueLPMTMeanCharge,ErrorSPMTMeanCharge[id],ErrorTrueLPMTMeanCharge);
        npeTrueLPMTtonpeSPMT[id]->SetTitle(GraphName);
        npeTrueLPMTtonpeSPMT[id]->SetName(GraphName);
        npeTrueLPMTtonpeSPMT[id]->GetXaxis()->SetTitle("nPE SPMT");
        npeTrueLPMTtonpeSPMT[id]->GetYaxis()->SetTitle("nPE LPMT");
        npeTrueLPMTtonpeSPMT[id]->Fit("pol1");
        gStyle->SetOptFit(1111);
        gStyle->SetStatY(0.5);

        outputfile->cd();
        npeTrueLPMTtonpeSPMT[id]->Write();
    }

    TGraphErrors *NonlinearityGraph; //
    TString GraphName = TString::Format("Nonlinearity");
    NonlinearityGraph = new TGraphErrors(n,TrueLPMTMeanCharge,Nonlinearity,ErrorTrueLPMTMeanCharge,ErrorNonlinearity);
    NonlinearityGraph->SetTitle(GraphName);
    NonlinearityGraph->SetName(GraphName);
    NonlinearityGraph->GetXaxis()->SetTitle("True nPE LPMT from ElecSim");
    NonlinearityGraph->GetYaxis()->SetTitle("(WaveRecNPE - ElecSimNPE)/ElecSimNPE");
    NonlinearityGraph->SetFillColor(4);
    NonlinearityGraph->SetFillStyle(3002);
    NonlinearityGraph->SetMarkerColor(4);
    NonlinearityGraph->SetMarkerSize(0.8);
    NonlinearityGraph->SetMarkerStyle(20);
    NonlinearityGraph->Draw("a3");
    NonlinearityGraph->Draw("same L X P");

    outputfile->cd();
    NonlinearityGraph->Write();
    


    TString CanvasName = "c1";
    TCanvas *c1 = new TCanvas(CanvasName,CanvasName,1);
    c1->Divide(2,1);
    c1->Print("hist.pdf[","pdf");
    for (int i = 0; i < 6; i++)
    {
        c1->cd(1);
        gPad->SetLogz();
        TrueLPMTCharge2SPMTCharge[i]->GetXaxis()->SetTitle("SPMT nPE");
        TrueLPMTCharge2SPMTCharge[i]->GetYaxis()->SetTitle("LPMT nPE");
        TrueLPMTCharge2SPMTCharge[i]->GetYaxis()->SetLabelFont( 63  );
        TrueLPMTCharge2SPMTCharge[i]->GetYaxis()->SetLabelSize( 11 );
        //gStyle->SetOptStat(kFALSE);
        TrueLPMTCharge2SPMTCharge[i]->Draw("COLZ");
        c1->Update();
        TPaveStats* stats1 = (TPaveStats*)TrueLPMTCharge2SPMTCharge[i]->GetListOfFunctions()->FindObject("stats");
        stats1->SetX1NDC(0.67);
        stats1->SetX2NDC(0.87);
        c1->Update();
        TPaletteAxis *palette1 = (TPaletteAxis*)TrueLPMTCharge2SPMTCharge[i]->GetListOfFunctions()->FindObject("palette");
        if (palette1 != nullptr)
        {
            palette1->SetX1NDC(0.902);
            palette1->SetX2NDC(0.932);
            c1->Modified();
            c1->Update();
        }
        c1->cd(2);
        gPad->SetLogz();
        LPMTCharge2SPMTCharge[i]->GetXaxis()->SetTitle("SPMT nPE");
        LPMTCharge2SPMTCharge[i]->GetYaxis()->SetTitle("LPMT nPE");
        LPMTCharge2SPMTCharge[i]->GetYaxis()->SetLabelFont( 63  );
        LPMTCharge2SPMTCharge[i]->GetYaxis()->SetLabelSize( 11 );
        //gStyle->SetOptStat(kFALSE);
        LPMTCharge2SPMTCharge[i]->Draw("COLZ");
        c1->Update();
        TPaveStats* stats2 = (TPaveStats*)LPMTCharge2SPMTCharge[i]->GetListOfFunctions()->FindObject("stats");
        stats2->SetX1NDC(0.67);
        stats2->SetX2NDC(0.87);
        c1->Update();
        TPaletteAxis *palette2 = (TPaletteAxis*)LPMTCharge2SPMTCharge[i]->GetListOfFunctions()->FindObject("palette");
		if (palette2 != nullptr)
		{
			palette2->SetX1NDC(0.902);
			palette2->SetX2NDC(0.932);
			c1->Modified();
			c1->Update();
		}
        c1->Print("hist.pdf","pdf");
    }
    c1->Print("hist.pdf]","pdf");
}