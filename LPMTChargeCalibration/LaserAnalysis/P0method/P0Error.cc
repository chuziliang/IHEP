double Error(int TotalEvents, int FireEvents)
{
    double P0 = (double)(TotalEvents - FireEvents)/TotalEvents;
    double mean = -TMath::Log(P0);
    // double miuP0 = P0*(1 - P0)/TotalEvents;
    // miuP0 = sqrt(miuP0);
    // double error = miuP0/P0;
    double error = mean/TotalEvents;
    error = sqrt(error);
    return error;
}

double RelativeError(int TotalEvents, int FireEvents)
{
    double P0 = (double)(TotalEvents - FireEvents)/TotalEvents;
    double mean = -TMath::Log(P0);
    // double miuP0 = P0*(1 - P0)/TotalEvents;
    // miuP0 = sqrt(miuP0);
    // double error = miuP0/P0;
    double error = mean/TotalEvents;
    error = sqrt(error);
    return error/mean;
}

double Mean(int TotalEvents, int FireEvents)
{
    double P0 = (double)(TotalEvents - FireEvents)/TotalEvents;
    double mean = -TMath::Log(P0);
    double miuP0 = P0*(1 - P0)/TotalEvents;
    miuP0 = sqrt(miuP0);
    double error = miuP0/P0;
    return mean;
}

void P0Error()
{
    int TotEvent = 100000;
    vector<double> FireEvent;
    vector<double> Errors;
    vector<double> RelativeErrors;
    int Fired = 2;
    while (Fired< 0.5*TotEvent)
    {
        double E = Error(TotEvent,Fired);
        double RE = RelativeError(TotEvent,Fired);
        // cout<<RE<<endl;
        FireEvent.push_back(Mean(TotEvent,Fired));
        Errors.push_back(E*sqrt((double)TotEvent));
        RelativeErrors.push_back(RE*sqrt((double)TotEvent));

        Fired *= 2;
    }
    while (Fired< TotEvent-2)
    {
        double E = Error(TotEvent,Fired);
        double RE = RelativeError(TotEvent,Fired);
        // cout<<RE<<endl;
        FireEvent.push_back(Mean(TotEvent,Fired));
        Errors.push_back(E*sqrt((double)TotEvent));
        RelativeErrors.push_back(RE*sqrt((double)TotEvent));

        Fired += (TotEvent - Fired)/2;
        cout<<Fired<<endl;
    }

    // int Num[2] = {99900,99990};
    // for (int i = 0; i < 2; i++)
    // {
    //     int Fired = Num[i];
    //     double E = Error(TotEvent,Fired);
    //     double RE = RelativeError(TotEvent,Fired);
    //     // cout<<RE<<endl;
    //     FireEvent.push_back(Mean(TotEvent,Fired));
    //     Errors.push_back(E*sqrt((double)TotEvent));
    //     RelativeErrors.push_back(RE*sqrt((double)TotEvent));
    // }

    TCanvas *c1 = new TCanvas("c1","c1",1);
    TGraph *output = new TGraph(FireEvent.size(),&FireEvent[0],&Errors[0]);
    output->GetYaxis()->SetTitle("1/sqrt(n)");
    output->GetXaxis()->SetTitle("p.e.");
    output->SetTitle("Absolute error of P(0) method");
    output->Draw();


    TCanvas *c2 = new TCanvas("c2","c2",1);
    TGraph *output2 = new TGraph(FireEvent.size(),&FireEvent[0],&RelativeErrors[0]);
    output2->GetYaxis()->SetTitle("1/[sqrt(n)*mean]");
    output2->GetXaxis()->SetTitle("p.e.");
    output2->SetTitle("Relative error of P(0) method");
    output2->SetLineColor(kRed);
    output2->Draw();
    

    // Float_t scale = output->GetHistogram()->GetMaximum()/output2->GetHistogram()->GetMaximum();
    // cout<<output->GetHistogram()->GetMaximum()<<endl;
    // cout<<output2->GetHistogram()->GetMaximum()<<endl;
    // Float_t rightmax = gPad->GetUymax()/scale;
    // output2->Scale(scale);
    // // gStyle->SetOptStat(1);
    
    // output2->Draw("same");
    // c1->Update();

    // TGaxis*axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
    //                         gPad->GetUxmax(),gPad->GetUymax(),
    //                         0,rightmax,510,"+L");
    // axis->SetLineColor(kRed);
    // axis->SetLabelColor(kRed);
    // axis->Draw();
    // c1->Update();
}