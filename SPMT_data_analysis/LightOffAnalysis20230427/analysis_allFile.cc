// RawData to Root file v5 for IHEP DAVIS
//
// by Bei-Zhen Hu 2020.03.23
//
// USAGE: ./main datapath datalist.txt OutputFileName
// [datapath]: data path must wiht "/" in the end
//             ex: ~/path/to/data/
// 2020.11.12 update: cyNB = cycN[ iChnNo ][thisMode]; ==>> cyNB = cycN[ iChnNo ][thisMode]-1;
// 2021.1.26 Duplicated for IHEP DAVIS

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <TH1F.h>
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TMath.h>
#include "TStyle.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TGraph.h"
#include "TPaletteAxis.h"

#include "dataDcode.h"
//#include <TApplication.h>
using namespace std;

#define lenOfHderPart1 4
#define lenOfHderPart2 23

unsigned int isHeader = 3405695742;
unsigned int Header_NumOfRChn;  // 1Byte
unsigned int testHeaderID;
unsigned int Header_EvtTimeStmp;  // 5Byte

string FilePath = "binary/";

struct EventBrief {
    int runNB, chnNo;
    int rCT, rFT;
    int qADC, pipoM, lhGain;
    int nbOfRChn;
};

map<int /* RunNo */, vector< EventBrief > > PrevEvtPing;
map<int /* RunNo */, vector< EventBrief > > PrevEvtPong;

unsigned int headerDcode(char *charName, int len){
    
    testHeaderID = 0;
    int k = len;
    for (int i=0; i<len; i++) {
        k--;
        int shiftBit = 8*k;
        testHeaderID += ( (charName[i] & 0xFFu) << shiftBit );
    }
    return testHeaderID;
}



int main(int argc,char** argv) {
    
    if( argc != 2 ) {

        std::cerr << "# of arguments error." << std::endl;
        std::cerr << "USAGE: ./main datalist.txt" << std::endl;
        return 1;
    }


    
    //string rawDataPath = argv[1];
    string runName[400];
    string AllRunNumber[80];//定义所有的RunNumber
    int RunNumberCounter = 0;
    string AllGCU[10];//定义所有的GCU
    int GCUCounter = 0;
    ifstream rawDataList;
    rawDataList.open(argv[1]);
    //string markedF   = argv[3];
    
    int i=0;
    while (!rawDataList.eof()) {
        rawDataList>>runName[i];
        i++;
    }
    int TotFileNum = i;
    
    cout<<"Total file number : "<<TotFileNum<<endl;
    for(int fileNum = 0; fileNum<TotFileNum; fileNum++)
    {
        string SubRunName;//定义runName中有用的重要信息，如时间等，在输出文件名中保留这些信息
        string GCUnumber;//这个bin对应的GCU
        string RunNumber;//这个bin对应的RunNumber
        string ResultPathROOT;
        string ResultPathPDF;
        ResultPathROOT = "./Result/ROOT/";
        ResultPathPDF = "./Result/pdf/";
        size_t endPos = runName[fileNum].find_last_of('.');
        size_t startPos = runName[fileNum].find_last_of('/');
        SubRunName = runName[fileNum].substr(startPos + 1, endPos - startPos - 1);
        GCUnumber = runName[fileNum].substr(startPos + 1, 4);
        RunNumber = runName[fileNum].substr(endPos - 6, 4);

        //填充所有的RunNumber、GCU
        bool RunNumber_exist = false;
        for (int RunN = 0; RunN < RunNumberCounter; RunN++)
        {
            if (RunNumber == AllRunNumber[RunN])
            {
                RunNumber_exist = true;
                break;
            }
        }
        if (!RunNumber_exist)
        {
            AllRunNumber[RunNumberCounter] = RunNumber;
            RunNumberCounter++;
        }
        
        bool GCU_exist = false;
        for (int GCUN = 0; GCUN < GCUCounter; GCUN++)
        {
            if (GCUnumber == AllGCU[GCUN])
            {
                GCU_exist = true;
                break;
            }
        }
        if (!GCU_exist)
        {
            AllGCU[GCUCounter] = GCUnumber;
            GCUCounter++;
        }
        

        //TFile *ofile = new TFile((ResultPath+SubRunName+markedF+".root").c_str(),"recreate");
        TFile *ofile = new TFile((ResultPathROOT+SubRunName+".root").c_str(),"recreate");
        //TFile *ofile = new TFile(argv[3],"recreate");

        //定义输出的Tree、图像等
        TTree *aEvent = new TTree("Event","Event Tree");    /// create tree
        int RunNo = 0;
        int ChnN = 0;
        int ADCU = 0;
        int FT   = 0;
        int CT   = 0;
        int mode = 0;
        int nRChn= 0;
        int gLH  = 0;
        int cyNB = 0;
        
        aEvent->Branch("RunNo",    &RunNo, "RunNo/I");
        aEvent->Branch("ChnN",     &ChnN,  "ChnN/I");
        aEvent->Branch("ADCU",     &ADCU,  "ADCU/I");
        aEvent->Branch("FT",       &FT  ,  "FT/I");
        aEvent->Branch("CT",       &CT  ,  "CT/I");
        aEvent->Branch("mode",     &mode,  "mode/I");
        aEvent->Branch("gLH",      &gLH,   "gLH/I");
        aEvent->Branch("nRChn",    &nRChn, "nRChn/I");
        aEvent->Branch("cyNB",     &cyNB,  "cyNB/I");
        
        ///////////////////////////////////////////////
        TH2 *ohistPing = new TH2F("charges HG ping","charges HG ping",128,0,127,1024,0,1024);
        TH2 *ohistPong = new TH2F("charges HG pong","charges HG pong",128,0,127,1024,0,1024);
        TH2 *ohistPingLG = new TH2F("charges LG ping","charges LG ping",128,0,127,1024,0,1024);
        TH2 *ohistPongLG = new TH2F("charges LG pong","charges LG pong",128,0,127,1024,0,1024);

        ofile->cd();
        ofile->mkdir("HighGain");
        ofile->cd("HighGain");
        TH1F *h_Adc_H[2][130];
        for (int iSign = 0; iSign<=1; iSign++) {    // ping:0, pong:1
            for (int iChn=0; iChn<128; iChn++) {
                if (iSign == 0)
                {
                    h_Adc_H[iSign][iChn] = new TH1F(TString::Format("Ping_Chn%1d_HG",iChn),
                                                TString::Format("Ping_Chn%1d_HG",iChn),
                                                1024, 0, 1024);
                    h_Adc_H[iSign][iChn]->GetXaxis()->SetTitle("ADCu");
                }
                else
                {
                    h_Adc_H[iSign][iChn] = new TH1F(TString::Format("Pong_Chn%1d_HG",iChn),
                                                TString::Format("Pong_Chn%1d_HG",iChn),
                                                1024, 0, 1024);
                    h_Adc_H[iSign][iChn]->GetXaxis()->SetTitle("ADCu");
                }
                
            }
        }
        
        ofile->cd();
        ofile->mkdir("LowGain");
        ofile->cd("LowGain");
        TH1F *h_Adc_L[2][130];
        for (int iSign = 0; iSign<=1; iSign++) {
            for (int iChn=0; iChn<128; iChn++) {
                if (iSign == 0)
                {
                    h_Adc_L[iSign][iChn] = new TH1F(TString::Format("Ping_Chn%1d_LG",iChn),
                                                TString::Format("Ping_Chn%1d_LG",iChn),
                                                1024, 0, 1024);
                    h_Adc_L[iSign][iChn]->GetXaxis()->SetTitle("ADCu");
                }
                else
                {
                    h_Adc_L[iSign][iChn] = new TH1F(TString::Format("Pong_Chn%1d_LG",iChn),
                                                TString::Format("Pong_Chn%1d_LG",iChn),
                                                1024, 0, 1024);
                    h_Adc_L[iSign][iChn]->GetXaxis()->SetTitle("ADCu");
                }
            }
        }

        ofile->cd();
        ofile->mkdir("CoarseTime");
        ofile->cd("CoarseTime");
        TGraph *CoarseTime[2][256];
        for (int iSign = 0; iSign<=1; iSign++) {    // ping:0, pong:1
            for (int iChn=0; iChn<256; iChn++) {
                if (iSign == 0)
                {
                    CoarseTime[iSign][iChn] = new TGraph();
                    CoarseTime[iSign][iChn]->SetName(TString::Format("Ping Chn%1d CoarseTime wrt Evt_ID",iChn));
                    CoarseTime[iSign][iChn]->SetTitle(TString::Format("Ping Chn%1d CoarseTime wrt Evt_ID",iChn));
                    CoarseTime[iSign][iChn]->GetXaxis()->SetTitle("Evt_ID");
                    CoarseTime[iSign][iChn]->GetYaxis()->SetTitle("Coarse Time");
                }
                else
                {
                    CoarseTime[iSign][iChn] = new TGraph();
                    CoarseTime[iSign][iChn]->SetName(TString::Format("Pong Chn%1d CoarseTime wrt Evt_ID",iChn));
                    CoarseTime[iSign][iChn]->SetTitle(TString::Format("Pong Chn%1d CoarseTime wrt Evt_ID",iChn));
                    CoarseTime[iSign][iChn]->GetXaxis()->SetTitle("Evt_ID");
                    CoarseTime[iSign][iChn]->GetYaxis()->SetTitle("Coarse Time");
                }
                
            }
        }
        //--------------------------------定义输出的Tree、图像等结束-------------------------------------------------
        
        //for (int lpFile=0; lpFile<fileNum; lpFile++)
        {
            int run_nb = stoi(RunNumber);
            
            //string FileName = "/raw_data.bin"; ///all_raw_data.sci"; //
            ifstream myFile ((FilePath + runName[fileNum]).c_str(), ios::in | ios::binary); //remove FileName for DAVIS
            //TFile *myFile = TFile::Open((runName[fileNum]+"?filetype=raw").c_str(),"READ");
        if (!myFile)
        {
            cout<<"File not exist!"<<endl;
            return 1;
        }
        else
        {
            cout<<"File exist"<<endl;
        }
            cout<<"This file number : "<<fileNum+1<<endl;
            cout<<"test path: "<<(runName[fileNum]).c_str()<<endl;
            ///////////////////////////////////
            
            ///////////////////////////////////////////////
            

            ///////////////////////////////////////////////
            
            
            DataInfo testInfo;
            //struct stat results;
            char ABC_header1[lenOfHderPart1];
            char ABC_header2[lenOfHderPart2];
            
            int     loopTotalN = 0;
            unsigned int HeaderID = 0;
            int preTdc = 0;
            int cycN[500][2] = {0};
            //myFile->SetFileBytesRead(0);
            while (!myFile.eof()) 
            //Long64_t myFileSize = myFile->GetSize();
            //while (myFile->GetFileBytesRead()<myFileSize)
            {
                
                myFile.read (ABC_header1, lenOfHderPart1);
                // if(myFile->ReadBuffer(ABC_header1,lenOfHderPart1))
                // {
                //     cout<<"Read buffer error!!!!!!!!!!"<<endl;
                //     break;
                // }
                HeaderID = headerDcode(ABC_header1, lenOfHderPart1);
                
                if (HeaderID == 3405695742) {
                    loopTotalN++;
                    myFile.read (ABC_header2, lenOfHderPart2);
                    // if(myFile->ReadBuffer(ABC_header2,lenOfHderPart2))
                    // {
                    //     cout<<"Read buffer error!!!!!!!!!!"<<endl;
                    //     break;
                    // }

                    Header_NumOfRChn = ABC_header2[5] & 0xFFu;
                    //Header_EvtTimeStmp= headerDcode(ABC_header2, 5);
                    //cout<<"TEST Check Time stamp: "<<Header_EvtTimeStmp<<endl;
                    
                    if (Header_NumOfRChn!=0) {
                        //unsigned int evtTimeStamp = headerDcode(ABC_header2, 5);
                        //cout<<"TEST Time Stamp: "<<loopTotalN<<"\t"<<evtTimeStamp<<"\t"<<Header_NumOfRChn<<endl;
                        
                        for (int loopN = 0; loopN<Header_NumOfRChn; loopN++) {
                            char ABC_Data[9];
                            myFile.read (ABC_Data, 9);
                            // if(myFile->ReadBuffer(ABC_Data,9))
                            // {
                            //     cout<<"Read buffer error!!!!!!!!!!"<<endl;
                            //     break;
                            // }

                            testInfo.inputData(ABC_Data,9);
                            
                            int iRunNo = run_nb;
                            int iChnNo = testInfo.ChnNum();
                            int thisFT = testInfo.FineTime();
                            int thisCT = testInfo.CoasTime();
                            
                            int thisADC  = testInfo.Charge();
                            int thisMode = testInfo.mode();
                            int thisRChnN= Header_NumOfRChn;
                            //cout<<"Check1: "<<loopN<<"\t"<<iRunNo<<"\t"<<iChnNo<<"\t"<<thisMode<<"\t"<<thisRChnN<<endl;
                            if (thisCT<0) continue;
                            
                            if (thisMode==0){
                                int nEvt = PrevEvtPing[ iChnNo ].size();
                                //cout<<"Check INFO: "<<loopTotalN<<"\t"<<iChnNo<<"\t"<<nEvt<<"\t"<<cycN[ iChnNo ][thisMode]<<endl;
                                if (nEvt==1) {
                                    int preCT = PrevEvtPing[ iChnNo ][0].rCT;
                                    
                                    
                                    cyNB = cycN[ iChnNo ][thisMode];
                                    RunNo= PrevEvtPing[ iChnNo ][0].runNB;
                                    ChnN = PrevEvtPing[ iChnNo ][0].chnNo;
                                    if (ChnN<128)   gLH = 1;    //HG
                                    else            gLH = 0;    //LG
                                    
                                    ADCU = PrevEvtPing[ iChnNo ][0].qADC;
                                    FT   = PrevEvtPing[ iChnNo ][0].rFT;
                                    CT   = PrevEvtPing[ iChnNo ][0].rCT;
                                    mode = PrevEvtPing[ iChnNo ][0].pipoM;
                                    nRChn= PrevEvtPing[ iChnNo ][0].nbOfRChn;
                                    aEvent->Fill();
                                    //cout<<"Test1: "<<iRunNo<<"\t"<<iChnNo<<"\t"<<nEvt<<"\t"<<preCT<<"\t"<<cyNB<<endl;
                                    //if (preCT>thisCT) cycN[ iChnNo ][thisMode]++;// cycle counter.
                                    int dT = thisCT-preCT;
                                    if (dT<-60000000) cycN[ iChnNo ][thisMode]++;// cycle counter.
                                    vector<EventBrief>::iterator k =  PrevEvtPing[ iChnNo ].begin();
                                    PrevEvtPing[ iChnNo ].erase(k);
                                    //cout<<"check size again: "<<PrevEvtPing[ iChnNo ].size()<<endl;
                                }
                            }
                            if (thisMode==1){
                                int nEvt = PrevEvtPong[ iChnNo ].size();
                                if (nEvt==1) {
                                    int preCT = PrevEvtPong[ iChnNo ][0].rCT;
                                    
                                    cyNB = cycN[ iChnNo ][thisMode];
                                    RunNo= PrevEvtPong[ iChnNo ][0].runNB;
                                    ChnN = PrevEvtPong[ iChnNo ][0].chnNo;
                                    if (ChnN<128)   gLH = 1;    //HG
                                    else            gLH = 0;    //LG
                                    
                                    ADCU = PrevEvtPong[ iChnNo ][0].qADC;
                                    FT   = PrevEvtPong[ iChnNo ][0].rFT;
                                    CT   = PrevEvtPong[ iChnNo ][0].rCT;
                                    mode = PrevEvtPong[ iChnNo ][0].pipoM;
                                    nRChn= PrevEvtPong[ iChnNo ][0].nbOfRChn;
                                    aEvent->Fill();
                                    //if (preCT>thisCT) cycN[ iChnNo ][thisMode]++;// cycle counter.
                                    int dT = thisCT-preCT;
                                    if (dT<-60000000) cycN[ iChnNo ][thisMode]++;// cycle counter.
                                    vector<EventBrief>::iterator k =  PrevEvtPong[ iChnNo ].begin();
                                    PrevEvtPong[ iChnNo ].erase(k);
                                }
                            }

                            
                            //if (thisCT>0) {
                                EventBrief preEvt;
                                preEvt.runNB = iRunNo;
                                preEvt.chnNo = iChnNo;
                                preEvt.rCT   = thisCT;
                                preEvt.rFT   = thisFT;
                                preEvt.qADC  = thisADC;
                                preEvt.pipoM = thisMode;

                                

                                if (iChnNo<128)   {
                                    preEvt.lhGain = 1;    //HG
                                    if (thisMode == 0)
                                    {
                                        ohistPing->Fill(iChnNo,thisADC);
                                    }
                                    else
                                    {
                                        ohistPong->Fill(iChnNo,thisADC);
                                    }

                                    ofile->cd();
                                    ofile->cd("HighGain");
                                    h_Adc_H[thisMode][iChnNo]->Fill(thisADC);
                                    ofile->cd();
                                    ofile->cd("CoarseTime");
                                    CoarseTime[thisMode][iChnNo]->AddPoint(loopTotalN,thisCT);

                                }
                                else{
                                    preEvt.lhGain = 0;    //LG

                                    if (thisMode == 0)
                                    {
                                        ohistPingLG->Fill(iChnNo-128,thisADC);
                                    }
                                    else
                                    {
                                        ohistPongLG->Fill(iChnNo-128,thisADC);
                                    }
                                    ofile->cd();
                                    ofile->cd("LowGain");
                                    h_Adc_L[thisMode][iChnNo-128]->Fill(thisADC);
                                    ofile->cd();
                                    ofile->cd("CoarseTime");
                                    CoarseTime[thisMode][iChnNo]->AddPoint(loopTotalN,thisCT);
                                }
                                preEvt.nbOfRChn = thisRChnN;
                                if (thisMode==0) PrevEvtPing[ iChnNo ].push_back( preEvt );
                                if (thisMode==1) PrevEvtPong[ iChnNo ].push_back( preEvt );
                            
                            //}
                            
                        }
                    }else{ // if NumOfRChn ==0, then skip it!
                        continue;
                    }
                    //loopTotalN++;
                } else { // if != HeaderID, then skip it!
                    continue;
                }
                // double prograss;
                // prograss = 100.0*myFile->GetFileBytesRead()/myFileSize;
                // cout<<"Analysis prograss: "<< std::fixed << std::setprecision(2)<<prograss<<"%"<<flush;
                // cout<<"\r";
            }//while loop
            // cout<<"\n";
            cout<<"loopTotalN: "<<loopTotalN<<endl;
            /////////
            PrevEvtPing.erase(PrevEvtPing.begin(), PrevEvtPing.end());
            PrevEvtPong.erase(PrevEvtPong.begin(), PrevEvtPong.end());
            myFile.close();
        
        }
        
        ofile->cd();
        ofile->Write();
        TString CanvasName = "c1"+SubRunName;
        TCanvas *c1 = new TCanvas(CanvasName,CanvasName,1);
        c1->Divide(2,1);
        c1->cd(1);
        gPad->SetLogz();
        ohistPing->GetXaxis()->SetTitle("Channel ID");
        ohistPing->GetYaxis()->SetTitle("ADCu");
        ohistPing->GetYaxis()->SetLabelFont( 63  );
        ohistPing->GetYaxis()->SetLabelSize( 11 );
        //gStyle->SetOptStat(kFALSE);
        ohistPing->Draw("COLZ");
        c1->Update();
        TPaletteAxis *palette1 = (TPaletteAxis*)ohistPing->GetListOfFunctions()->FindObject("palette");
        if (palette1 != nullptr)
        {
            palette1->SetX1NDC(0.902);
            palette1->SetX2NDC(0.932);
            c1->Modified();
            c1->Update();
        }
        c1->cd(2);
        gPad->SetLogz();
        ohistPong->GetXaxis()->SetTitle("Channel ID");
        ohistPong->GetYaxis()->SetTitle("ADCu");
        ohistPong->GetYaxis()->SetLabelFont( 63  );
        ohistPong->GetYaxis()->SetLabelSize( 11 );
        //gStyle->SetOptStat(kFALSE);
        ohistPong->Draw("COLZ");
        c1->Update();
        TPaletteAxis *palette2 = (TPaletteAxis*)ohistPong->GetListOfFunctions()->FindObject("palette");
        if (palette2 != nullptr)
        {
            palette2->SetX1NDC(0.902);
            palette2->SetX2NDC(0.932);
            c1->Modified();
            c1->Update();
        }      

        CanvasName = "c2"+SubRunName;
        TCanvas *c2 = new TCanvas(CanvasName,CanvasName,1);
        c2->Divide(2,1);
        c2->cd(1);
        gPad->SetLogz();
        ohistPingLG->GetXaxis()->SetTitle("Channel ID");
        ohistPingLG->GetYaxis()->SetTitle("ADCu");
        ohistPingLG->GetYaxis()->SetLabelFont( 63  );
        ohistPingLG->GetYaxis()->SetLabelSize( 11 );
        //gStyle->SetOptStat(kFALSE);
        ohistPingLG->Draw("COLZ");
        c2->Update();
        TPaletteAxis *palette3 = (TPaletteAxis*)ohistPingLG->GetListOfFunctions()->FindObject("palette");
        if (palette3 != nullptr)
        {
            palette3->SetX1NDC(0.902);
            palette3->SetX2NDC(0.932);
            c2->Modified();
            c2->Update();
        }     

        c2->cd(2);
        gPad->SetLogz();
        ohistPongLG->GetXaxis()->SetTitle("Channel ID");
        ohistPongLG->GetYaxis()->SetTitle("ADCu");
        ohistPongLG->GetYaxis()->SetLabelFont( 63  );
        ohistPongLG->GetYaxis()->SetLabelSize( 11 );
        //gStyle->SetOptStat(kFALSE);
        ohistPongLG->Draw("COLZ");
        c2->Update();
        TPaletteAxis *palette4 = (TPaletteAxis*)ohistPongLG->GetListOfFunctions()->FindObject("palette");
        if (palette4 != nullptr)
        {
            palette4->SetX1NDC(0.902);
            palette4->SetX2NDC(0.932);
            c2->Modified();
            c2->Update();
        }

        c1->Print((ResultPathPDF+SubRunName+".pdf(").c_str(),"pdf");
        c2->Print((ResultPathPDF+SubRunName+".pdf").c_str(),"pdf");

        ofile->mkdir("canvas");
        ofile->cd();
        ofile->cd("canvas");
        c1->Write();
        c2->Write();

        TCanvas *TH2All;
        CanvasName = "TH2All"+SubRunName;
        TH2All = new TCanvas(CanvasName);
        TH2All->Divide(2,1);
        TString histtitle;
        if (fileNum == 0)
        {
            TH2All->Print("Result/ChargePingAllRun.pdf[");
        }
        TH2All->cd(1);
        gPad->SetLogz();
        histtitle = "GU"+GCUnumber +" Run" + RunNumber+" charges HG ping";
        ohistPing->SetTitle(histtitle);
        ohistPing->Draw("COLZ");
        TH2All->Update();
        TH2All->cd(2);
        gPad->SetLogz();
        histtitle = "GU"+GCUnumber +" Run" + RunNumber+" charges LG ping";
        ohistPingLG->SetTitle(histtitle);
        ohistPingLG->Draw("COLZ");
        TH2All->Update();
        TH2All->Print("Result/ChargePingAllRun.pdf");
        if (fileNum + 1 == TotFileNum)
        {
            TH2All->Print("Result/ChargePingAllRun.pdf]");
        }

        
        TH1* HistRMSPingHG = new TH1F("RMS Ping HG","RMS Ping HG",128,0,128);
        TH1* HistRMSPongHG = new TH1F("RMS Pong HG","RMS Pong HG",128,0,128);
        TH1* HistRMSPingLG = new TH1F("RMS Ping LG","RMS Ping LG",128,0,128);
        TH1* HistRMSPongLG = new TH1F("RMS Pong LG","RMS Pong LG",128,0,128);

        TH1* HistMeanPingHG = new TH1F("Mean Ping HG","Mean Ping HG",128,0,128);
        TH1* HistMeanPongHG = new TH1F("Mean Pong HG","Mean Pong HG",128,0,128);
        TH1* HistMeanPingLG = new TH1F("Mean Ping LG","Mean Ping LG",128,0,128);
        TH1* HistMeanPongLG = new TH1F("Mean Pong LG","Mean Pong LG",128,0,128);
        for (int iSign = 0; iSign<=1; iSign++) {    // ping:0, pong:1
            for (int iChn=0; iChn<128; iChn++) {
                if(iSign == 0)
                {
                    double RMSHG = h_Adc_H[iSign][iChn] -> GetRMS();
                    HistRMSPingHG->SetBinContent(iChn+1,RMSHG);
                    double RMSLG = h_Adc_L[iSign][iChn] -> GetRMS();
                    HistRMSPingLG->SetBinContent(iChn+1,RMSLG);
                    double MeanHG = h_Adc_H[iSign][iChn] -> GetMean();
                    HistMeanPingHG->SetBinContent(iChn+1,MeanHG);
                    double MeanLG = h_Adc_L[iSign][iChn] -> GetMean();
                    HistMeanPingLG->SetBinContent(iChn+1,MeanLG);
                }
                else if(iSign == 1)
                {
                    double RMSHG = h_Adc_H[iSign][iChn] -> GetRMS();
                    HistRMSPongHG->SetBinContent(iChn+1,RMSHG);
                    double RMSLG = h_Adc_L[iSign][iChn] -> GetRMS();
                    HistRMSPongLG->SetBinContent(iChn+1,RMSLG);
                    double MeanHG = h_Adc_H[iSign][iChn] -> GetMean();
                    HistMeanPongHG->SetBinContent(iChn+1,MeanHG);
                    double MeanLG = h_Adc_L[iSign][iChn] -> GetMean();
                    HistMeanPongLG->SetBinContent(iChn+1,MeanLG);

                }
            }
        }

        HistMeanPingHG->GetYaxis()->SetRangeUser(0,140);
        HistMeanPongHG->GetYaxis()->SetRangeUser(0,140);
        HistMeanPingLG->GetYaxis()->SetRangeUser(0,140);
        HistMeanPongLG->GetYaxis()->SetRangeUser(0,140);

        HistRMSPingHG->GetYaxis()->SetRangeUser(0,14);
        HistRMSPongHG->GetYaxis()->SetRangeUser(0,14);
        HistRMSPingLG->GetYaxis()->SetRangeUser(0,14);
        HistRMSPongLG->GetYaxis()->SetRangeUser(0,14);

        TCanvas *c5;
        CanvasName = "c5"+SubRunName;
        c5 = new TCanvas(CanvasName);
        c5->Divide(2,1);
        c5->cd(1);
        HistRMSPingHG->Draw();
        c5->Update();
        c5->cd(2);
        HistRMSPongHG->Draw();
        c5->Update();
        ofile->cd();
        ofile->cd("canvas");
        c5->Write();
        c5->Print((ResultPathPDF+SubRunName+".pdf").c_str(),"pdf");

        TCanvas *c6;
        CanvasName = "c6"+SubRunName;
        c6 = new TCanvas(CanvasName);
        c6->Divide(2,1);
        c6->cd(1);
        HistRMSPingLG->Draw();
        c6->Update();
        c6->cd(2);
        HistRMSPongLG->Draw();
        c6->Update();
        ofile->cd();
        ofile->cd("canvas");
        c6->Write();
        c6->Print((ResultPathPDF+SubRunName+".pdf").c_str(),"pdf");

        TCanvas *c7;
        CanvasName = "c7"+SubRunName;
        c7 = new TCanvas(CanvasName);
        c7->Divide(2,1);
        c7->cd(1);
        HistMeanPingHG->Draw();
        c7->Update();
        c7->cd(2);
        HistMeanPongHG->Draw();
        c7->Update();
        ofile->cd();
        ofile->cd("canvas");
        c7->Write();
        c7->Print((ResultPathPDF+SubRunName+".pdf").c_str(),"pdf");

        TCanvas *c8;
        CanvasName = "c8"+SubRunName;
        c8 = new TCanvas(CanvasName);
        c8->Divide(2,1);
        c8->cd(1);
        HistMeanPingLG->Draw();
        c8->Update();
        c8->cd(2);
        HistMeanPongLG->Draw();
        c8->Update();
        ofile->cd();
        ofile->cd("canvas");
        c8->Write();
        c8->Print((ResultPathPDF+SubRunName+".pdf").c_str(),"pdf");


        TCanvas *c3;
        CanvasName = "c3"+SubRunName;
        c3 = new TCanvas(CanvasName);
        c3->Divide(4,4);
        for (int iSign = 0; iSign<=1; iSign++) {    // ping:0, pong:1
            for (int iChn=0; iChn<128; iChn++) {
                c3->cd((iChn%16)+1);
                //gPad->SetLogy(1);
                h_Adc_H[iSign][iChn] -> GetXaxis()->SetRangeUser(0,400);
                h_Adc_H[iSign][iChn] -> Draw();
                //h_Adc_H[iSign][iChn] -> GetXaxis()->SetRange(0,400);
                // if ((iChn%16)<9)
                // {
                //     h_Adc_H[iSign][iChn]->SetLineColor((iChn%16)+1);
                // }
                // if ((iChn%16)>=9)
                // {
                //     h_Adc_H[iSign][iChn]->SetLineColor(((iChn%16)-7)*10);//设置图像线的颜色。10-20是不同的灰度，所以这里单独设置
                // }
                if (iChn%16 == 15)
                {
                    c3->Print((ResultPathPDF+SubRunName+".pdf").c_str(),"pdf");
                    ofile->cd();
                    ofile->cd("canvas");
                    c3->Write();
                }
            }
        }
        
        for (int iSign = 0; iSign<=1; iSign++) {
            for (int iChn=0; iChn<128; iChn++) {
                c3->cd((iChn%16)+1);
                //gPad->SetLogy();
                h_Adc_L[iSign][iChn] -> Draw();
                // if ((iChn%16)<9)
                // {
                //     h_Adc_L[iSign][iChn]->SetLineColor((iChn%16)+1);
                // }
                // if ((iChn%16)>=9)
                // {
                //     h_Adc_L[iSign][iChn]->SetLineColor(((iChn%16)-7)*10);
                // }
                if (iSign == 1 && iChn == 127)
                {
                    c3->Print((ResultPathPDF+SubRunName+".pdf)").c_str(),"pdf");
                    // ofile->cd();
                    // ofile->cd("canvas");
                    //c3->Write();
                }
                else if (iChn%16 == 15)
                {
                    c3->Print((ResultPathPDF+SubRunName+".pdf").c_str(),"pdf");
                    ofile->cd();
                    ofile->cd("canvas");
                    c3->Write();
                }
            }
        }

        TCanvas *c4;
        CanvasName = "c4"+SubRunName;
        c4 = new TCanvas(CanvasName);
        c4->Divide(4,4);
        
        // for (int iSign = 0; iSign<=1; iSign++) {
        //     for (int iChn=0; iChn<256; iChn++) {
        //         c4->cd((iChn%16)+1);
        //         //gPad->SetLogy();
        //         gPad->Clear();
        //         CoarseTime[iSign][iChn] -> Draw();
        //         // if ((iChn%16)<9)
        //         // {
        //         //     h_Adc_L[iSign][iChn]->SetLineColor((iChn%16)+1);
        //         // }
        //         // if ((iChn%16)>=9)
        //         // {
        //         //     h_Adc_L[iSign][iChn]->SetLineColor(((iChn%16)-7)*10);
        //         // }
        //         if (iSign == 1 && iChn == 255)
        //         {
        //             c4->Print((ResultPath+SubRunName+markedF+".pdf)").c_str(),"pdf");
        //             // ofile->cd();
        //             // ofile->cd("canvas");
        //             //c3->Write();
        //         }
        //         else if (iChn%16 == 15)
        //         {
        //             c4->Print((ResultPath+SubRunName+markedF+".pdf").c_str(),"pdf");
        //             // ofile->cd();
        //             // ofile->cd("canvas");
        //             //c3->Write();
        //         }
        //     }
        // }

        for (int iSign = 0; iSign<=1; iSign++) {    // ping:0, pong:1
            for (int iChn=0; iChn<256; iChn++) {
                ofile->cd();
                ofile->cd("CoarseTime");
                CoarseTime[iSign][iChn]->Write();
            }
        }
        
        //aEvent->Write();
        cout<<"-------------------------------------------"<<endl;
        ofile->Close();

        delete c1;
        delete c2;
        delete c3;
        delete c4;
        delete c5;
        delete c6;
        delete c7;
        delete c8;
        delete TH2All;
    }

    return 0;
}
