void CoarseTimeDistance(int target_line)
{
    ifstream inputfile_txt;
    inputfile_txt.open("/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/input_file_name.txt");
    string filename;
    int current_line = 0; // 当前行号
    string line;
    while (getline(inputfile_txt, filename)) 
    {
        if (current_line == target_line) 
        {
            line = "/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/root/"+filename;
            break; // 已经获取到目标行，跳出循环
        }
    current_line++; // 更新当前行号
    }
   
    TFile *inputfile = TFile::Open(line.c_str(),"read");
    TTree *DataTree    = (TTree*)inputfile->Get("EventData");
    TTree *TrailerTree = (TTree*)inputfile->Get("BlockTrailer");

    // string delimiter = "/";
    // size_t pos = 0;
    // string token;
    // while ((pos = line.find(delimiter)) != string::npos)
    // {
    //     line.erase(0, pos + delimiter.length());
    // }
    // pos = line.size()-4;
    // token = line.substr(0, pos);
    string OutputDirectory = "/junofs/users/chuziliang125/SPMT_data_analysis/V22Analysis/20231201_ABC_FW2.3/result/";
    string outputfilename = OutputDirectory+filename+"root";
        
    cout << "the outputfilename is "<<outputfilename<<endl;


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
    DataTree->SetBranchAddress("EventCounterPerChannelUser", &eventCounter);
    DataTree->SetBranchAddress("EventCounterPerChannel", &eventCounterPerCh);
    DataTree->SetBranchAddress("Charge", &chargeValue);
    DataTree->SetBranchAddress("FineTime", &fineTime);

    TrailerTree->SetBranchAddress("NumberChannels", &numChannels);
    TrailerTree->SetBranchAddress("NumberChannelsUser", &nChs);
    TrailerTree->SetBranchAddress("CoarseTimeOverflow", &coarseTimeOverflow);

    TFile *outputfile = TFile::Open(outputfilename.c_str(),"recreate");
    outputfile->cd();
    TH1 *DistanceHist = new TH1F("Coarse time distance between two hits","Distance",67108864,0,67108864);
    DistanceHist->GetXaxis()->SetTitle("CoarseTime");
    TH1 *DistanceHist_EvtCounter = new TH1F("Coarse time distance divided by EventCounterPerChannel","Coarse time distance divided by EventCounterPerChannel",67108864,0,67108864);
    DistanceHist_EvtCounter->GetXaxis()->SetTitle("CoarseTime");

    vector<int> VectorEventCounterPerCh[128];
    vector<int> VectorEventCounterUser[128];
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
                Distance =  coarseTime - LastCoarseTime;
                
                if ((LastCoarseTimeOverflow == coarseTimeOverflow)&&(coarseTime > LastCoarseTime))
                {
                    DistanceHist->Fill(Distance);
                    if (eventCounterPerCh > LastEventCounterPerChannel)
                    {
                        DistanceHist_EvtCounter->Fill(Distance/(eventCounterPerCh-LastEventCounterPerChannel));
                    }
                    else if (eventCounterPerCh < LastEventCounterPerChannel)
                    {
                        DistanceHist_EvtCounter->Fill(Distance/(eventCounterPerCh+2048-LastEventCounterPerChannel));
                    }
                }

                VectorEventCounterPerCh[Chn].push_back(eventCounterPerCh);
                VectorEventCounterUser[Chn].push_back(EventCounterPerChannelUser);
                EventCounterPerChannelUser++;

                LastCoarseTime = coarseTime;
                LastCoarseTimeOverflow = coarseTimeOverflow;
                LastEventCounterPerChannel = eventCounterPerCh;
            }
        }
        cout<<Chn<<endl;
    }
    inputfile->Close();

    outputfile->cd();

    TGraph *GraphEventCounterPerChannel[128];
    for (int i = 0; i < 128; i++)
    {
        GraphEventCounterPerChannel[i] = new TGraph(VectorEventCounterPerCh[i].size(),&VectorEventCounterUser[i][0],&VectorEventCounterPerCh[i][0]);
        GraphEventCounterPerChannel[i]->SetTitle(TString::Format("EventCounter (in data) to EventID (counted by program) Chn%1d",i));
        GraphEventCounterPerChannel[i]->GetXaxis()->SetTitle("EventID");
        GraphEventCounterPerChannel[i]->GetYaxis()->SetTitle("EventCounterPerChannel");
        GraphEventCounterPerChannel[i]->Write();
    }

    DistanceHist->Write();
    DistanceHist_EvtCounter->Write();
    
    outputfile->Close();
    
}