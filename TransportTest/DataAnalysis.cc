#define DataLength 88819
#define startyear 2022
#define startmonth 3
#define startday 7
#define starthour 11
#define startmin 28
#define startsec 40

void DataAnalysis()
{
    int year,month,day,hour,min;
    double sec;
    double time[DataLength];
    double ax[DataLength];
    double ay[DataLength];
    double az[DataLength];
    double wx[DataLength];
    double wy[DataLength];
    double wz[DataLength];
    double AngleX[DataLength];
    double AngleY[DataLength];
    double AngleZ[DataLength];
    double nunnum;
    char nunchar;

    TDatime starttime(startyear,startmonth,startday,starthour,startmin,0);
    TString name;
    name = "Transport.txt";
    ifstream inputfile;
    inputfile.open(name,ios::in);
    inputfile.seekg(2,inputfile.cur);
    while(inputfile.peek() != inputfile.widen('\n'))
    {
        inputfile.seekg(1,inputfile.cur);
    }
    for (int datanum = 0; datanum < DataLength; datanum++ )
    {
        if (!inputfile.good())
        {
            break;
        }
        
        if (datanum%9000 == 0)
        {
            cout<<datanum/9000<<endl;
        }
        

        while(inputfile.peek()>'9' or inputfile.peek()<'0')
        {
            inputfile.seekg(1,inputfile.cur);
        }

        inputfile.seekg(12,inputfile.cur);

        inputfile>>year>>nunchar>>month>>nunchar>>day>>hour>>nunchar>>min>>nunchar>>sec
        >>ax[datanum]>>ay[datanum]>>az[datanum]>>wx[datanum]>>wy[datanum]>>wz[datanum]
        >>AngleX[datanum]>>AngleY[datanum]>>AngleZ[datanum]>>nunnum>>nunnum>>nunnum>>nunnum;

        TDatime curenttime(year,month,day,hour,min,0);

        time[datanum] = curenttime.Convert()-starttime.Convert()+(sec-startsec);
        
    }
    gStyle->SetTimeOffset(starttime.Convert());
    
    TGraph *ax2time = new TGraph(DataLength,time,ax);
    ax2time->SetName("acceleration x");
    ax2time->SetTitle("acceleration");
    ax2time->SetMarkerStyle(8);
    ax2time->SetMarkerSize(0.7);
    ax2time->GetXaxis()->SetTimeDisplay(1);
    //ax2time->GetYaxis()->SetRangeUser(-10,10);
    ax2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    ax2time->SetMarkerColor(kBlue);
    
    TGraph *ay2time = new TGraph(DataLength,time,ay);
    ay2time->SetName("acceleration y");
    ay2time->SetTitle("acceleration y");
    ay2time->SetMarkerStyle(8);
    ay2time->SetMarkerSize(0.7);
    ay2time->GetXaxis()->SetTimeDisplay(1);
    ay2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    ay2time->SetMarkerColor(kRed);
    
    TGraph *az2time = new TGraph(DataLength,time,az);
    az2time->SetName("acceleration z");
    az2time->SetTitle("acceleration z");
    az2time->SetMarkerStyle(8);
    az2time->SetMarkerSize(0.7);
    az2time->GetXaxis()->SetTimeDisplay(1);
    az2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    az2time->SetMarkerColor(kGreen);
    
    
    TCanvas *c1 = new TCanvas("c1","Acceleration",1);
    c1->cd(1);
    ax2time->Draw("AP");
    ay2time->Draw("SAME P");
    az2time->Draw("SAME P");
    TLegend *legenda =new TLegend(0.6,0.65,0.88,0.85);
    legenda->SetTextFont(72);
    legenda->SetTextSize(0.04);
    legenda->AddEntry(ax2time,"acceleration x","lpe");
    legenda->AddEntry(ay2time,"acceleration y","lpe");
    legenda->AddEntry(az2time,"acceleration z","lpe");
    legenda->Draw();
    c1->cd();

    TGraph *wx2time = new TGraph(DataLength,time,wx);
    wx2time->SetName("Angular velocity x");
    wx2time->SetTitle("Angular velocity");
    wx2time->SetMarkerStyle(8);
    wx2time->SetMarkerSize(0.7);
    wx2time->GetXaxis()->SetTimeDisplay(1);
    wx2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    wx2time->GetYaxis()->SetRangeUser(-800,600);
    wx2time->SetMarkerColor(kBlue);
   
    TGraph *wy2time = new TGraph(DataLength,time,wy);
    wy2time->SetName("Angular velocity y");
    wy2time->SetTitle("Angular velocity y");
    wy2time->SetMarkerStyle(8);
    wy2time->SetMarkerSize(0.7);
    wy2time->GetXaxis()->SetTimeDisplay(1);
    wy2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    wy2time->SetMarkerColor(kRed);
    
    TGraph *wz2time = new TGraph(DataLength,time,wz);
    wz2time->SetName("Angular velocity z");
    wz2time->SetTitle("Angular velocity z");
    wz2time->SetMarkerStyle(8);
    wz2time->SetMarkerSize(0.7);
    wz2time->GetXaxis()->SetTimeDisplay(1);
    wz2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    wz2time->SetMarkerColor(kGreen);
    
    TCanvas *c2 = new TCanvas("c2","Angular velocity",1);
    c2->cd(1);
    wx2time->Draw("AP");
    wy2time->Draw("SAME P");
    wz2time->Draw("SAME P");
    TLegend *legendw =new TLegend(0.6,0.65,0.88,0.85);
    legendw->SetTextFont(72);
    legendw->SetTextSize(0.04);
    legendw->AddEntry(wx2time,"Angular velocity x","lpe");
    legendw->AddEntry(wy2time,"Angular velocity y","lpe");
    legendw->AddEntry(wz2time,"Angular velocity z","lpe");
    legendw->Draw();
    
    TGraph *AngleX2time = new TGraph(DataLength,time,AngleX);
    AngleX2time->SetName("Angle x");
    AngleX2time->SetTitle("Angle");
    AngleX2time->SetMarkerStyle(8);
    AngleX2time->SetMarkerSize(0.7);
    AngleX2time->GetXaxis()->SetTimeDisplay(1);
    AngleX2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    AngleX2time->GetYaxis()->SetRangeUser(-200,200);
    AngleX2time->SetMarkerColor(kBlue);

    TGraph *AngleY2time = new TGraph(DataLength,time,AngleY);
    AngleY2time->SetName("Angle y");
    AngleY2time->SetTitle("Angle y");
    AngleY2time->SetMarkerStyle(8);
    AngleY2time->SetMarkerSize(0.7);
    AngleY2time->GetXaxis()->SetTimeDisplay(1);
    AngleY2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    AngleY2time->SetMarkerColor(kRed);

    TGraph *AngleZ2time = new TGraph(DataLength,time,AngleZ);
    AngleZ2time->SetName("Angle z");
    AngleZ2time->SetTitle("Angle z");
    AngleZ2time->SetMarkerStyle(8);
    AngleZ2time->SetMarkerSize(0.7);
    AngleZ2time->GetXaxis()->SetTimeDisplay(1);
    AngleZ2time->GetXaxis()->SetTimeOffset(starttime.Convert());
    AngleZ2time->SetMarkerColor(kGreen);

    TCanvas *c3 = new TCanvas("c3","Angule",1);
    c3->cd(1);
    AngleX2time->Draw("AP");
    AngleY2time->Draw("SAME P");
    AngleZ2time->Draw("SAME P");
    TLegend *legendangle =new TLegend(0.6,0.65,0.88,0.85);
    legendangle->SetTextFont(72);
    legendangle->SetTextSize(0.04);
    legendangle->AddEntry(AngleX2time,"Angle x","lpe");
    legendangle->AddEntry(AngleY2time,"Angle y","lpe");
    legendangle->AddEntry(AngleZ2time,"Angle z","lpe");
    legendangle->Draw();
    c3->Update();
    c3->cd(1);

}