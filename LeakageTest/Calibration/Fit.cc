double Fitfunction(double *x, double *par)
{
    // return par[0]*x[0] + par[1];
    return par[0]*TMath::Exp(x[0]*par[1]);
}

void Fit()
{
    double time[] = {0,208,285,399,527,660,797};
    double time_error[] = {1,1,1,1,1,1,1};
    double SF6[] = {6.445,4.1625,2.925,2.075,1.1775,0.76375,0.54625};
    double SF6_error[] = {0.05258921,0.192816299,0.189761363,0.078302299,0.062873882,0.041926275,0.019485572};

    for (int i = 0; i < 7; i++)
    {
        time[i] = time[i]*60.;
        time_error[i] = time_error[i] * 60.;
    }
    

    TGraphErrors *gr = new TGraphErrors(7,time,SF6,time_error,SF6_error);
    TF1 *fitFcn = new TF1("fitFcn",Fitfunction,0,900,2);
    fitFcn->SetParameter(0,6.445);
    fitFcn->SetParameter(1,TMath::Log(SF6[1]/SF6[0])/time[1]);
    fitFcn->SetLineColor(kRed);
    fitFcn->SetParNames("C_{SF_{6}}(t_{0})","-L/V");
    gr->SetTitle("");
    gr->GetXaxis()->SetTitle("Time/s");
    gr->GetYaxis()->SetTitle("Concentration/PPM");
    gr->Fit("fitFcn");
    gr->SetMarkerStyle(20);
    gr->Draw("AP");
    gStyle->SetOptFit(1111);
    fitFcn->Draw("same");

    cout<<"L="<<fitFcn->GetParameter(1)*0.011375<<"  "<<"Error="<<fitFcn->GetParError(1)*0.011375<<endl;
}