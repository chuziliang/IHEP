TString path("./t_related");

void fit4() {
    Int_t n = 6;

    Double_t length12[3];
    Double_t elength12[3] = {0, 0, 0};
    Double_t TTS12[3];
    Double_t eTTS12[3];
    Double_t rise12[3];
    Double_t erise12[3];
    Double_t fall12[3];
    Double_t efall12[3];
    Double_t mid12[3];
    Double_t emid12[3];

    Double_t length15[3];
    Double_t elength15[3] = {0, 0, 0};
    Double_t TTS15[3];
    Double_t eTTS15[3];
    Double_t rise15[3];
    Double_t erise15[3];
    Double_t fall15[3];
    Double_t efall15[3];
    Double_t mid15[3];
    Double_t emid15[3];

    double l, hv, t, et, r, er, f, ef, m, em;

    ifstream in(path);
    if (in.good()){
        for (int i = 0; i < n; i ++){
            int j = i / 2;
            in >> l >> hv >> t >> et >> r >> er >> f >> ef >> m >> em;
            if (i % 2 == 0){
                length12[j] = l;
                TTS12[j] = t;
                eTTS12[j] = et;
                rise12[j] = r;
                erise12[j] = er;
                fall12[j] = f;
                efall12[j] = ef;
                mid12[j] = m;
                emid12[j] = em;
            }
            else {
                length15[j] = l;
                TTS15[j] = t;
                eTTS15[j] = et;
                rise15[j] = r;
                erise15[j] = er;
                fall15[j] = f;
                efall15[j] = ef;
                mid15[j] = m;
                emid15[j] = em;
            }
        }

        auto TTSvCL12 = new TGraphErrors(3, length12, TTS12, elength12, eTTS12);
        auto TTSvCL15 = new TGraphErrors(3, length15, TTS15, elength15, eTTS15);
        auto risevCL12 = new TGraphErrors(3, length12, rise12, elength12, erise12);
        auto risevCL15 = new TGraphErrors(3, length15, rise15, elength15, erise15);
        auto fallvCL12 = new TGraphErrors(3, length12, fall12, elength12, efall12);
        auto fallvCL15 = new TGraphErrors(3, length15, fall15, elength15, efall15);
        auto midvCL12 = new TGraphErrors(3, length12, mid12, elength12, emid12);
        auto midvCL15 = new TGraphErrors(3, length15, mid15, elength15, emid15);

        TTSvCL12->SetMarkerStyle(kFullDotLarge);
        TTSvCL12->SetMarkerColor(kBlue);
        TTSvCL12->SetLineColor(kBlue);
        TTSvCL12->SetLineWidth(2);
        TTSvCL12->GetXaxis()->SetRangeUser(-1, 12);
        TTSvCL15->SetMarkerStyle(kFullDotLarge);
        TTSvCL15->SetMarkerColor(kRed);
        TTSvCL15->SetLineColor(kRed);
        TTSvCL15->SetLineWidth(2);
        TTSvCL15->GetXaxis()->SetRangeUser(-1, 12);

        risevCL12->SetMarkerStyle(kFullDotLarge);
        risevCL12->SetMarkerColor(kBlue);
        risevCL12->SetLineColor(kBlue);
        risevCL12->SetLineWidth(2);
        risevCL12->GetXaxis()->SetRangeUser(-1, 12);
        risevCL15->SetMarkerStyle(kFullDotLarge);
        risevCL15->SetMarkerColor(kRed);
        risevCL15->SetLineColor(kRed);
        risevCL15->SetLineWidth(2);
        risevCL15->GetXaxis()->SetRangeUser(-1, 12);

        fallvCL12->SetMarkerStyle(kFullDotLarge);
        fallvCL12->SetMarkerColor(kBlue);
        fallvCL12->SetLineColor(kBlue);
        fallvCL12->SetLineWidth(2);
        fallvCL12->GetXaxis()->SetRangeUser(-1, 12);
        fallvCL15->SetMarkerStyle(kFullDotLarge);
        fallvCL15->SetMarkerColor(kRed);
        fallvCL15->SetLineColor(kRed);
        fallvCL15->SetLineWidth(2);
        fallvCL15->GetXaxis()->SetRangeUser(-1, 12);

        midvCL12->SetMarkerStyle(kFullDotLarge);
        midvCL12->SetMarkerColor(kBlue);
        midvCL12->SetLineColor(kBlue);
        midvCL12->SetLineWidth(2);
        midvCL12->GetXaxis()->SetRangeUser(-1, 12);
        midvCL15->SetMarkerStyle(kFullDotLarge);
        midvCL15->SetMarkerColor(kRed);
        midvCL15->SetLineColor(kRed);
        midvCL15->SetLineWidth(2);
        midvCL15->GetXaxis()->SetRangeUser(-1, 12);

        auto mg = new TMultiGraph();
        mg->Add(TTSvCL12);
        mg->Add(TTSvCL15);
        mg->GetXaxis()->SetLabelSize(0.07);
        mg->GetXaxis()->SetTitle("Cable length / m");
        mg->GetXaxis()->SetTitleSize(0.08);
        mg->GetXaxis()->SetRangeUser(0, 11);
        mg->GetYaxis()->SetLabelSize(0.07);
        mg->GetYaxis()->SetTitle("TTS (#sigma) / ns");
        mg->GetYaxis()->SetTitleSize(0.08);
        mg->GetYaxis()->SetTitleOffset(0.8);
        mg->GetYaxis()->SetRangeUser(1.3, 2.0);
        auto c = new TCanvas();
        c->SetBottomMargin(0.2);
        c->SetLeftMargin(0.2);
        c->cd();
        mg->Draw("APL");

        auto mg1 = new TMultiGraph();
        mg1->Add(risevCL12);
        mg1->Add(risevCL15);
        mg1->GetXaxis()->SetLabelSize(0.07);
        mg1->GetXaxis()->SetTitle("Cable length / m");
        mg1->GetXaxis()->SetTitleSize(0.08);
        mg1->GetXaxis()->SetRangeUser(0, 11);
        mg1->GetYaxis()->SetLabelSize(0.07);
        mg1->GetYaxis()->SetTitle("risetime / ns");
        mg1->GetYaxis()->SetTitleSize(0.08);
        mg1->GetYaxis()->SetTitleOffset(0.8);
        mg1->GetYaxis()->SetRangeUser(1.3, 2.4);
        auto c1 = new TCanvas();
        c1->SetBottomMargin(0.2);
        c1->SetLeftMargin(0.2);
        c1->cd();
        mg1->Draw("APL");

        auto mg2 = new TMultiGraph();
        mg2->Add(fallvCL12);
        mg2->Add(fallvCL15);
        mg2->GetXaxis()->SetLabelSize(0.07);
        mg2->GetXaxis()->SetTitle("Cable length / m");
        mg2->GetXaxis()->SetTitleSize(0.08);
        mg2->GetXaxis()->SetRangeUser(0, 11);
        mg2->GetYaxis()->SetLabelSize(0.07);
        mg2->GetYaxis()->SetTitle("falltime / ns");
        mg2->GetYaxis()->SetTitleSize(0.08);
        mg2->GetYaxis()->SetTitleOffset(0.8);
        mg2->GetYaxis()->SetRangeUser(18, 25);
        auto c2 = new TCanvas();
        c2->SetBottomMargin(0.2);
        c2->SetLeftMargin(0.2);
        c2->cd();
        mg2->Draw("APL");

        auto mg3 = new TMultiGraph();
        mg3->Add(midvCL12);
        mg3->Add(midvCL15);
        mg3->GetXaxis()->SetLabelSize(0.07);
        mg3->GetXaxis()->SetTitle("Cable length / m");
        mg3->GetXaxis()->SetTitleSize(0.08);
        mg3->GetXaxis()->SetRangeUser(0, 11);
        mg3->GetYaxis()->SetLabelSize(0.07);
        mg3->GetYaxis()->SetTitle("mid / ns");
        mg3->GetYaxis()->SetTitleSize(0.08);
        mg3->GetYaxis()->SetTitleOffset(0.8);
        mg3->GetYaxis()->SetRangeUser(8, 20);
        auto c3 = new TCanvas();
        c3->SetBottomMargin(0.2);
        c3->SetLeftMargin(0.2);
        c3->cd();
        mg3->Draw("APL");

        auto leg = new TLegend(0.4, 0.6, 0.89, 0.89);
        leg->AddEntry(TTSvCL12, "TTS @ 1279 V (3E6, 10 m)");
        leg->AddEntry(TTSvCL15, "TTS @ 1529 V (1E7, 10 m)");
        leg->Draw();

    }
}
