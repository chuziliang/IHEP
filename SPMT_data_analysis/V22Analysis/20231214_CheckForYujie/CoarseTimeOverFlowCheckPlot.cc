void CoarseTimeOverFlowCheckPlot(TString filename, TString outputname)
{
    // TString filename;
    // filename = "/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/root/8001_231207-0954_2381_0.root";
    
    TFile *inputfile = TFile::Open(filename,"read");
    TTree *DataTree    = (TTree*)inputfile->Get("EventData");
    TTree *TrailerTree = (TTree*)inputfile->Get("BlockTrailer");

    uint32_t marker;
    uint32_t blockId;
    uint32_t emptyHeader;
    uint32_t abcTimestamp;
    uint32_t eventType;
    uint32_t channelNumber;
    uint32_t blockChannelNumber;
    uint32_t coarseTime;
    uint32_t gain;
    uint32_t eventCounter;
    uint32_t chargeValue;
    uint32_t fineTime;
    uint32_t numChannels;
    uint32_t emptyTrailer;
    uint32_t coarseTimeOverflow;
    uint32_t eventCounterPerCh;
    uint32_t dataPackId;
    uint32_t sign;
    uint32_t nChs;

    DataTree->SetBranchAddress("BlockId", &blockId);
    DataTree->SetBranchAddress("DataPackId", &dataPackId);
    DataTree->SetBranchAddress("ABCTimestamp", &abcTimestamp);
    DataTree->SetBranchAddress("EventType", &eventType);
    DataTree->SetBranchAddress("ChannelNumber", &blockChannelNumber);
    DataTree->SetBranchAddress("ABCChannelNumber", &channelNumber);
    DataTree->SetBranchAddress("CoarseTime", &coarseTime);
    DataTree->SetBranchAddress("Gain", &gain);
    DataTree->SetBranchAddress("Sign", &sign);
    DataTree->SetBranchAddress("EventCounterPerChannelUser", &eventCounter);
    DataTree->SetBranchAddress("EventCounterPerChannel", &eventCounterPerCh);
    DataTree->SetBranchAddress("Charge", &chargeValue);
    DataTree->SetBranchAddress("FineTime", &fineTime);

    TrailerTree->SetBranchAddress("NumberChannels", &numChannels);
    TrailerTree->SetBranchAddress("NumberChannelsUser", &nChs);
    TrailerTree->SetBranchAddress("CoarseTimeOverflow", &coarseTimeOverflow);

    vector<int> VectorCoarseTime[128];
    vector<int> VectorEventCounterPerCh[128];
    vector<int> VectorOverFlow[128];
    int eventCounterPerChOverFlow[128];
    int LastEventCounterPerChannel[128];
    for (int id = 0; id < 128; id++)
    {
        eventCounterPerChOverFlow[id] = 0;
        LastEventCounterPerChannel[id] = 0;
    }
    
    
    inputfile->cd();

    for (int entry = 0; entry < 10000000; entry++)
    {
        DataTree->GetEntry(entry);
        TrailerTree->GetEntry(dataPackId - 1);

        if (eventCounterPerCh < LastEventCounterPerChannel[channelNumber])
        {
            eventCounterPerChOverFlow[channelNumber]++;
        }
        

        // VectorCoarseTime[channelNumber].push_back(coarseTimeOverflow*67108864+coarseTime);
        VectorCoarseTime[channelNumber].push_back(coarseTime);
        VectorEventCounterPerCh[channelNumber].push_back(eventCounterPerChOverFlow[channelNumber]*2048 + eventCounterPerCh);
        VectorOverFlow[channelNumber].push_back(coarseTimeOverflow);

        LastEventCounterPerChannel[channelNumber] = eventCounterPerCh;
    }

    inputfile->Close();
    
    TFile *outputfile = TFile::Open(outputname,"recreate");
    TGraph *GraphCoarseTime[128];
    TGraph *GraphOverFlow[128];
    for (int i = 0; i < 128; i++)
    {
        GraphCoarseTime[i] = new TGraph(VectorEventCounterPerCh[i].size(),&VectorEventCounterPerCh[i][0],&VectorCoarseTime[i][0]);
        GraphCoarseTime[i]->SetTitle(TString::Format("CoarseTime to EventCounterPerChannel Chn%1d",i));
        GraphCoarseTime[i]->GetXaxis()->SetTitle("EventCounterPerChannel");
        GraphCoarseTime[i]->GetYaxis()->SetTitle("CoarseTime");
        // GraphCoarseTime[i]->Write();

        GraphOverFlow[i] = new TGraph(VectorEventCounterPerCh[i].size(),&VectorEventCounterPerCh[i][0],&VectorOverFlow[i][0]);
        GraphOverFlow[i]->SetTitle(TString::Format("CoarseTimeOverFlow to EventCounterPerChannel Chn%1d",i));
        GraphOverFlow[i]->GetXaxis()->SetTitle("EventCounterPerChannel");
        GraphOverFlow[i]->GetYaxis()->SetTitle("CoarseTimeOverFlow");
        GraphOverFlow[i]->SetMarkerColor(kBlue);
        // GraphOverFlow[i]->Write();
        TCanvas *c = new TCanvas(TString::Format("c%1d",i),TString::Format("c%1d",i),1);
        GraphCoarseTime[i]->Draw("AP");
        c->Update();

        Float_t scale = TMath::MaxElement(VectorEventCounterPerCh[i].size(),GraphCoarseTime[i]->GetY())/TMath::MaxElement(VectorEventCounterPerCh[i].size(),GraphOverFlow[i]->GetY());
        Float_t rightmax = gPad->GetUymax()/scale;
        GraphOverFlow[i]->Scale(scale);
        
        GraphOverFlow[i]->Draw("same p");
        GraphOverFlow[i]->SetMarkerColor(kRed);
        TGaxis*axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
                            gPad->GetUxmax(),gPad->GetUymax(),
                            0,rightmax,210,"+L");
        axis->SetLineColor(kRed);
        axis->SetLabelColor(kRed);
        axis->Draw();
        c->Update();
        for (int point = 0; point < GraphOverFlow[i]->GetN(); point++)
        {
            if (GraphOverFlow[i]->GetPointY(point) != GraphOverFlow[i]->GetPointY(point-1))
            {
                TLine *overflowline  = new TLine(GraphOverFlow[i]->GetPointX(point),0,GraphOverFlow[i]->GetPointX(point),67108864);
                overflowline->SetLineColor(kBlue);
                overflowline->SetLineStyle(3);
                overflowline->Draw("same");
                c->Update();
            }
            
        }
        
        
        c->Write();
    }
    outputfile->Close();
    
}