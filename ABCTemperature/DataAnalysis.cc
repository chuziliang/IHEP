#define DataLength 708
#define startyear 2022
#define startmonth 11
#define startday 20
#define starthour 22
#define startmin 57
#define startsec 00

void DataAnalysis()
{
    int year,month,day,hour,min;
    double sec;
    double time[DataLength];
    double Temperature0[DataLength];
    double Temperature1[DataLength];
    double Temperature2[DataLength];
    double Temperature3[DataLength];
    double nunnum;
    char nunchar;

    TDatime starttime(startyear,startmonth,startday,starthour,startmin,startsec);
    TString name0;
    TString name1;
    TString name2;
    TString name3;
    name0 = "20221120results/221120-2257_ABCV12_PED_TEMPTERATURE_57.txt";
    name1 = "20221120results/221120-2257_ABCV12_PED_TEMPTERATURE_221.txt";
    name2 = "20221120results/221120-2257_ABCV12_PED_TEMPTERATURE_40.txt";
    name3 = "20221120results/221120-2257_ABCV12_PED_TEMPTERATURE_5.txt";
    ifstream inputfile0;
    ifstream inputfile1;
    ifstream inputfile2;
    ifstream inputfile3;
    inputfile0.open(name0,ios::in);
    inputfile1.open(name1,ios::in);
    inputfile2.open(name2,ios::in);
    inputfile3.open(name3,ios::in);

    for (int datanum = 0; datanum < DataLength; datanum++ )
    {
        if (!inputfile0.good() && !inputfile1.good() && !inputfile2.good() && !inputfile3.good())
        {
            break;
        }
        if (inputfile0.good())
        {
            inputfile0>>Temperature0[datanum];
            cout<<Temperature0[datanum]<<endl;
        }
        if (inputfile1.good())
        {
            inputfile1>>Temperature1[datanum];
            cout<<Temperature1[datanum]<<endl;
        }
        if (inputfile2.good())
        {
            inputfile2>>Temperature2[datanum];
            cout<<Temperature2[datanum]<<endl;
        }
        if (inputfile3.good())
        {
            inputfile3>>Temperature3[datanum];
            cout<<Temperature3[datanum]<<endl;
        }
        time[datanum] = datanum*10.0;
        cout<<time[datanum]<<endl;
        cout<<"------------------"<<endl;
        
    }
    //gStyle->SetTimeOffset(starttime.Convert());
    
    TGraph *Tem57toTime = new TGraph(DataLength,time,Temperature0);
    Tem57toTime->SetName("TEMPTERATURE_57");
    Tem57toTime->SetTitle("TEMPTERATURE");
    Tem57toTime->SetMarkerStyle(8);
    Tem57toTime->SetMarkerSize(0.7);
    Tem57toTime->GetXaxis()->SetTimeDisplay(1);
    Tem57toTime->GetYaxis()->SetRangeUser(39,59);
    Tem57toTime->GetXaxis()->SetTimeOffset(starttime.Convert());
    Tem57toTime->SetMarkerColor(kBlue);

    TGraph *Tem221toTime = new TGraph(DataLength,time,Temperature1);
    Tem221toTime->SetName("TEMPTERATURE_221");
    Tem221toTime->SetTitle("TEMPTERATURE_221");
    Tem221toTime->SetMarkerStyle(8);
    Tem221toTime->SetMarkerSize(0.7);
    Tem221toTime->GetXaxis()->SetTimeDisplay(1);
    Tem221toTime->GetXaxis()->SetTimeOffset(starttime.Convert());
    Tem221toTime->SetMarkerColor(kRed);

    TGraph *Tem5toTime = new TGraph(DataLength,time,Temperature3);
    Tem5toTime->SetName("TEMPTERATURE_5");
    Tem5toTime->SetTitle("TEMPTERATURE_5");
    Tem5toTime->SetMarkerStyle(8);
    Tem5toTime->SetMarkerSize(0.7);
    Tem5toTime->GetXaxis()->SetTimeDisplay(1);
    Tem5toTime->GetXaxis()->SetTimeOffset(starttime.Convert());
    Tem5toTime->SetMarkerColor(kGreen);

    TGraph *Tem40toTime = new TGraph(DataLength,time,Temperature2);
    Tem40toTime->SetName("TEMPTERATURE_40");
    Tem40toTime->SetTitle("TEMPTERATURE_40");
    Tem40toTime->SetMarkerStyle(8);
    Tem40toTime->SetMarkerSize(0.7);
    Tem40toTime->GetXaxis()->SetTimeDisplay(1);
    Tem40toTime->GetXaxis()->SetTimeOffset(starttime.Convert());
    Tem40toTime->SetMarkerColor(kYellow);
    
    
    TCanvas *c1 = new TCanvas("c1","Tempterature",1);
    c1->cd(1);
    Tem57toTime->Draw("AP");
    Tem221toTime->Draw("SAME P");
    Tem5toTime->Draw("SAME P");
    Tem40toTime->Draw("SAME P");
    TLegend *legenda =new TLegend(0.6,0.65,0.88,0.85);
    legenda->SetTextFont(72);
    legenda->SetTextSize(0.04);
    legenda->AddEntry(Tem5toTime,"GU8005 old thermal gal","lpe");
    legenda->AddEntry(Tem221toTime,"GU8221 old thermal gal","lpe");
    legenda->AddEntry(Tem40toTime,"GU8040 new thermal gal","lpe");
    legenda->AddEntry(Tem57toTime,"GU8057 new thermal gal","lpe");
    legenda->Draw();
    c1->cd();
}