int EventCounter()
{
    TFile *inputfile = TFile::Open("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/TrailerProblemAnalysis/8010_230822-1426_2431_0.root","read");
    TTree *EventData = (TTree*)inputfile->Get("EventData");
    TTree *BlockTrailer = (TTree*)inputfile->Get("BlockTrailer");

    vector<double> VectorEventCounterTotal[128];
    vector<double> VectorEventCounterProblematic[128];
    vector<double> VectorEventIDTotal[128];
    vector<double> VectorEventIDProblematic[128];
    
    vector<double> VectorCoarseTimeTotal[128];
    vector<double> VectorCoarseTimeProblematic[128];

    vector<double> VectorHitRateTotal[128];
    vector<double> VectorHitRateProblematic[128];
    vector<double> VectorCoarseTimeOverFlowTotal;
    vector<double> VectorCoarseTimeOverFlowProblematic;

    UInt_t chargeValue;
    UInt_t blockID;
    UInt_t blockChannelNumber;
    UInt_t EventCounter;
    UInt_t ABCChannelNumber;
    UInt_t BlockNumber;
    UInt_t CoarseTime;

    EventData->SetBranchAddress("ChargeValue",&chargeValue);
    EventData->SetBranchAddress("BlockId",&blockID);
    EventData->SetBranchAddress("ChannelNumber",&blockChannelNumber);
    EventData->SetBranchAddress("EventCounter",&EventCounter);
    EventData->SetBranchAddress("BlockNumber",&BlockNumber);
    EventData->SetBranchAddress("CoarseTime",&CoarseTime);

    UInt_t BlockNumberTrailer;
    UInt_t CoarseTimeOverflow;
    UInt_t NumberChannels;
    UInt_t NumberChannelsUser;

    BlockTrailer->SetBranchAddress("BlockNumber",&BlockNumberTrailer);
    BlockTrailer->SetBranchAddress("CoarseTimeOverflow",&CoarseTimeOverflow);
    BlockTrailer->SetBranchAddress("NumberChannels",&NumberChannels);
    BlockTrailer->SetBranchAddress("NumberChannelsUser",&NumberChannelsUser);

    int Evt_ID[128];
    int EvtNumber[128];
    for (int i = 0; i < 128; i++)
    {
        Evt_ID[i] = 0;
        EvtNumber[i] = 0;
    }
    
    UInt_t CoarseTimeOverFlowLastStep;
    bool TypeProblematic = false;
    double HitRate[128];

    for(int i = 0; i < EventData->GetEntries(); i++)
    {
        EventData->GetEntry(i);
        BlockTrailer->GetEntry(BlockNumber);
        if (BlockNumberTrailer != BlockNumber) return 1;

        ABCChannelNumber = 8*blockID + blockChannelNumber;
        
        Evt_ID[ABCChannelNumber]++;

        if (CoarseTimeOverflow != CoarseTimeOverFlowLastStep && i != 0)
        {
            for (int t = 0; t < 128; t++)
            {
                HitRate[t] = (double)EvtNumber[t]/1.6777;
                EvtNumber[t] = 0;
            }

            if (TypeProblematic)
            {
                for (int t = 0; t < 128; t++)
                {
                    VectorHitRateProblematic[t].push_back(HitRate[t]);
                }
                VectorCoarseTimeOverFlowProblematic.push_back((double)CoarseTimeOverFlowLastStep);
            }
            else
            {
                for (int t = 0; t < 128; t++)
                {
                    VectorHitRateTotal[t].push_back(HitRate[t]);
                }
                VectorCoarseTimeOverFlowTotal.push_back((double)CoarseTimeOverFlowLastStep);
            }
            
            TypeProblematic = false;
        }
        EvtNumber[ABCChannelNumber]++;

        if (NumberChannels == NumberChannelsUser)
        {
            VectorEventCounterTotal[ABCChannelNumber].push_back(EventCounter);
            VectorEventIDTotal[ABCChannelNumber].push_back(Evt_ID[ABCChannelNumber]);
            VectorCoarseTimeTotal[ABCChannelNumber].push_back(CoarseTime);
        }
        else
        {
            VectorEventCounterProblematic[ABCChannelNumber].push_back(EventCounter);
            VectorEventIDProblematic[ABCChannelNumber].push_back(Evt_ID[ABCChannelNumber]);
            VectorCoarseTimeProblematic[ABCChannelNumber].push_back(CoarseTime);

            if (CoarseTimeOverflow == 9)
            {
                cout<<"blockNumber = "<<BlockNumber<<"\t";
                cout<<"CoarseTime = "<<CoarseTime<<"\t";
                cout<<"ABCChannel = "<<ABCChannelNumber<<endl;
            }
            

            TypeProblematic = true;
        } 
       CoarseTimeOverFlowLastStep = CoarseTimeOverflow;
    }

    

    TGraph *EventCounterTotal[128];
    TGraph *EventCounterProblematic[128];
    TGraph *CoarseTimeTotal[128];
    TGraph *CoarseTimeProblematic[128];
    TGraph *HitRateTotal[128];
    TGraph *HitRateProblematic[128];

    for (int iChn=0; iChn<128; iChn++) {
        EventCounterTotal[iChn] = new TGraph(VectorEventCounterTotal[iChn].size(),&VectorEventIDTotal[iChn][0],&VectorEventCounterTotal[iChn][0]);
        EventCounterTotal[iChn]->SetName(TString::Format("Chn%1d EventCounter to Evt_ID",iChn));
        EventCounterTotal[iChn]->SetTitle(TString::Format("Chn%1d EventCounter to Evt_ID",iChn));
        EventCounterTotal[iChn]->GetXaxis()->SetTitle("Evt_ID");
        EventCounterTotal[iChn]->GetYaxis()->SetTitle("Event counter");

        EventCounterProblematic[iChn] = new TGraph(VectorEventCounterProblematic[iChn].size(),&VectorEventIDProblematic[iChn][0],&VectorEventCounterProblematic[iChn][0]);
        EventCounterProblematic[iChn]->SetName(TString::Format("Problematic Chn%1d EventCounter to Evt_ID",iChn));
        EventCounterProblematic[iChn]->SetTitle(TString::Format("Chn%1d EventCounter to Evt_ID",iChn));
        EventCounterProblematic[iChn]->GetXaxis()->SetTitle("Evt_ID");
        EventCounterProblematic[iChn]->GetYaxis()->SetTitle("Event counter");

        CoarseTimeTotal[iChn] = new TGraph(VectorCoarseTimeTotal[iChn].size(),&VectorEventIDTotal[iChn][0],&VectorCoarseTimeTotal[iChn][0]);
        CoarseTimeTotal[iChn]->SetName(TString::Format("Chn%1d Coarse time to Evt_ID",iChn));
        CoarseTimeTotal[iChn]->SetTitle(TString::Format("Chn%1d Coarse time to Evt_ID",iChn));
        CoarseTimeTotal[iChn]->GetXaxis()->SetTitle("Evt_ID");
        CoarseTimeTotal[iChn]->GetYaxis()->SetTitle("Coarse Time");

        CoarseTimeProblematic[iChn] = new TGraph(VectorCoarseTimeProblematic[iChn].size(),&VectorEventIDProblematic[iChn][0],&VectorCoarseTimeProblematic[iChn][0]);
        CoarseTimeProblematic[iChn]->SetName(TString::Format("Problematic Chn%1d Coarse time to event counter",iChn));
        CoarseTimeProblematic[iChn]->SetTitle(TString::Format("Chn%1d Coarse time to event counter",iChn));
        CoarseTimeProblematic[iChn]->GetXaxis()->SetTitle("Event ID");
        CoarseTimeProblematic[iChn]->GetYaxis()->SetTitle("Coarse Time");

        HitRateTotal[iChn] = new TGraph(VectorHitRateTotal[iChn].size(),&VectorCoarseTimeOverFlowTotal[0],&VectorHitRateTotal[iChn][0]);
        HitRateTotal[iChn]->SetName(TString::Format("Chn%1d Hit rate to CoarseTimeOverflow",iChn));
        HitRateTotal[iChn]->SetTitle(TString::Format("Chn%1d Hit rate to CoarseTimeOverflow",iChn));
        HitRateTotal[iChn]->GetXaxis()->SetTitle("CoarseTimeOverflow");
        HitRateTotal[iChn]->GetYaxis()->SetTitle("Hit Rate");

        HitRateProblematic[iChn] = new TGraph(VectorHitRateProblematic[iChn].size(),&VectorCoarseTimeOverFlowProblematic[0],&VectorHitRateProblematic[iChn][0]);
        HitRateProblematic[iChn]->SetName(TString::Format("Problematic Chn%1d Hit rate to CoarseTimeOverflow",iChn));
        HitRateProblematic[iChn]->SetTitle(TString::Format("Chn%1d Hit rate to CoarseTimeOverflow",iChn));
        HitRateProblematic[iChn]->GetXaxis()->SetTitle("CoarseTimeOverflow");
        HitRateProblematic[iChn]->GetYaxis()->SetTitle("Hit Rate");
    }


    int compteur_canvas = 0;
    int compteur_plots = 0;
    TCanvas* CanvasEventCounter = new TCanvas("CanvasEventCounter");
    TCanvas* CanvasCoarseTime = new TCanvas("CanvasCoarseTime");
    TCanvas* CanvasHitRate = new TCanvas("CanvasHitRate");
    CanvasEventCounter->Divide(4,4);
    CanvasCoarseTime->Divide(4,4);
    CanvasHitRate->Divide(4,4);

    TFile *OutputRootFile = new TFile("EventCounterOutput.root","RECREATE");
    for(int i=0;i<128; i++)
    {
        CanvasEventCounter->cd(compteur_plots+1);
        EventCounterTotal[i]->Draw("AP");
        EventCounterProblematic[i]->SetMarkerColor(kRed);
        EventCounterProblematic[i]->Draw("p");
        CanvasEventCounter->Update();

        CanvasCoarseTime->cd(compteur_plots+1);
        CoarseTimeTotal[i]->Draw("AP");
        CoarseTimeProblematic[i]->SetMarkerColor(kRed);
        CoarseTimeProblematic[i]->Draw("p");
        CanvasCoarseTime->Update();

        CanvasHitRate->cd(compteur_plots+1);
        double max1 = *max_element(VectorHitRateProblematic[i].begin(), VectorHitRateProblematic[i].end());
        double max2 = *max_element(VectorHitRateTotal[i].begin(), VectorHitRateTotal[i].end());
        double min1 = *min_element(VectorHitRateProblematic[i].begin(), VectorHitRateProblematic[i].end());
        double min2 = *min_element(VectorHitRateTotal[i].begin(), VectorHitRateTotal[i].end());

        double maxNum = (max1 > max2) ? max1 : max2;
        double minNum = (min1 < min2) ? min1 : min2;
        
        HitRateTotal[i]->SetMarkerStyle(21);
        HitRateTotal[i]->SetMarkerSize(0.5);
        HitRateTotal[i]->GetYaxis()->SetRangeUser(0.9*minNum,1.1*maxNum);
        HitRateTotal[i]->Draw("AP");
        HitRateProblematic[i]->SetMarkerColor(kRed);
        HitRateProblematic[i]->SetMarkerStyle(21);
        HitRateProblematic[i]->SetMarkerSize(0.5);
        HitRateProblematic[i]->Draw("P");
        CanvasHitRate->Update();

        compteur_plots++;
        if (compteur_plots == 16 || i == 127)
        {
            compteur_canvas++;
            compteur_plots = 0;
            
            CanvasCoarseTime->Write();

            if (compteur_canvas == 1)
            {
                CanvasEventCounter->Print("EventCounter_EvtID.pdf(","pdf");
                CanvasCoarseTime->Print("CoarseTime_EvtID.pdf(","pdf");
                CanvasHitRate->Print("HitRate_CoarseTimeOverFlow.pdf(","pdf");
            }
            else if (i==127)
            {
                CanvasEventCounter->Print("EventCounter_EvtID.pdf)","pdf");
                CanvasCoarseTime->Print("CoarseTime_EvtID.pdf)","pdf");
                CanvasHitRate->Print("HitRate_CoarseTimeOverFlow.pdf)","pdf");
            }
            else
            {
                CanvasEventCounter->Print("EventCounter_EvtID.pdf","pdf");
                CanvasCoarseTime->Print("CoarseTime_EvtID.pdf","pdf");
                CanvasHitRate->Print("HitRate_CoarseTimeOverFlow.pdf","pdf");
            }
            CanvasEventCounter->Clear();
            CanvasCoarseTime->Clear();
            CanvasHitRate->Clear();
            CanvasEventCounter->Divide(4,4);
            CanvasCoarseTime->Divide(4,4);
            CanvasHitRate->Divide(4,4);
        }
        
        
    }
    OutputRootFile->Close();

    return 0;
}