//这个脚本用于分析模拟的梯形波信号，对比论文中的ADCu与电荷的关系，来刻度SSH输出信号的峰值。
//SSH [ADCu] = 1/7.9[fC]  + 66.9(+- 0.3)，适用于width=10ns，eage=5ns的梯形波，来自于文章实验结果。
//scale = 27.6416  即SSH输出波形scale(27.6416)+66.9，得到以ADCu为单位的波形。20230302

#include "/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/shaper.cc"

void SSHcalib()
{

	TFile *ifile = TFile::Open("/junofs/users/chuziliang125/CATIROCsimulation/compiable_version/FSHcalib/TrapezoidalWave.root");
    double charge[10];
    double SSHmax[10];
    
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

        charge[j] = 48*(j+1);
        SSHmax[j] = SSH->GetBinContent(SSH->GetMaximumBin());
		if (j+1 == 10)
		{
			output = (TH1*)SSH->Clone();
		}
		delete FSH;
		delete SSH;
		delete tmp;
	}

	TF1 *g1 = new TF1("g1","pol1",0,1);

	TCanvas *c1 = new TCanvas("c1","c1",1);
	c1->cd();
    TGraph *charge2SSHmax = new TGraph(10,charge,SSHmax);
	charge2SSHmax->Draw();
	charge2SSHmax->Fit(g1);
	TCanvas *c2 = new TCanvas("c2","c2",1);
	c2->cd();
	output->Draw();
	
	double scale = (1.0/7.9)/g1->GetParameter("p1");

	cout<<"scale = "<<scale<<endl;

	double SSHscale[10];

	for (int i = 0; i < 10; i++)
	{
		SSHscale[i] = SSHmax[i]*scale;
	}

	TCanvas *c4 = new TCanvas("c4","c4",1);
    TGraph *charge2SSHscale = new TGraph(10,charge,SSHscale);
	charge2SSHscale->Draw();
	charge2SSHscale->Fit("pol1");
	
}