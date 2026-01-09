//这个脚本用于分析模拟的高斯信号，对比论文中的阈值与电荷的关系（SCurve），确认FSH的模拟是否正确。

#define triggerDACu 650
#define DelayTime 42
#include "shaper.cc"

void test()
{

	TFile *ifile = TFile::Open("./FSHcalib/TrapezoidalWave.root");

	//for(int j = 0;j<10;j++)
	
		
	TH1* tmp = nullptr;
	TH1* FSH = nullptr;
	TH1* SSH = nullptr;
    tmp = (TH1*)ifile->Get("Signal 0.1PE");
	shaper(tmp,FSH,SSH);

	
	TCanvas *c1 = new TCanvas("c1","c1",1);
	FSH->Draw("HIST L");
	TCanvas *c2 = new TCanvas("c2","c2",1);
	SSH->Draw("HIST L");
	TCanvas *c3 = new TCanvas("c3","c3",1);
	tmp->Draw("HIST L");

}