#include <stdio.h>
double RMSsum=0;
int gcucount=0;
void sum(TObject* obj)
{
    TKey* key=(TKey*)obj;
    
    TH1F* RMSHis = (TH1F*)key->ReadObj();
    for(int i=1;i<=128;i++)
    {
		RMSsum+=RMSHis->GetBinContent(i);
    }
    gcucount++;
}
void RMSmean()
{
    TFile* f1=new TFile("/junofs/users/chuziliang125/PED_analysis20230816/hist/run2322result.root");
    TDirectory* HighPingRMSDirec=f1->GetDirectory("hgPingRMS");
    TDirectory* HighPongRMSDirec=f1->GetDirectory("hgPongRMS");
    TList* HighPingRMSList=HighPingRMSDirec->GetListOfKeys();
    TList* HighPongRMSList=HighPongRMSDirec->GetListOfKeys();
    TIter HighPingiter(HighPingRMSList);
    std::for_each(HighPingiter.Begin(),HighPingiter.End(),sum);
    std::cout<<"High_Ping_RMSMean="<<RMSsum/(128*gcucount)<<std::endl;
    RMSsum=0;gcucount=0;
    TIter HighPongiter(HighPongRMSList);
    std::for_each(HighPongiter.Begin(),HighPongiter.End(),sum);
    std::cout<<"High_Pong_RMSMean="<<RMSsum/(128*gcucount)<<std::endl;
}


