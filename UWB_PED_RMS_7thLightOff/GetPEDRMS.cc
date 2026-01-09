#include <iostream>
#include <fstream>

std::string getSubstringBetween(std::string str, char startChar, char endChar) {
    size_t endPos = str.rfind(endChar);
    if (endPos == std::string::npos) {
        return ""; // 如果找不到结束字符，返回空字符串
    }

    size_t startPos = str.rfind(startChar, endPos);
    if (startPos == std::string::npos) {
        return ""; // 如果找不到起始字符，返回空字符串
    }

    return str.substr(startPos + 1, endPos - startPos - 1);
}


void GetPEDRMS(TString filepath)
{
    string filename = getSubstringBetween((string)filepath,'/','.');
   
    TFile *inputfile = TFile::Open(filepath,"read");
    TTree *DataTree = (TTree*)inputfile->Get("EventData");

    string OutputDirectory = "/junofs/users/chuziliang125/UWB_PED_RMS_7thLightOff/10th_LightOn/result/";
    string outputfilename = OutputDirectory+filename+".txt";
        
    cout << "the outputfilename is "<<outputfilename<<endl;
    std::ofstream outputFile(outputfilename,std::ios::trunc);
    outputFile << "GCUId" << "\t"
        << "Chn" << "\t"
        << "Mean_Ping_HG" << "\t"
        << "Mean_Pong_HG" << "\t"
        << "RMS_Ping_HG" << "\t"
        << "RMS_Pong_HG" << "\t"
        << "Mean_Ping_LG" << "\t"
        << "Mean_Pong_LG" << "\t"
        << "RMS_Ping_LG" << "\t"
        << "RMS_Pong_LG" << "\t"
        << "Length" << "\t"
        << "x" << "\t"
        << "y" << "\t"
        << "z" <<std::endl;

    uint32_t channelNumber;
    uint32_t gain;
    uint32_t chargeValue;
    uint32_t sign;
    double x,y,z;
    uint32_t CableLength;
    uint32_t GCUId;

    DataTree->SetBranchAddress("ABCChannelNumber", &channelNumber);
    DataTree->SetBranchAddress("Gain", &gain);
    DataTree->SetBranchAddress("Sign", &sign);
    DataTree->SetBranchAddress("Charge", &chargeValue);
    DataTree->SetBranchAddress("X", &x);
    DataTree->SetBranchAddress("Y", &y);
    DataTree->SetBranchAddress("Z", &z);
    DataTree->SetBranchAddress("CableLength", &CableLength);
    DataTree->SetBranchAddress("GCUId", &GCUId);

    TH1 *PEDHist_ping_HG[128];
    TH1 *PEDHist_pong_HG[128];
    TH1 *PEDHist_ping_LG[128];
    TH1 *PEDHist_pong_LG[128];
    double Output_x[128];
    double Output_y[128];
    double Output_z[128];
    int Length[128];
    for (int Chn = 0; Chn < 128; Chn++)
    {
        TString HistName = TString::Format("PEDHist_Chn%1d",Chn);
        PEDHist_ping_HG[Chn] = new TH1F(HistName+(TString)"_Ping_HG",HistName+(TString)"_Ping_HG",512,0,512);
        PEDHist_pong_HG[Chn] = new TH1F(HistName+(TString)"_Pong_HG",HistName+(TString)"_Pong_HG",512,0,512);
        PEDHist_ping_LG[Chn] = new TH1F(HistName+(TString)"_Ping_LG",HistName+(TString)"_Ping_LG",512,0,512);
        PEDHist_pong_LG[Chn] = new TH1F(HistName+(TString)"_Pong_LG",HistName+(TString)"_Pong_LG",512,0,512);
        Output_x[Chn] = 0;
        Output_y[Chn] = 0;
        Output_z[Chn] = 0;
        Length[Chn] = 0;
    }

    for (int entry = 0; entry < DataTree->GetEntries(); entry++)
    {
        DataTree->GetEntry(entry);

        if (gain == 1 && sign == 0)
        {
            PEDHist_ping_HG[channelNumber]->Fill(chargeValue);
        }
        else if (gain == 1 && sign == 1)
        {
            PEDHist_pong_HG[channelNumber]->Fill(chargeValue);
        }
        if (gain == 0 && sign == 0)
        {
            PEDHist_ping_LG[channelNumber]->Fill(chargeValue);
        }
        else if (gain == 0 && sign == 1)
        {
            PEDHist_pong_LG[channelNumber]->Fill(chargeValue);
        }

        if (Output_x[channelNumber] == 0 || Output_y[channelNumber] == 0 || Output_z[channelNumber] == 0)
        {
            Output_x[channelNumber] = x;
            Output_y[channelNumber] = y;
            Output_z[channelNumber] = z;
            Length[channelNumber] = CableLength;
        }
    }

    for (int Chn = 0; Chn < 128; Chn++)
    {
        double Mean_Ping_HG = PEDHist_ping_HG[Chn]->GetMean();
        double Mean_Pong_HG = PEDHist_pong_HG[Chn]->GetMean();
        double RMS_Ping_HG = PEDHist_ping_HG[Chn]->GetRMS();
        double RMS_Pong_HG = PEDHist_pong_HG[Chn]->GetRMS();

        double Mean_Ping_LG = PEDHist_ping_LG[Chn]->GetMean();
        double Mean_Pong_LG = PEDHist_pong_LG[Chn]->GetMean();
        double RMS_Ping_LG = PEDHist_ping_LG[Chn]->GetRMS();
        double RMS_Pong_LG = PEDHist_pong_LG[Chn]->GetRMS();
        outputFile << GCUId << "\t"
        << Chn << "\t"
        << Mean_Ping_HG << "\t"
        << Mean_Pong_HG << "\t"
        << RMS_Ping_HG << "\t"
        << RMS_Pong_HG << "\t"
        << Mean_Ping_LG << "\t"
        << Mean_Pong_LG << "\t"
        << RMS_Ping_LG << "\t"
        << RMS_Pong_LG << "\t"
        << Length[Chn] << "\t"
        << Output_x[Chn] << "\t"
        << Output_y[Chn] << "\t"
        << Output_z[Chn] <<std::endl;
    }

    outputFile.close();
    inputfile->Close();
}
