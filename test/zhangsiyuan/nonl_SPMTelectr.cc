// #include "Identifier.h"
// #include "JunoDetectorID.h"
// #include "CdID.h"

int CdID2pmtid(unsigned int cdid)
{
    int pmtid = (cdid-(0x10<<24))>>8;
    return pmtid;
}

void nonl_SPMTelectr()
{
	int InputTrueEnergy=1000;
	TChain *calibevt_spmt = new TChain("/Event/CdSpmtCalib/CdSpmtCalibEvt");
	TChain *set=new TChain("/Event/CdSpmtTruth/CdSpmtElecTruthEvt");
	//TChain *SE=new TChain("/Event/Sim/SimEvt");
	for(int i = 0; i < 200; i++){
		// TFile *f=new TFile(Form("/junofs/users/junospmt/GXU/Data/Masstest/Input_file/Other_Data/hep/calib/highE/SimOut/uniform/%dMeV/calib/calib_%dMeV_%d.root",InputTrueEnergy,InputTrueEnergy,i));
		TFile* f = TFile::Open(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/root/1500MeV/cailb-%1d.root",i),"read");
                if (!f) {delete f;continue;}
                delete f;
		calibevt_spmt->Add(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/root/1500MeV/cailb-%1d.root",i));
		set->Add(TString::Format("root://junoeos01.ihep.ac.cn//eos/juno/groups/Calibration/chuziliang125/calib/root/1500MeV/cailb-%1d.root",i));
		//SE->Add(Form("/junofs/users/junospmt/GXU/Data/Masstest/Input_file/Other_Data/hep/calib/highE/SimOut/uniform/%dMeV/calib/calib_%dMeV_%d.root",InputTrueEnergy,InputTrueEnergy,i));
	}
	JM::CdSpmtCalibEvt *calibobj_spmt=0;
        calibevt_spmt->SetBranchAddress("CdSpmtCalibEvt",&calibobj_spmt);
	Int_t TotalNumEvent = calibevt_spmt->GetEntries();
	cout << "TotalNumEvent = " << TotalNumEvent << endl;
	
	//JM::SimEvt *seobj=0;
	//SE->SetBranchAddress("SimEvt",&seobj);
	
	JM::CdSpmtElecTruthEvt *setobj=0;
        set->SetBranchAddress("CdSpmtElecTruthEvt",&setobj);

	TFile* f=new TFile("chargenonl.root","recreate");

	TH1D *h[10];
	for(int i=0;i<10;i++){
		h[i]=new TH1D(Form("n%dpe",i+1),"",200,-2,2);
	}

	TH1F *test = new TH1F("test","",40000,0,400000);
	
	for(int i=0;i<TotalNumEvent;i++){
		if(i%100==0) cout << "event:"<<i<<endl;
		/*
		SE->GetEntry(i);
		const std::vector< JM::SimPMTHit * >& sph=seobj->getCDHitsVec();
		double  npe[25600];
		for (int j = 0; j < 25600;j++){
                        npe[j]=0;
                }
		for (int j = 0;j < sph.size();j++){
			int id = sph[j]->getPMTID();
			if(id<300000) continue;
			npe[id-300000]=sph[j]->getNPE();
			//if(sph[j]->getNPE()>1)
			cout<<"truth npe="<<sph[j]->getNPE()<<endl;
		}
		*/
		set->GetEntry(i);
		const std::vector< JM::SpmtElecTruth > & SET=setobj->truths();
		double  npe[25600];
		for (int j = 0; j < 25600;j++){
			npe[j]=0;
		}
		//cout<<"truth npmt="<<SET.size()<<endl;;
		for (int j = 0; j < SET.size();j++){
			int id = SET[j].pmtId();
			npe[id-300000]+=SET[j].npe();
			
			//if(SET[j].npe()>1)cout<<"truth npe="<<SET[j].npe()<<endl;;
		}
		
		//
		calibevt_spmt->GetEntry(i);
		const std::list<JM::CalibPmtChannel*>& pmtchannel_spmt=calibobj_spmt->calibPMTCol();
                std::list<JM::CalibPmtChannel*>::const_iterator schit=pmtchannel_spmt.begin();
		double charge[25600];
		for (int j = 0; j < 25600;j++){
                        charge[j]=0;
                }
		while(schit!=pmtchannel_spmt.end())
                {
			const JM::CalibPmtChannel  *calib_spmt = *schit++;
			unsigned int pmtID=calib_spmt->pmtId();
			unsigned int pmtid=CdID2pmtid(pmtID);
			// Identifier id = Identifier(pmtID);
			// if(CdID::is3inch(id))
            //             {
			// 	double ch = calib_spmt->nPE();
			// 	unsigned int pmtid = CdID::module(id);
			// 	charge[pmtid-17612]=ch;
			// }
			test->Fill(CdID2pmtid(pmtID));

			if(pmtid > 17612)
                        {
				double ch = calib_spmt->nPE();
				// unsigned int pmtid = CdID::module(id);
				charge[pmtid-17612] +=ch;
			}
		}
		for (int j = 0; j < 25600;j++){
			for (int k = 1; k < 11;k++){
				if(npe[j]==k) h[k-1]->Fill((charge[j]-double(k))/double(k));
			}
		}
	}
	
	double x[10],ex[10],ey[10],y[10];
	for(int i=0;i<10;i++){
		x[i]=i+1;
		ex[i]=0;
		y[i]=h[i]->GetMean();
		ey[i]=h[i]->GetMeanError();
	}
	TCanvas *c1=new TCanvas();
	c1->SetTopMargin(0.05);
        c1->SetRightMargin(0.02);
        c1->SetLeftMargin(0.12);
        c1->SetBottomMargin(0.125);
	TGraphErrors *g=new TGraphErrors(10,x,y,ex,ey);
	g->SetTitle("");
	g->GetXaxis()->SetTitle("True nPE [p.e.]");
	g->GetXaxis()->CenterTitle();
	g->GetYaxis()->SetTitle("Nonlinearity");
	g->GetYaxis()->CenterTitle();
	g->GetYaxis()->SetLabelSize(0.05);
        g->GetYaxis()->SetTitleSize(0.06);
        g->GetXaxis()->SetTitleSize(0.06);
        g->GetXaxis()->SetLabelSize(0.05);
        g->GetXaxis()->SetTitleOffset(0.95);
        g->GetYaxis()->SetTitleOffset(1.);
	g->SetMarkerStyle(20);
	g->SetMarkerColor(4);
	g->SetMarkerSize(1.4);
	g->Draw("apl");
	c1->Print("spmt_electronics_nonl.pdf");	
	for(int i=0;i<10;i++){
		f->WriteTObject(h[i]);
	}	

	TCanvas *al=new TCanvas("tw","tq",1);
	test->Draw();
}

