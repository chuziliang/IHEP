//这个脚本用于分析模拟的梯形波信号，对比论文中的阈值与电荷的关系（SCurve），确认FSH的模拟是否正确。
//FSH [DACu]=-323*[PE]+991，适用于width=10ns，eage=5ns的梯形波，来自于文章实验结果。
//scale = -18.9193  即FSH输出波形scale(-18.9193)+991，得到以DACu为单位的波形。20230302

#include "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/shaper.cc"

void FSHcalib()
{

	TFile *ifile = TFile::Open("./TrapezoidalWave.root");
    double PE[10];
    double FSHmax[10];
	double DACu[10] = {953,926,895,865,833,799,764,730,698,0};
    
	TH1 *output = 0;

	for(int j = 0;j<10;j++)
	{
		
		TH1* tmp = nullptr;
    	tmp = (TH1*)ifile->Get(TString::Format("Signal %1.1fPE",0.1*(j+1)));

		TH1* FSH = nullptr;
		TH1* SSH = nullptr;

		shaper(tmp,FSH,SSH);
        FSH->SetName(TString::Format("FSH %1.1fPE",0.1*(j+1)));
		SSH->SetName(TString::Format("SSH %1.1fPE",0.1*(j+1)));

        PE[j] = 0.1*(j+1);
        FSHmax[j] = FSH->GetBinContent(FSH->GetMaximumBin());
		if (j+1 == 10)
		{
			output = (TH1*)FSH->Clone();
		}
		delete FSH;
		delete SSH;
		delete tmp;
	}

	TF1 *g1 = new TF1("g1","pol1",0,1);
	TF1 *g2 = new TF1("g2","pol1",0,1);

	TCanvas *c1 = new TCanvas("c1","c1",1);
	c1->cd();
    TGraph *PE2FSHmax = new TGraph(10,PE,FSHmax);
	PE2FSHmax->Draw();
	PE2FSHmax->Fit(g1);
	TCanvas *c2 = new TCanvas("c2","c2",1);
	c2->cd();
	output->Draw();
	TCanvas *c3 = new TCanvas("c3","c3",1);
	TGraph *PE2DACu = new TGraph(9,PE,DACu);
	PE2DACu->Draw();
	PE2DACu->Fit(g2);

	double scale = g2->GetParameter("p1")/g1->GetParameter("p1");

	cout<<"scale = "<<scale<<endl;

	double FSHscale[10];

	for (int i = 0; i < 10; i++)
	{
		FSHscale[i] = FSHmax[i]*scale;
	}

	TCanvas *c4 = new TCanvas("c4","c4",1);
    TGraph *PE2FSHscale = new TGraph(10,PE,FSHscale);
	PE2FSHscale->Draw();
	PE2FSHscale->Fit("pol1");
	
}