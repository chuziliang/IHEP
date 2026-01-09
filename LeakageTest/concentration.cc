#define D 0.9475e-5
#define TotalTime 1  //一共多少小时
#define tbin 3600   //一小时多少bin
#define LeakageRate 1E-9  //Pa*m3/s
#define Length 5.0  //模拟范围，单位m
#define rbin 100  //一米多少bin

void concentration()
{
    double Ci[(int)Length*rbin];
    double Cip[(int)Length*rbin];
    
    TH2 *con = new TH2F("Concentration","Concentration",(int)TotalTime*tbin,0,TotalTime,(int)Length*rbin,0,Length);
    con->GetXaxis()->SetTitle("h");
    con->GetYaxis()->SetTitle("m");

    for (int ti = 0; ti < TotalTime*tbin-1; ti++)
    {
        for (int ri = 0; ri < Length*rbin; ri++)//在每一秒开始之前，先更新这一秒的数据
        {
            Ci[ri] = Cip[ri];
            Cip[ri] = 0;
        }
        for (int i = 0; i < 10; i++)
        {
            Ci[i] =  LeakageRate*10*rbin*rbin*rbin*(3.0/(4.0*TMath::Pi()))*(3600.0/tbin)/1000.0;//更新完数据之后，在中心的bin内加入溢出的SF6
        }   

        Cip[0] = D*(3600.0/tbin)*( ((-Ci[3]+4.0*Ci[2]-5.0*Ci[1]+2.0*Ci[0])*rbin*rbin) + (-Ci[2]+4*Ci[1]-3*Ci[0])*rbin*rbin/1 ) + Ci[0];
        Cip[1] = D*(3600.0/tbin)*( ((-Ci[4]+4.0*Ci[3]-5.0*Ci[2]+2.0*Ci[1])*rbin*rbin) + (-Ci[3]+4*Ci[2]-3*Ci[1])*rbin*rbin/1 ) + Ci[1];

        con->SetBinContent(ti+1,1,Ci[0]);
        con->SetBinContent(ti+1,2,Ci[1]);

        for (int ri = 2; ri < Length*rbin; ri++ )
        {
            Cip[ri] = D*(3600.0*tbin)*( ((Ci[ri]-2.0*Ci[ri-1]+Ci[ri-2])*rbin*rbin) + 2*(Ci[ri]-Ci[ri-1])*rbin*rbin/(ri+1) ) + Ci[ri];
            con->SetBinContent(ti+1,ri+1,Ci[ri]);
        }
        // Cip[(int)Length*rbin-1] = D*(3600.0*tbin)*( 
        //     ((2.0*Ci[(int)Length*rbin-1]-5.0*Ci[(int)Length*rbin-2]+4.0*Ci[(int)Length*rbin-3]-Ci[(int)Length*rbin-4])*rbin*rbin) 
        // + (Ci[(int)Length*rbin-3]-4*Ci[(int)Length*rbin-2]+3*Ci[(int)Length*rbin-1])*rbin/(double)Length ) 
        // + Ci[(int)Length*rbin-1];
        // con->SetBinContent(ti,rbin,Ci[(int)Length*rbin-1]);  
    }
     for (int ri = 0; ri < Length*rbin; ri++ )
        {
            con->SetBinContent(TotalTime*tbin,ri+1,Cip[ri]);
        }

        con->Draw("COLZ");
}