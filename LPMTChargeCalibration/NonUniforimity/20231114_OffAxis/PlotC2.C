#include <vector>
#include <fstream>
#include "TGraphErrors.h"
#include "TLine.h"
#include "TCanvas.h"

void PlotC2() {
    // 读取数据文件
    ifstream infile("FactorData.txt");
    double trueFactor, trueError;
    infile >> trueFactor >> trueError;

    std::vector<double> theta, factor, factorError;
    double t, f, fe;
    while (infile >> t >> f >> fe) {
        theta.push_back(t);
        factor.push_back(f);
        factorError.push_back(fe);
    }

    // 创建图形
    TGraphErrors *Factor_positions = new TGraphErrors(theta.size(), 
        theta.data(), factor.data(), 
        nullptr, factorError.data()); // X误差设为0

    TCanvas *c2 = new TCanvas("c2","c2",1200,900);
    c2->SetFillColor(0);
    c2->SetBorderMode(0);
    c2->SetBorderSize(2);
    c2->SetLeftMargin(0.1277129);
    c2->SetRightMargin(0.07262103);
    c2->SetTopMargin(0.06735159);
    c2->SetBottomMargin(0.1335616);
    c2->SetFrameBorderMode(0);
    c2->SetFrameBorderMode(0);

    Factor_positions->SetMarkerStyle(20);
    Factor_positions->SetTitle("");
    Factor_positions->SetFillStyle(3002);
    Factor_positions->SetMarkerSize(0.8);
    Factor_positions->SetMarkerStyle(20);
    Factor_positions->SetTitle("");
    Factor_positions->SetFillStyle(3002);
    Factor_positions->SetMarkerColor(4);
    Factor_positions->SetLineColor(4);
    Factor_positions->SetMarkerSize(1.0);
    Factor_positions->SetMarkerStyle(20);
    Factor_positions->SetLineWidth(2);
    Factor_positions->GetXaxis()->CenterTitle();
    Factor_positions->GetYaxis()->CenterTitle();
    Factor_positions->GetXaxis()->SetTitleFont(22);
    Factor_positions->GetYaxis()->SetTitleFont(22);
    Factor_positions->GetXaxis()->SetTitleSize(0.06);
    Factor_positions->GetYaxis()->SetTitleSize(0.06);
    Factor_positions->GetXaxis()->SetLabelSize(0.06);
    Factor_positions->GetYaxis()->SetLabelSize(0.06);
    Factor_positions->GetXaxis()->SetTitleOffset(0.92);
    Factor_positions->GetYaxis()->SetTitleOffset(0.92);

   
    Factor_positions->SetTitle("");
    Factor_positions->GetXaxis()->SetTitle("Angle of incidence [deg]");
    Factor_positions->GetYaxis()->SetTitle("Calibration factor");
    Factor_positions->Draw("AP");

     // 在x=48处添加一条直线
    double x1 = 48;
    double y1 = 30;
    double x2 = 48;
    double y2 = 128;
    TLine *line = new TLine(x1, y1, x2, y2);
    line->SetLineColor(kRed);
    line->SetLineStyle(2);
    line->SetLineWidth(3);
    line->Draw();

    // 在x=52处添加一条直线
    double x3 = 53.2;
    double y3 = 30;
    double x4 = 53.2;
    double y4 = 128;
    TLine *line2 = new TLine(x3, y3, x4, y4);
    line2->SetLineColor(kGreen);
    line2->SetLineStyle(9);
    line2->SetLineWidth(3);
    line2->Draw();

    
    // 绘制真实因子参考线
    TLine *l = new TLine(0, trueFactor, 65, trueFactor);
    TLine *lh = new TLine(0, trueFactor+trueError, 60, trueFactor+trueError);
    TLine *ll = new TLine(0, trueFactor-trueError, 60, trueFactor-trueError);
    
    l->SetLineWidth(3);
    l->SetLineStyle(5);
    l->SetLineColor(6);
    lh->SetLineStyle(2);
    ll->SetLineStyle(2);
    
    l->Draw("same");
    // lh->Draw("same");
    // ll->Draw("same");

    TLegend *leg = new TLegend(0.1469115,0.7705479,0.4040067,0.9075342,NULL,"brNDC");
    leg->SetBorderSize(0);
    leg->SetTextFont(22);
    leg->SetTextSize(0.03424658);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(1);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);


    leg->AddEntry(line, "SPMT coverd by LPMT", "L");
    leg->AddEntry(line2, "SPMT and LPMT coverd by supporting bar", "L");
    leg->AddEntry(l, "Detector center result", "L");
    leg->Draw();

}
