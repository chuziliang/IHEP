//用于拟合不同LPMT nPE与SPMT nPE之间的对应关系。
const int Points = 21;

int GetCorrespondingSPMT(int (&CorrespondingSPMT)[17612][6])
{
    ifstream file("/junofs/users/chuziliang125/LPMTChargeCalibration/FindCorrespondingSPMT/PMTID_map.txt");
    string line;
    int currentLine = 0;
    int SPMT_ID[6];

    while (getline(file, line)) 
    { 
        stringstream ss(line);
        int LPMTID;
        ss >> LPMTID >> SPMT_ID[0] >> SPMT_ID[1] >> SPMT_ID[2] >> SPMT_ID[3] >> SPMT_ID[4] >> SPMT_ID[5];
        for (int i = 0; i < 6; i++)
        {
            CorrespondingSPMT[LPMTID][i] = SPMT_ID[i];
        }
    }

    file.close();
    return 0;
}

double GetSPMTQE(double (&SPMTQE)[25600])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_SPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("QE",&Effiency);
    for (int id = 0; id < 25600; id ++)
    {
        PMTData->GetEntry(id);
        SPMTQE[id] = Effiency;
    }
    
    // if (PMTID == SPMTID)
    // {
    //     inputfile->Close();
    //     return 100*Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong SPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

double GetLPMTQE(double (&LPMTQE)[17612])
{
    double Effiency;
    TFile *inputfile = TFile::Open("/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J22.1.0-rc4/data/Simulation/SimSvc/PMTSimParamSvc/PMTParam_CD_LPMT.root","read");
    TTree *PMTData = (TTree*)inputfile->Get("data");
    int PMTID;
    PMTData->SetBranchAddress("pmtID",&PMTID);
    PMTData->SetBranchAddress("PDE",&Effiency);
    for (int id = 0; id < 17612; id ++)
    {
        PMTData->GetEntry(id);
        LPMTQE[id] = Effiency/100.0;
    }
    
    // if (PMTID == LPMTID)
    // {
    //     inputfile->Close();
    //     return Effiency;
    // }
    // else
    // {
    //     cout<<"Wrong LPMTID!!!"<<endl;
    //     inputfile->Close();
    //     return 0;
    // }

    inputfile->Close();
    return 0;
}

double GetCalibFactor(int (&CorrespondingSPMT)[17612][6], double (&LPMTQE)[17612], double (&SPMTQE)[25600], TH1 *CalibFactorHist, TH1 *CalibFactorErrorHist, TH1 *ChiSquareHist, TFile *outputfile)
{
    int energy[]={1,2,3,4,5,6,7,8,9,10,20,30,50,100,150,200,300,500,700,1000,1500};

    vector<vector<vector<double>>> SPMTMean = vector<vector<vector<double>>>(17612, vector<vector<double>>(6, vector<double>(Points, 0)));
    vector<vector<vector<double>>> SPMTError = vector<vector<vector<double>>>(17612, vector<vector<double>>(6, vector<double>(Points, 0)));
    vector<vector<double>> LPMTMean = vector<vector<double>>(17612, vector<double>(Points,0));
    vector<vector<double>> LPMTError = vector<vector<double>>(17612, vector<double>(Points,0));

    TTree *Charge;
    double LPMTCharge[17612];
    double SPMTCharge[25600];

    for (int EnergyNum = 0; EnergyNum < 21; EnergyNum++)
    {
        TH1 *LPMTnpeSpectrum[17612];
        TH1 *SPMTnpeSpectrum[17612][6];

        for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
        {
            TString LPMTnpeSpectrumName = TString::Format("LPMT nPE spectrun %1dMeV LPMT%1d",energy[EnergyNum],LPMTID);
            LPMTnpeSpectrum[LPMTID] = new TH1F(LPMTnpeSpectrumName,LPMTnpeSpectrumName,10000,0,1000);
            for (int id = 0; id < 6; id++)
            {
                TString SPMTnpeSpectrumName;
                SPMTnpeSpectrumName = TString::Format("SPMT_%1d nPE spectrun %1dMeV LPMT%1d",id,energy[EnergyNum],LPMTID);
                SPMTnpeSpectrum[LPMTID][id] = new TH1F(SPMTnpeSpectrumName,SPMTnpeSpectrumName,1000,0,100);
            }
        }

        TFile *inputfile = TFile::Open(TString::Format("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/AllLPMT/CombineChargeSpectrum/CombineCharge_%1dMeV.root",energy[EnergyNum]),"read");
        Charge = (TTree*)inputfile->Get("charge");
        Charge->SetBranchAddress("LPMTCharge",LPMTCharge);
        Charge->SetBranchAddress("SPMTCharge",SPMTCharge);
        for (int Entry = 0; Entry < Charge->GetEntries(); Entry++)
        {
            Charge->GetEntry(Entry);
            for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
            {
                LPMTnpeSpectrum[LPMTID]->Fill(LPMTCharge[LPMTID]);
                for (int id = 0; id < 6; id++)
                {
                    if (CorrespondingSPMT[LPMTID][id] != 50000)
                    {
                        SPMTnpeSpectrum[LPMTID][id]->Fill(SPMTCharge[CorrespondingSPMT[LPMTID][id]-300000]);
                    }
                }
            }

            // if (Entry%10000 == 0 )
            // {
            //     cout<<Entry/10000<<endl;
            // }
        }
        inputfile->Close();

        cout<<"Read file "<<EnergyNum<< "Finished"<<endl;

        for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
        {
            LPMTMean[LPMTID][EnergyNum] = LPMTnpeSpectrum[LPMTID]->GetMean();
            LPMTError[LPMTID][EnergyNum] = LPMTnpeSpectrum[LPMTID]->GetMeanError();
            
            LPMTnpeSpectrum[LPMTID]->SetDirectory(nullptr);
            LPMTnpeSpectrum[LPMTID]->Delete();
            
            for (int id = 0; id < 6; id++)
            {
                if (CorrespondingSPMT[LPMTID][id] != 50000)
                {
                    SPMTMean[LPMTID][id][EnergyNum] = SPMTnpeSpectrum[LPMTID][id]->GetMean();
                    SPMTError[LPMTID][id][EnergyNum] = SPMTnpeSpectrum[LPMTID][id]->GetMeanError();
                }
                SPMTnpeSpectrum[LPMTID][id]->SetDirectory(nullptr);
                SPMTnpeSpectrum[LPMTID][id]->Delete();
            }
            if (LPMTID%100 == 0 )
            {
                cout<<LPMTID/100<<endl;
            }
        }
        cout<<"File "<<EnergyNum<< "Finished"<<endl;
    }

    for (int LPMTID = 0; LPMTID < 17612; LPMTID++)
    {
        outputfile->cd();
        outputfile->cd(TString::Format("%1d",LPMTID/1000));
        for (int id = 0; id < 6; id++)
        {
            if (CorrespondingSPMT[LPMTID][id] != 50000)
            {
                TGraphErrors *npeLPMTtonpeSPMT;
                npeLPMTtonpeSPMT = new TGraphErrors(Points,&SPMTMean[LPMTID][id][0],&LPMTMean[LPMTID][0],&SPMTError[LPMTID][id][0],&LPMTError[LPMTID][0]);
                TString GraphName = TString::Format("LPMT_%1d npe to SPMT_%1d npe",LPMTID,CorrespondingSPMT[LPMTID][id]);
                npeLPMTtonpeSPMT->SetTitle(GraphName);
                npeLPMTtonpeSPMT->SetName(GraphName);
                npeLPMTtonpeSPMT->GetXaxis()->SetTitle("nPE SPMT");
                npeLPMTtonpeSPMT->GetYaxis()->SetTitle("nPE LPMT");
                TF1 *f1 = new TF1("f1","[0]*x",0,10);
                f1->SetParNames("k");
                npeLPMTtonpeSPMT->Fit(f1,"Q");
                double chi2 = f1->GetChisquare();
                double k = f1->GetParameter(0)*SPMTQE[CorrespondingSPMT[LPMTID][id]-300000]/LPMTQE[LPMTID];
                double kError = f1->GetParError(0)*SPMTQE[CorrespondingSPMT[LPMTID][id]-300000]/LPMTQE[LPMTID];
                if (k>51.0 || k<43.5)
                {
                    cout<<"CalibFactor = "<<k<<" LPMT"<<LPMTID<<" SPMT"<<CorrespondingSPMT[LPMTID][id]<<endl;
                }
                
                
                CalibFactorHist->Fill(k);
                CalibFactorErrorHist->Fill(kError);
                ChiSquareHist->Fill(chi2);
                gStyle->SetOptFit(1111);
                gStyle->SetStatY(0.5);

                npeLPMTtonpeSPMT->Write();
                npeLPMTtonpeSPMT->Delete();
                f1->Delete();
            }
        }
    }
    // TCanvas *Canvas[6];

    // TString GraphName = TString::Format("LPMT npe to SPMT_%1d npe",CorrespondingID);
    // npeLPMTtonpeSPMT = new TGraphErrors(Points,SPMTMean,LPMTMean,SPMTError,LPMTError);
    // npeLPMTtonpeSPMT->SetTitle(GraphName);
    // npeLPMTtonpeSPMT->SetName(GraphName);
    // npeLPMTtonpeSPMT->GetXaxis()->SetTitle("nPE SPMT");
    // npeLPMTtonpeSPMT->GetYaxis()->SetTitle("nPE LPMT");
    // TF1 *f1 = new TF1("f1","[0]*x",0,10);
    // f1->SetParNames("k");
    // npeLPMTtonpeSPMT->Fit(f1);

    // double chi2 = f1->GetChisquare();
    // k = f1->GetParameter(0)*SPMTQE[CorrespondingSPMT[LPMTID][CorrespondingID]-300000]/LPMTQE[LPMTID];
    // kError = f1->GetParError(0)*SPMTQE[CorrespondingSPMT[LPMTID][CorrespondingID]-300000]/LPMTQE[LPMTID];

    // CalibFactorHist->Fill(k);
    // CalibFactorErrorHist->Fill(kError);
    // ChiSquareHist->Fill(chi2);

    // gStyle->SetOptFit(1111);
    // gStyle->SetStatY(0.5);

    
    // npeLPMTtonpeSPMT->Delete();
    // f1->Delete();
    return 1.0;

    // TString CanvasName = TString::Format("C%1d",id);
    // Canvas[id] = new TCanvas(CanvasName,CanvasName,1);
        // Canvas[id]->cd();
        // gPad->SetLogx();
        // gPad->SetLogy();
    // npeLPMTtonpeSPMT[id]->Draw("AP");
        // output->cd();
    // npeLPMTtonpeSPMT[id]->Write(); 
}

void npeLPMTtonpeSPMT()
{
    double LPMTQE[17612];
    double SPMTQE[25600];
    int CorrespondingSPMT[17612][6];

    GetLPMTQE(LPMTQE);
    GetSPMTQE(SPMTQE);
    GetCorrespondingSPMT(CorrespondingSPMT);

    TFile *outputfile = TFile::Open("/junofs/users/chuziliang125/LPMTChargeCalibration/LaserAnalysis/AllLPMT/npeLPMTtonpeSPMT_1.root","recreate");

    for (int i = 0; i < 17612/1000; i++)
    {
        outputfile->mkdir(TString::Format("%1d",i));
    }
    outputfile->cd();

    TH1 *CalibFactorHist = new TH1F("CalibFactorHist","Calibration Factor",1000,0,100);
    TH1 *CalibFactorErrorHist = new TH1F("CalibFactorErrorHist","Calibration Factor error",1000,0,100);
    TH1 *KarSquareHist = new TH1F("KarSquareHist","k square",1000,0,100);

    GetCalibFactor(CorrespondingSPMT,LPMTQE,SPMTQE, CalibFactorHist, CalibFactorErrorHist, KarSquareHist, outputfile);
    // for (int LPMTID = 0; LPMTID < 10; LPMTID++)
    // {
    //     for (int CorrespondingID = 0; CorrespondingID < 6; CorrespondingID++)
    //     {
    //         GetCalibFactor(LPMTID,CorrespondingID,CorrespondingSPMT,LPMTQE,SPMTQE, CalibFactorHist, CalibFactorErrorHist, KarSquareHist);
    //     }
    //     cout<<LPMTID<<" Finished."<<endl;
    // }
    outputfile->cd();
    CalibFactorHist->Write();
    CalibFactorErrorHist->Write();
    KarSquareHist->Write();

}