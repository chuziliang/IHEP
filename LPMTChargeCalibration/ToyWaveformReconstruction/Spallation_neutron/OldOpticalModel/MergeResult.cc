void MergeResult(int ChosenType, int CutLevel)
{

    TString ChosenTypeString;
    if (ChosenType==0) ChosenTypeString = "All";
    else if (ChosenType==1) ChosenTypeString = "Hamamatsu";
    else if (ChosenType==2) ChosenTypeString = "NNVT";
    else if (ChosenType==3) ChosenTypeString = "HighQENNVT";

    TStatistic *Sta_LPMTMu[21];
    TStatistic *Sta_TrueLPMTMu[21];
    TStatistic *Sta_TrueLPMTMu_QE[21];
    TStatistic *Sta_SPMTMu[21];
    TStatistic *Sta_SPMTDCR[21];
    TStatistic *Sta_SPMTQE[21];
    TStatistic *Sta_LPMTQE[21];

    TList *List_LPMTMu = new TList();
    TList *List_TrueLPMTMu = new TList();
    TList *List_TrueLPMTMu_QE = new TList();
    TList *List_SPMTMu = new TList();
    TList *List_SPMTDCR = new TList();
    TList *List_SPMTQE = new TList();
    TList *List_LPMTQE = new TList();

    vector<unsigned long long> SPMT0_Calib(21,0);
    vector<unsigned long long> SPMT0_DetSim(21,0);

    for (int QID = 0; QID < 21; QID++)
    {
        Sta_LPMTMu[QID] = new TStatistic(TString::Format("LPMTMu Q%1d",QID));
        Sta_TrueLPMTMu_QE[QID] = new TStatistic(TString::Format("TrueLPMTMu_QE Q%1d",QID));
        Sta_TrueLPMTMu[QID] = new TStatistic(TString::Format("TrueLPMTMu Q%1d",QID));
        Sta_SPMTMu[QID] = new TStatistic(TString::Format("SPMTMu Q%1d",QID));
        Sta_SPMTDCR[QID] = new TStatistic(TString::Format("SPMTDCR Q%1d",QID));
        Sta_SPMTQE[QID] = new TStatistic(TString::Format("SPMTQE Q%1d",QID));
        Sta_LPMTQE[QID] = new TStatistic(TString::Format("LPMTQE Q%1d",QID));
    }

    for (int FileId = 0; FileId < 465; FileId++)
    {
        TString InputName = "/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Spallation_neutron/OldOpticalModel/result/npeLPMTtonpeSPMT_B12_DetSim_AllPMT_"+ChosenTypeString+TString::Format("_File%1d",FileId)+"_Cut"+TString::Format("%1d",CutLevel)+".root";
        TFile *inputfile = TFile::Open(InputName,"read");
        if (!inputfile) 
        {
            cout<<"File "<<FileId<<" not exist"<<endl;
            continue;
        }
        TTree *SPMT0_Tree = (TTree*)inputfile->Get("SPMT0");
        if (!SPMT0_Tree) continue;
        int SPMT0_Calib_i;
        int SPMT0_DetSim_i;
        SPMT0_Tree->SetBranchAddress("Calib",&SPMT0_Calib_i);
        SPMT0_Tree->SetBranchAddress("DetSim",&SPMT0_DetSim_i);

        TStatistic *Sta_Temp_LPMTMu = nullptr;
        TStatistic *Sta_Temp_TrueLPMTMu = nullptr;
        TStatistic *Sta_Temp_TrueLPMTMu_QE = nullptr;
        TStatistic *Sta_Temp_SPMTMu = nullptr;
        TStatistic *Sta_Temp_SPMTDCR = nullptr;
        TStatistic *Sta_Temp_SPMTQE = nullptr;
        TStatistic *Sta_Temp_LPMTQE = nullptr;
        for (int QNum = 0; QNum < 21; QNum++)
        {
            SPMT0_Tree->GetEntry(QNum);
            SPMT0_Calib[QNum] += SPMT0_Calib_i;
            SPMT0_DetSim[QNum] += SPMT0_DetSim_i;

            Sta_Temp_LPMTMu = (TStatistic*)inputfile->Get(TString::Format("%1d/LPMTMu Q%1d",QNum,QNum));
            Sta_Temp_TrueLPMTMu = (TStatistic*)inputfile->Get(TString::Format("%1d/TrueLPMTMu_QE Q%1d",QNum,QNum));
            Sta_Temp_TrueLPMTMu_QE = (TStatistic*)inputfile->Get(TString::Format("%1d/TrueLPMTMu Q%1d",QNum,QNum));
            Sta_Temp_SPMTMu = (TStatistic*)inputfile->Get(TString::Format("%1d/SPMTMu Q%1d",QNum,QNum));
            Sta_Temp_SPMTDCR = (TStatistic*)inputfile->Get(TString::Format("%1d/SPMTDCR Q%1d",QNum,QNum));
            Sta_Temp_SPMTQE = (TStatistic*)inputfile->Get(TString::Format("%1d/SPMTQE Q%1d",QNum,QNum));
            Sta_Temp_LPMTQE = (TStatistic*)inputfile->Get(TString::Format("%1d/LPMTQE Q%1d",QNum,QNum));
            
            List_LPMTMu->Add(Sta_Temp_LPMTMu);
            List_TrueLPMTMu->Add(Sta_Temp_TrueLPMTMu);
            List_TrueLPMTMu_QE->Add(Sta_Temp_TrueLPMTMu_QE);
            List_SPMTMu->Add(Sta_Temp_SPMTMu);
            List_SPMTDCR->Add(Sta_Temp_SPMTDCR);
            List_SPMTQE->Add(Sta_Temp_SPMTQE);
            List_LPMTQE->Add(Sta_Temp_LPMTQE);

            Sta_LPMTMu[QNum]->Merge(List_LPMTMu);
            Sta_TrueLPMTMu_QE[QNum]->Merge(List_TrueLPMTMu);
            Sta_TrueLPMTMu[QNum]->Merge(List_TrueLPMTMu_QE);
            Sta_SPMTMu[QNum]->Merge(List_SPMTMu);
            Sta_SPMTDCR[QNum]->Merge(List_SPMTDCR);
            Sta_SPMTQE[QNum]->Merge(List_SPMTQE);
            Sta_LPMTQE[QNum]->Merge(List_LPMTQE);

            List_LPMTMu->Clear();
            List_TrueLPMTMu->Clear();
            List_TrueLPMTMu_QE->Clear();
            List_SPMTMu->Clear();
            List_SPMTDCR->Clear();
            List_SPMTQE->Clear();
            List_LPMTQE->Clear();

            Sta_Temp_LPMTMu->Delete();
            Sta_Temp_TrueLPMTMu->Delete();
            Sta_Temp_TrueLPMTMu_QE->Delete();
            Sta_Temp_SPMTMu->Delete();
            Sta_Temp_SPMTDCR->Delete();
            Sta_Temp_SPMTQE->Delete();
            Sta_Temp_LPMTQE->Delete();
        }
        inputfile->Close();
    }

    TString OutputName = "/junofs/users/chuziliang125/LPMTChargeCalibration/ToyWaveformReconstruction/Spallation_neutron/OldOpticalModel/MeragedResult/npeLPMTtonpeSPMT_B12_DetSim_AllPMT_"+ChosenTypeString+"_Cut"+TString::Format("%1d",CutLevel)+".root";
    TFile *outputfile = TFile::Open(OutputName,"recreate");
    TTree *Output_SPMT0_Tree = new TTree("SPMT0","SPMT0");
    unsigned long long Output_SPMT0_Calib_i;
    unsigned long long Output_SPMT0_DetSim_i;
    Output_SPMT0_Tree->Branch("Calib",&Output_SPMT0_Calib_i);
    Output_SPMT0_Tree->Branch("DetSim",&Output_SPMT0_DetSim_i);
    for (int QNum = 0; QNum < 21; QNum++)
    {
        outputfile->mkdir(TString::Format("%1d",QNum));
        outputfile->cd(TString::Format("%1d",QNum));

        Sta_LPMTMu[QNum]->Write();
        Sta_TrueLPMTMu[QNum]->Write();
        Sta_TrueLPMTMu_QE[QNum]->Write();
        Sta_SPMTMu[QNum]->Write();
        Sta_SPMTDCR[QNum]->Write();
        Sta_SPMTQE[QNum]->Write();
        Sta_LPMTQE[QNum]->Write();
        outputfile->cd();

        Output_SPMT0_Calib_i = SPMT0_Calib[QNum];
        Output_SPMT0_DetSim_i = SPMT0_DetSim[QNum];
        Output_SPMT0_Tree->Fill();
    }

    Output_SPMT0_Tree->Write();
    outputfile->Close();
}