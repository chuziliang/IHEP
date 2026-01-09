// RawData to Root file v5
//
// by Bei-Zhen Hu 2020.03.23
//
// USAGE: ./main datapath datalist.txt OutputFileName
//
// [datapath]: data path must wiht "/" in the end
//             ex: ~/path/to/data/
// 2020.11.12 update: cyNB = cycN[ iChnNo ][thisMode]; ==>> cyNB = cycN[ iChnNo ][thisMode]-1;
//

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "TH1F.h"
#include "TF1.h"
#include "TH1D.h"
#include "TH2D.h"
#include <TROOT.h>
#include <TFile.h>
#include <TChain.h>
#include <TMath.h>
#include "TStyle.h"

#include "dataDcode.h"
//#include <TApplication.h>
using namespace std;

#define lenOfHderPart1 4
#define lenOfHderPart2 23

unsigned int isHeader = 3405695742;
unsigned int Header_NumOfRChn;  // 1Byte
unsigned int testHeaderID;
unsigned int Header_EvtTimeStmp;  // 5Byte

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
    
    if( argc != 4 ) {

        std::cerr << "# of arguments error." << std::endl;
        std::cerr << "USAGE: ./main datapath datalist.txt OutputFileName" << std::endl;
        return 1;
    }


    
    string rawDataPath = argv[1];
    string runName[20];
    ifstream rawDataList;
    rawDataList.open(argv[2]);
    string markedF   = argv[3];
    
    int i=0;
    while (!rawDataList.eof()) {
        rawDataList>>runName[i];
        i++;
    }
    int fileNum = i-1;
    
    cout<<fileNum<<"\t"<<i<<" "<<runName[fileNum-1]<<endl;
    for (int lpFile=0; lpFile<fileNum; lpFile++) {
        int run_nb = atoi(runName[lpFile].substr(4,3).c_str() );
        
        string FileName = "/raw_data.bin"; ///all_raw_data.sci"; //
        ifstream myFile ((rawDataPath+runName[lpFile]+FileName).c_str(), ios::in | ios::binary);
        cout<<"test path: "<<(rawDataPath+runName[lpFile]+FileName).c_str()<<endl;
        ///////////////////////////////////
        
        ///////////////////////////////////////////////
        
        TFile *ofile = new TFile((markedF+"_"+runName[lpFile].substr(4,3)+".root").c_str(),"recreate");
        //TFile *ofile = new TFile(argv[3],"recreate");
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
        
        ofile->cd();
        ofile->mkdir("HighGain");
        ofile->cd("HighGain");
        TH1F *h_Adc_H[2][130];
        for (int iSign = 0; iSign<=1; iSign++) {    // ping:0, pong:1
            for (int iChn=0; iChn<128; iChn++) {
                h_Adc_H[iSign][iChn] = new TH1F(TString::Format("P%1d_Chn%1d_HG",iSign,iChn),
                                                TString::Format("P%1d_Chn%1d_HG",iSign,iChn),
                                                1000, 0, 1000);
            }
        }
        
        ofile->cd();
        ofile->mkdir("LowGain");
        ofile->cd("LowGain");
        TH1F *h_Adc_L[2][130];
        for (int iSign = 0; iSign<=1; iSign++) {
            for (int iChn=0; iChn<128; iChn++) {
                h_Adc_L[iSign][iChn] = new TH1F(TString::Format("P%1d_Chn%1d_LG",iSign,iChn),
                                                TString::Format("P%1d_Chn%1d_LG",iSign,iChn),
                                                1000, 0, 1000);
            }
        }
        ///////////////////////////////////////////////
        
        
        DataInfo testInfo;
        //struct stat results;
        char ABC_header1[lenOfHderPart1];
        char ABC_header2[lenOfHderPart2];
        
        int     loopTotalN = 0;
        unsigned int HeaderID = 0;
        int preTdc = 0;
        int cycN[500][2] = {0};
        while (!myFile.eof()) {
            
            myFile.read (ABC_header1, lenOfHderPart1);
            HeaderID = headerDcode(ABC_header1, lenOfHderPart1);
            
            if (HeaderID == 3405695742) {
                
                myFile.read (ABC_header2, lenOfHderPart2);
                Header_NumOfRChn = ABC_header2[5] & 0xFFu;
                //Header_EvtTimeStmp= headerDcode(ABC_header2, 5);
                //cout<<"TEST Check Time stamp: "<<Header_EvtTimeStmp<<endl;
                
                if (Header_NumOfRChn!=0) {
                    //unsigned int evtTimeStamp = headerDcode(ABC_header2, 5);
                    //cout<<"TEST Time Stamp: "<<loopTotalN<<"\t"<<evtTimeStamp<<"\t"<<Header_NumOfRChn<<endl;
                    
                    for (int loopN = 0; loopN<Header_NumOfRChn; loopN++) {
                        char ABC_Data[9];
                        myFile.read (ABC_Data, 9);
                        testInfo.inputData(ABC_Data,9);
                        
                        int iRunNo = run_nb;
                        int iChnNo = testInfo.ChnNum();
                        int thisFT = testInfo.FineTime();
                        int thisCT = testInfo.CoasTime();
                        
                        int thisADC  = testInfo.Charge();
                        int thisMode = testInfo.mode();
                        int thisRChnN= Header_NumOfRChn;
                        //cout<<"Check1: "<<loopN<<"\t"<<iRunNo<<"\t"<<iChnNo<<"\t"<<thisMode<<"\t"<<thisRChnN<<endl;
                        if (thisCT<=0) continue;
                        
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
                                if (preCT>thisCT) cycN[ iChnNo ][thisMode]++;// cycle counter.
                                //cout<<"Test1: "<<iRunNo<<"\t"<<iChnNo<<"\t"<<nEvt<<"\t"<<preCT<<"\t"<<cyNB<<endl;
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
                                if (preCT>thisCT) cycN[ iChnNo ][thisMode]++;// cycle counter.
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
                                ofile->cd();
                                ofile->cd("HighGain");
                                h_Adc_H[thisMode][iChnNo]->Fill(thisADC);
                            }
                            else{
                                preEvt.lhGain = 0;    //LG
                                ofile->cd();
                                ofile->cd("LowGain");
                                h_Adc_L[thisMode][iChnNo-128]->Fill(thisADC);
                            }
                            preEvt.nbOfRChn = thisRChnN;
                            if (thisMode==0) PrevEvtPing[ iChnNo ].push_back( preEvt );
                            if (thisMode==1) PrevEvtPong[ iChnNo ].push_back( preEvt );
                        
                        //}
                        
                    }
                }else{ // if NumOfRChn ==0, then skip it!
                    continue;
                }
                loopTotalN++;
            } else { // if != HeaderID, then skip it!
                continue;
            }
            
        }//while loop
        cout<<"loopTotalN: "<<loopTotalN<<endl;
        /////////
        PrevEvtPing.erase(PrevEvtPing.begin(), PrevEvtPing.end());
        PrevEvtPong.erase(PrevEvtPong.begin(), PrevEvtPong.end());
        myFile.close();
        
        ofile->cd();
        ofile->Write();
        aEvent->Write();
        ofile->Close();
    }
    
    
    
    
    return 0;
}

