void CoarseTimeDistance(int num)
{
    TString filename[2];
    filename[0] = "/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/root/8001_231201-1057_2310_0.root";
    filename[1] = "/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/root/8001_231201-1136_2314_0.root";

    TString outputfilename[2];
    outputfilename[0] = "/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/8001_231201-1057_2310_0_Distance.root";
    outputfilename[1] = "/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/8001_231201-1136_2314_0_Distance.root";
    
    TFile *inputfile = TFile::Open(filename[num],"read");
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
    int nChs;

    DataTree->SetBranchAddress("BlockId", &blockId);
    DataTree->SetBranchAddress("DataPackId", &dataPackId);
    DataTree->SetBranchAddress("ABCTimestamp", &abcTimestamp);
    DataTree->SetBranchAddress("EventType", &eventType);
    DataTree->SetBranchAddress("ChannelNumber", &blockChannelNumber);
    DataTree->SetBranchAddress("ABCChannelNumber", &channelNumber);
    DataTree->SetBranchAddress("CoarseTime", &coarseTime);
    DataTree->SetBranchAddress("Gain", &gain);
    DataTree->SetBranchAddress("Sign", &sign);
    DataTree->SetBranchAddress("EventCounter", &eventCounter);
    DataTree->SetBranchAddress("EventCounterPerChannel", &eventCounterPerCh);
    DataTree->SetBranchAddress("Charge", &chargeValue);
    DataTree->SetBranchAddress("FineTime", &fineTime);

    TrailerTree->SetBranchAddress("NumberChannels", &numChannels);
    TrailerTree->SetBranchAddress("NumberChannelsUser", &nChs);
    TrailerTree->SetBranchAddress("CoarseTimeOverflow", &coarseTimeOverflow);

    TFile *outputfile = TFile::Open(outputfilename[num],"recreate");
    outputfile->cd();
    TH1 *DistanceHist = new TH1F("Distance","Distance",67108864,0,67108864);
    TH1 *DistanceHist_EvtCounter = new TH1F("Distance divided by EventCounterPerChannel","Distance divided by EventCounterPerChannel",67108864,0,67108864);
    vector<int> VectorEventCounterPerCh;
    vector<int> VectorEventCounterUser;
    for (int Chn = 0; Chn < 128; Chn++)
    {
        inputfile->cd();
        uint32_t LastCoarseTime = 0;
        uint32_t Distance = 0;
        uint32_t LastCoarseTimeOverflow = 0;
        uint32_t LastEventCounterPerChannel = 0;
        int FirstEntry = 0;
        int EventCounterPerChannelUser = 0;
        for (int entry = 0; entry < DataTree->GetEntries(); entry++)
        {
            DataTree->GetEntry(entry);

            if (channelNumber == Chn)
            {
                FirstEntry = entry;
                LastCoarseTime = coarseTime;
                TrailerTree->GetEntry(dataPackId - 1);
                LastCoarseTimeOverflow = coarseTimeOverflow;
                LastEventCounterPerChannel = eventCounterPerCh;
                break;
            }
        }

        for (int entry = FirstEntry+1; entry < DataTree->GetEntries(); entry++)
        {
            DataTree->GetEntry(entry);
            
            if (channelNumber == Chn)
            {
                TrailerTree->GetEntry(dataPackId - 1);

                // if (LastEventCounterPerChannel != eventCounterPerCh)
                // {
                //     continue;
                // }
                // if (coarseTime < LastCoarseTime)
                // {
                //     continue;
                // }

                Distance =  coarseTime - LastCoarseTime;

                // if (Distance > 67108864)
                // {
                //     cout<<"Error Distance = "<<Distance<<endl;
                //     cout<<"Entry = "<<entry<<endl;
                //     break;
                // }
                
                if ((LastCoarseTimeOverflow == coarseTimeOverflow)&&(coarseTime > LastCoarseTime))
                {
                    DistanceHist->Fill(Distance);
                    DistanceHist_EvtCounter->Fill(Distance/(eventCounterPerCh-LastEventCounterPerChannel));
                    if (Chn == 0)
                    {
                        VectorEventCounterPerCh.push_back(eventCounterPerCh);
                        VectorEventCounterUser.push_back(EventCounterPerChannelUser);
                        EventCounterPerChannelUser++;
                    }
                    
                }
                LastCoarseTime = coarseTime;
                LastCoarseTimeOverflow = coarseTimeOverflow;
                LastEventCounterPerChannel = eventCounterPerCh;
            }
        }
        cout<<Chn<<endl;
    }
    inputfile->Close();

    outputfile->cd();
    TGraph *GraphEventCounterPerChannel = new TGraph(VectorEventCounterPerCh.size(),&VectorEventCounterPerCh[0],&VectorEventCounterUser[0]);
    DistanceHist->Write();
    DistanceHist_EvtCounter->Write();
    GraphEventCounterPerChannel->Write();

    outputfile->Close();
    
}