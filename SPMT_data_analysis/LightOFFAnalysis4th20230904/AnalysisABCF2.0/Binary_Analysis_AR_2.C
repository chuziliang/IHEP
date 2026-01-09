//g++ Binary_Analysis_AR.C -o Binary_Analysis_AR `root-config --cflags --libs`

#include <TFile.h>
#include <TApplication.h>
#include <TSystem.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include "TMinuit.h"
#include <TCanvas.h>
#include <TMath.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TH3.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TPad.h>
#include <TPaveStats.h>
#include <TList.h>
#include <TText.h>
#include <TString.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TF2.h>
#include <TGraph2D.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>
#include <THStack.h>
#include <TGaxis.h>
#include <TFrame.h>
#include <TChain.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TPaletteAxis.h>

#include <time.h>
#include <TVector.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>		// nécessaire pour utiliser les fonctions mathématiques cos, log, etc
#include <iomanip>
//#include "Data_format_v21.h"
#include "Data_format.h"
//#include "Event/SimHeader.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Histo definition and variable definitions --> will be accessible in all functions/loops

TString fp;
TString fn;
TFile *f;

bool DrawChargeHist = true;
bool DrawCoarseTimePlot = false;
bool DrawTFT_vs_EvtID = false;
bool DrawTrigger_width = false;

unsigned int evt_type;
unsigned int edge_type;

unsigned int dds_type;
unsigned int channel_nb;
unsigned int Coarse_time;
unsigned int Gain;
unsigned int Evt_counter_per_channel=0;
unsigned int Evt_counter_per_block=0;
unsigned int even_counter;
unsigned int odd_counter;

unsigned int Charge;
unsigned int Fine_time;

unsigned int channel_nb_discri;
unsigned int Channels_Number;
unsigned int Evt_counter=0;
unsigned int Trigger_width;
unsigned int Trigger_falling_time;
unsigned int Trigger_rising_time;
unsigned int Trigger_fine_time;

unsigned int Coarse_time_A;
unsigned int Coarse_time_B;
unsigned int Coarse_time_C;
unsigned int Coarse_time_D;

unsigned int Trigger_falling_time_A;
unsigned int Trigger_falling_time_B;
unsigned int Trigger_falling_time_C;
unsigned int Trigger_falling_time_D;

unsigned int Evt_counter_per_channel_A;
unsigned int Evt_counter_per_channel_B;
unsigned int Evt_counter_per_channel_C;
unsigned int Evt_counter_per_channel_D;

unsigned int Evt_counter_per_channel_discri_A;
unsigned int Evt_counter_per_channel_discri_B;
unsigned int Evt_counter_per_channel_discri_C;
unsigned int Evt_counter_per_channel_discri_D;

unsigned int Charge_A;
unsigned int Charge_B;

unsigned int Fine_time_A;
unsigned int Fine_time_B;

unsigned long Evt_ID;
unsigned long Blk_ID;

unsigned int Marker_A;
unsigned int Marker_B;
unsigned int Marker_C;
unsigned int Marker_D;
unsigned int Marker;

unsigned int Total_Length_A;
unsigned int Total_Length_B;
unsigned int Total_Length_C;
unsigned int Total_Length_D;
unsigned int Total_Length;

unsigned int Block_ID_A;
unsigned int Block_ID_B;
unsigned int Block_ID_C;
unsigned int Block_ID_D;
unsigned int Block_ID;

unsigned int Nb_of_Channels_A;
unsigned int Nb_of_Channels_B;
unsigned int Nb_of_Channels_C;
unsigned int Nb_of_Channels_D;
unsigned int Nb_of_Channels;

unsigned int Run_mode_A;
unsigned int Run_mode_B;
unsigned int Run_mode_C;
unsigned int Run_mode_D;
unsigned int Run_mode;

unsigned int CT_Overflow_Counter_A;
unsigned int CT_Overflow_Counter_B;
unsigned int CT_Overflow_Counter_C;
unsigned int CT_Overflow_Counter_D;
unsigned int CT_Overflow_Counter;

unsigned int Time_Stamp_A;
unsigned int Time_Stamp_B;
unsigned int Time_Stamp_C;
unsigned int Time_Stamp_D;
unsigned int Time_Stamp;
unsigned int length_Data = 4;
unsigned int length_Header = 4;

char *ABC_Data = new char[length_Data];
char *TSTMP_Data = new char[length_Data];
char *DISCRI_Data = new char[length_Data];
char *ABC_Header_1 = new char[length_Header];
char *ABC_Header_2 = new char[length_Header];
char *ABC_Header_3 = new char[length_Header];
char *ABC_Header_4 = new char[length_Header];
char *ABC_Header_5 = new char[length_Header];
char *ABC_Header_6 = new char[length_Header];
char *ABC_Trailer = new char[length_Header];


TTree *OutTree;
TTree *OutTree_Header;

vector<double> vector_Charge_ping[128];
vector<double> vector_Charge_pong[128];

vector<double> vector_TS_ping[128];
vector<double> vector_TS_pong[128];


vector<double> vector_TW_ping[256];
vector<double> vector_TW_pong[256];

vector<double> vector_Coarse_time_ping[256];
vector<double> vector_Coarse_time_pong[256];

vector<double> vector_Coarse_time_Overflow_Counter_ping[256];
vector<double> vector_Coarse_time_Overflow_Counter_pong[256];

vector<double> vector_Evt_ID_ping[256];
vector<double> vector_Evt_ID_pong[256];

vector<double> vector_TFT[256];
vector<double> vector_Evt_ID[256];

double number_of_events_ping[256];
vector<double> vector_number_of_events_ping[256];
double number_of_events_pong[256];
vector<double> vector_number_of_events_pong[256];

int Sign;


//------------ histos DDS -----------------
//TH1F* Histo_Trigger_width;
//TH1F* Histo_Trigger_falling_time;


//------------ histos charge stream -------
TH1F* Histo_Block_ID;
TH1F* Histo_channel_nb;
TH1F* Histo_Nb_of_read_Channels;
TH1F* Histo_Nb_of_expected_Channels;
TH1F* Histo_Time_Stamp_Coarse_time;

TH2F* Histo_channel_nb_vs_Charge;
TH2F* Histo_Nb_of_read_Channels_vs_Nb_of_expected_Channels;
TH2F* Histo_Time_Stamp_Coarse_time_vs_Evt_ID;

TH2 *ohistPing = new TH2F("charges HG ping","charges HG ping",128,0,127,1024,0,1024);
TH2 *ohistPong = new TH2F("charges HG pong","charges HG pong",128,0,127,1024,0,1024);
TH2 *ohistPingLG = new TH2F("charges LG ping","charges LG ping",128,0,127,1024,0,1024);
TH2 *ohistPongLG = new TH2F("charges LG pong","charges LG pong",128,0,127,1024,0,1024);

TH1F* Histo_Charge_ping_HG[128];
TH1F* Histo_Charge_pong_HG[128];
TH1F* Histo_Charge_ping_LG[128];
TH1F* Histo_Charge_pong_LG[128];

TH1F* Histo_Trigger_width[256];
TH1F* Histo_Trigger_falling_time[256];

TH1F* Histo_Coarse_time_ping[256];
TH1F* Histo_Coarse_time_pong[256];

TGraph *graph_Coarse_time_ping[256];
TGraph *graph_Coarse_time_pong[256];

TGraph *graph_TS_ping[256];
TGraph *graph_TS_pong[256];

TGraph *graph_TFT[256];

TGraph *graph_TW_ping[256];
TGraph *graph_TW_pong[256];

TGraph *graph_Coarse_time_Overflow_Counter_ping[256];
TGraph *graph_Coarse_time_Overflow_Counter_pong[256];

TH2F* Charge_vs_Evt_ID;

//vector<int> vector_Evt_ID;
vector<int> *charge_vector;

int Nb_of_expected_Channels = 0;
int Nb_of_read_Channels = 0;
int Channel_counter = 0;
int cat_data_counter = 0;
int dds_data_counter = 0;

TF1 *function_fit;

double mean_charge_ping[128];
double mean_charge_pong[128];
double charge_RMS_ping[128];
double charge_RMS_pong[128];

double error_mean_charge_ping[256];
double error_mean_charge_pong[256];
double error_charge_RMS_ping[256];
double error_charge_RMS_pong[256];
bool verbose = false;
int lecture = 0;
int range_min;
int range_max;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool fileExists(const char *fileName) /*avoids loading a non-existant file*/
{
	cout<<"file is "<<fileName<<endl;
	ifstream infile(fileName);
	return infile.good();
}



void Carte(TString file) //Fonction qui va lire le fichier binaire et stocker les informations dans un arbre, qui sera ensuite lu par le reste du programme
{
	ifstream f(file, ifstream::binary);


	Evt_ID = 0;

	while (!f.eof())
	{
		if (lecture == 0)
		{
			f.read(ABC_Header_1,length_Header);
		}
		if (isHeader(ABC_Header_1)) //Header
		{

			Channel_counter = 0;
			Evt_ID++;
			f.read(ABC_Header_2,length_Header);
			Block_ID = get_Block_ID(ABC_Header_2);
			Time_Stamp = get_Local_Time_Stamp(ABC_Header_2);
			//CT_Overflow_Counter = get_CT_0verflow(ABC_Header_2);		
			lecture = 0;

			if (verbose)
			{
				//cout << "[Info ] Header -> " << "Evt_ID: " << Evt_ID<< " Block_ID: " << Block_ID << " Time Stamp: "  << hex <<  Time_Stamp << " CT_Overflow: " << CT_Overflow_Counter<<endl;	
				cout << "[Info ] Header -> " << "Evt_ID: " << Evt_ID << " Block_ID: " << Block_ID << " Local Time Stamp: "  <<  Time_Stamp <<endl;				
			}

			/* if (Block_ID == 0)
			   {
			   cout << "[Info ] Header -> " << "Evt_ID: " << Evt_ID<< " Block_ID: " << Block_ID << " Time Stamp: "  << Time_Stamp << " CT_Overflow: " << CT_Overflow_Counter<<endl;				
			   } */
			OutTree_Header->Fill();
		}
		else
		{
			if (isTrailer(ABC_Header_1)) //Trailer 
			{
				f.read(ABC_Trailer,length_Header);
				Nb_of_expected_Channels = get_NbChannels_Readed(ABC_Trailer);
				Evt_counter_per_block++;
				//cout << "//-------Evt_ID = " << Evt_ID << " -------//" << endl;	
				//	
				if (verbose)
				{
					cout << "[Info ] Trailer -> "  << " Nb_of_expected_Channels: "  << Nb_of_expected_Channels <<  " Channel_Counter: " << Channel_counter<<  endl;
					if (( Nb_of_expected_Channels != Channel_counter) )
					{
						cout << "[Error ] .. "  << Nb_of_expected_Channels << " : "  << Channel_counter << endl;
					}

					cout << ""  << endl;
				}
				//cout << "[Info ] Trailer -> "  << "Total_Length: "  << Total_Length << " Nb_of_expected_Channels: "  << Nb_of_expected_Channels << " Nb_of_Readed_Channels: "  << Channel_counter <<  endl;
				if (verbose && (( Nb_of_expected_Channels != Channel_counter) || (Nb_of_expected_Channels == 0))) 
				{
					cout << "[Error ] .. "  << Nb_of_expected_Channels << " : "  << Channel_counter << endl;

				}
				//cout << ""  << endl;
				OutTree->Fill();
				OutTree_Header->Fill();
			}
			else //Channel
			{
				evt_type = ABC_Header_1[0] >> 6 & 0x03;
				//edge_type = (ABC_Header_1[0] & 0x20)  >>  5 ;

				if (( evt_type == 0) || ( evt_type == 1))
				{
					cat_data_counter++;
					Channel_counter++;				
					//channel_nb = ABC_Header_1[0] >> 2 & 0xFFu;
					channel_nb = get_cat_channel_nb(ABC_Header_1);
					channel_nb = channel_nb + Block_ID*8;

					Coarse_time = 	get_coarse_time(ABC_Header_1);				

					f.read(ABC_Data,length_Data);
					// Gain on 1 bit: 0=HG, 1=LG
					Gain = get_gain(ABC_Data);													
					Evt_counter_per_channel = get_Evt_counter_per_channel(ABC_Data);

					if (Evt_counter_per_channel%2== 0) 		// If PING or Evt_counter even with Sign==0
					{
						Sign=0;
					}
					else 
					{
						Sign=1; 					// If PONG or Evt_counter odd with Sign==1
					}
					// Charge on 10 bits						

					Charge = get_Charge(ABC_Data);

					// Fine time on 10 bits		

					Fine_time = get_Fine_time(ABC_Data);

					//Coarse_time = Coarse_time * 24 + Fine_time ;

					if (verbose)
					{
						cout << "[Info ] Catiroc-> " << "channel_nb: " << channel_nb << " Coarse_time: "  << Coarse_time<< " Gain: " << Gain << " Charge: " << Charge << " Fine time: " << Fine_time <<endl;
					}
					// Remplissage de l'arbre		
					//cout<<"Remplissage de l'arbre "<<endl;
					OutTree->Fill();
				}
				else
				{
					// Décodage des données type DDS
					if ( evt_type == 2)
					{
						/*
						edge_type = (ABC_Header_1[0] & 0x20)  >>  5 ;
						if (edge_type == 0) 
							{*/
								
								dds_data_counter++;
								Channel_counter++;

								channel_nb = get_dds_channel_nb(ABC_Header_1);

								channel_nb = channel_nb + Block_ID*8;


								// Event counter per channel on 18 bits			

								Evt_counter = get_discri_Evt_counter_per_channel(ABC_Header_1);
								// Trigger Width  on 7 bits
								Trigger_width = get_Trigger_width_time(ABC_Header_1);

								
								f.read(DISCRI_Data,length_Data);

								// Trigger Time  on 26 bits

								
									
									Trigger_falling_time = get_Trigger_falling_time(DISCRI_Data);

									Trigger_fine_time = get_discri_fine_time(DISCRI_Data);
									Trigger_falling_time = Trigger_falling_time * 24 + Trigger_fine_time ;


									if (verbose )
									{
										
										if (( Trigger_width == 127) || ( Trigger_width == 0))
										{		 
											cout << "[Error ] DDS-> " << "channel_nb: " << channel_nb <<  " Evt_counter: " << Evt_counter<< " Falling_time: " << Trigger_falling_time<< " Width_time: " << Trigger_width <<endl;
										}
										else
										{
											//if (( channel_nb == 0) || (channel_nb == 8))
											//{
											cout << "[Info ] DDS-> " << "channel_nb: " << channel_nb <<  " Evt_counter: " << Evt_counter<< " Falling_time: " << Trigger_falling_time<< " Width_time: " << Trigger_width <<endl;
											//}
										}
										
									}
									
									OutTree->Fill();
							
							//}
					}	
				}
			}
			//			lecture = 1;
		}
	}
	cout<<"end of while loop"<<endl;
}
//}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Main function

int main(int argc,char** argv)
{
	//Ouverture du fichier -------------------------------------------------------------------------------------------------------------
	
	ifstream file_ref_1;
  	file_ref_1.open("/junofs/users/chuziliang125/SPMT_data_analysis/LightOFFAnalysis4th20230904/AnalysisABCF2.0/input_binary_filename.txt");
	
	string filename;
	string OutputDirectory;
	int target_line;

	OutputDirectory="/junofs/users/chuziliang125/SPMT_data_analysis/LightOFFAnalysis4th20230904/Result/";

	for (int i = 1; i < argc; i++)
	{
		if (std::string(argv[i]) == "-o" && i + 1 < argc)
		{
			OutputDirectory = argv[i + 1];
		}
		if (std::string(argv[i]) == "-i" && i + 1 < argc)
		{
			target_line = std::atoi(argv[i + 1]);
			cout<<"target line is "<<target_line<<endl;
		}
	}

	//while (!file_ref_1.eof())
	{
		string line;
		int current_line = 0; // 当前行号
    	while (getline(file_ref_1, filename)) 
		{
			if (current_line == target_line) 
			{
				line = "/junofs/users/chuziliang125/SPMT_data_analysis/LightOFFAnalysis4th20230904/binary/"+filename;
				cout<<"filename is "<<line<<endl;
				break; // 已经获取到目标行，跳出循环
			}
        current_line++; // 更新当前行号
    	}
		// getline(file_ref_1, filename);
		// string line = "/junofs/users/chuziliang125/LightOffAnalysis20230619/binary/"+filename;
		

		string HV ="HV";
		string slash ="/";
		size_t pos1, pos2= 0;
		
		string ABCnumber = "ABC_";
		int nb_channels_max = 128;
		
		
		//ofstream rootfile("input_root_filename.txt");
		
		//Creation de l'arbre et des histos -------------------------------------------------------------------------------------------------------------

		OutTree = new TTree("OutTree","tree");
		OutTree->Branch("evt_type",&evt_type,"evt_type/i");
		OutTree->Branch("Evt_ID",&Evt_ID,"Evt_ID/i");
		OutTree->Branch("Block_ID",&Block_ID,"Block_ID/i");
		OutTree->Branch("channel_nb",&channel_nb,"channel_nb/i");

		OutTree->Branch("Coarse_time",&Coarse_time,"Coarse_time/i");
		OutTree->Branch("Fine_time",&Fine_time,"Fine_time/i");
		OutTree->Branch("Charge",&Charge,"Charge/i");
		OutTree->Branch("Gain",&Gain,"Gain/i");
		OutTree->Branch("Sign",&Sign,"Sign/i");

		OutTree->Branch("Trigger_falling_time",&Trigger_falling_time,"Trigger_falling_time/i");
		OutTree->Branch("Trigger_width",&Trigger_width,"Trigger_width/i");
		OutTree->Branch("Evt_counter",&Evt_counter,"Evt_counter/i");

		OutTree_Header = new TTree("OutTree_Header","tree_header");
		OutTree_Header->Branch("Time_Stamp",&Time_Stamp,"Time_Stamp/i");
		OutTree_Header->Branch("Nb_of_expected_Channels",&Nb_of_expected_Channels,"Nb_of_expected_Channels/i");
		//OutTree_Header->Branch("nb_read_channels",&nb_read_channels,"nb_read_channels/i");
		OutTree_Header->Branch("Evt_ID",&Evt_ID,"Evt_ID/i");

		

		//Histo_Trigger_width = new TH1F("Histo_Trigger_width","Trigger_width",128,0,127);

		for (int i=0;i<128;i++)
		{
			TString name_Histo_Charge_ping_HG = "Histo_Charge_HG_" + TString(Form("%i",i)) + "_ping";
			TString title_Histo_Charge_ping_HG = "HG Charges in channel " + TString(Form("%i",i)) + " (ping)";

			Histo_Charge_ping_HG[i] = new TH1F(name_Histo_Charge_ping_HG,title_Histo_Charge_ping_HG,1024,0,1024);
			Histo_Charge_ping_HG[i]->GetXaxis()->SetTitle("Charge (UDAC)");
			Histo_Charge_ping_HG[i]->GetYaxis()->SetTitle("Nb Events");

			TString name_Histo_Charge_ping_LG = "Histo_Charge_LG_" + TString(Form("%i",i)) + "_ping";
			TString title_Histo_Charge_ping_LG = "LG Charges in channel " + TString(Form("%i",i)) + " (ping)";

			Histo_Charge_ping_LG[i] = new TH1F(name_Histo_Charge_ping_LG,title_Histo_Charge_ping_LG,1024,0,1024);
			Histo_Charge_ping_LG[i]->GetXaxis()->SetTitle("Charge (UDAC)");
			Histo_Charge_ping_LG[i]->GetYaxis()->SetTitle("Nb Events");

			TString name_Histo_Charge_pong_HG = "Histo_Charge_HG_" + TString(Form("%i",i)) + "_pong";
			TString title_Histo_Charge_pong_HG = "HG Charges in channel " + TString(Form("%i",i)) + " (pong)";

			Histo_Charge_pong_HG[i] = new TH1F(name_Histo_Charge_pong_HG,title_Histo_Charge_pong_HG,1024,0,1024);
			Histo_Charge_pong_HG[i]->GetXaxis()->SetTitle("Charge (UDAC)");
			Histo_Charge_pong_HG[i]->GetYaxis()->SetTitle("Nb Events");

			TString name_Histo_Charge_pong_LG = "Histo_Charge_LG_" + TString(Form("%i",i)) + "_pong";
			TString title_Histo_Charge_pong_LG = "LG Charges in channel " + TString(Form("%i",i)) + " (pong)";

			Histo_Charge_pong_LG[i] = new TH1F(name_Histo_Charge_pong_LG,title_Histo_Charge_pong_LG,1024,0,1024);
			Histo_Charge_pong_LG[i]->GetXaxis()->SetTitle("Charge (UDAC)");
			Histo_Charge_pong_LG[i]->GetYaxis()->SetTitle("Nb Events");
			
			TString name_Histo_Trigger_width = "Histo_Trigger_width_" + TString(Form("%i",i));
			TString title_Histo_Trigger_width = "Trigger width in channel " + TString(Form("%i",i));
			Histo_Trigger_width[i] = new TH1F(name_Histo_Trigger_width,title_Histo_Trigger_width,50,0,59);
			
			TString name_Histo_Trigger_falling_time = "Histo_Trigger_falling_time_" + TString(Form("%i",i));
			TString title_Histo_Trigger_falling_time = "Trigger falling time in channel " + TString(Form("%i",i));
			Histo_Trigger_falling_time[i] = new TH1F(name_Histo_Trigger_falling_time,title_Histo_Trigger_falling_time,1024,0,127);
			
			TString name_Histo_Coarse_time_ping = "Histo_CT_" + TString(Form("%i",i)) + "_ping";
			TString title_Histo_Coarse_time_ping = "Coarse time in channel " + TString(Form("%i",i)) + " (ping)";
			
			Histo_Coarse_time_ping[i] = new TH1F(name_Histo_Coarse_time_ping,title_Histo_Coarse_time_ping,1000,0,200000);
			Histo_Coarse_time_ping[i]->GetXaxis()->SetTitle("Coarse Time (25*ns)");
			Histo_Coarse_time_ping[i]->GetYaxis()->SetTitle("Nb Events");
			
			TString name_Histo_Coarse_time_pong = "Histo_CT_" + TString(Form("%i",i)) + "_pong";
			TString title_Histo_Coarse_time_pong = "Coarse time in channel " + TString(Form("%i",i)) + " (pong)";
			
			Histo_Coarse_time_pong[i] = new TH1F(name_Histo_Coarse_time_pong,title_Histo_Coarse_time_pong,1000,0,200000);
			Histo_Coarse_time_pong[i]->GetXaxis()->SetTitle("Coarse Time (25*ns)");
			Histo_Coarse_time_pong[i]->GetYaxis()->SetTitle("Nb Events");
			
			
			
		}


		//Remplissage de l'arbre -------------------------------------------------------------------------------------------------------------

		if (!fileExists(line.c_str()))
		{
			cout << endl << "The file you wish to open, " << line << ", does not exist, or at least not in this directory." << endl << endl;
			return 0;
		}

		/* if (strcmp(argv[1],"true") == 0)
		{
		Carte(line, true, strtol(argv[2], NULL, 10)); // <-- Appel de la fonction principale !
		}
		else
		{
		Carte(line, false, 0); // <-- Appel de la fonction principale !
		} */


		Carte(line); // <-- Appel de la fonction principale !
		cout << endl << "File " << line << " analyzed" << endl;

		string delimiter = "/";
		size_t pos = 0;
		string token;
		
		while ((pos = line.find(delimiter)) != string::npos)
		{
				line.erase(0, pos + delimiter.length());
		}
		pos = line.size()-4;
		token = line.substr(0, pos);
				
		cout << "the token is "<<token<<endl;

		TFile *ofile = new TFile((OutputDirectory+"root/"+token+".root").c_str(),"recreate");


		int n_CT_reset_ping[128];
		int n_CT_reset_pong[128];

		for (int e=0;e<128;e++)
		{
			n_CT_reset_ping[e] = 0;
			n_CT_reset_pong[e] = 0;
		}
		for (int e=0;e<OutTree->GetEntries();e++)
		{
			OutTree->GetEntry(e);
			//Histo_Charge_ping_HG[channel_nb]->Fill(Charge);

			//cout << "Channel_nb " << channel_nb << " Trigger_width " << Trigger_width << endl;
			if (DrawTrigger_width) Histo_Trigger_width[channel_nb]->Fill(Trigger_width);
			//Histo_Trigger_falling_time[channel_nb]->Fill(Trigger_falling_time);
			//cout << channel_nb << ": " << Trigger_falling_time << ": " << Evt_counter << endl;
			if (DrawTFT_vs_EvtID) vector_TFT[channel_nb].push_back(Trigger_falling_time);
			//cout << channel_nb << ": " << vector_TFT[channel_nb] << endl;
			if (DrawTFT_vs_EvtID) vector_Evt_ID[channel_nb].push_back(Evt_counter);

			if (e%2 == 0) 		// If PING
			{
				if (Gain == 1)
				{
					Histo_Charge_ping_HG[channel_nb]->Fill(Charge);
					ohistPing->Fill(channel_nb,Charge);
				}
				else if (Gain == 0)
				{
					Histo_Charge_ping_LG[channel_nb]->Fill(Charge);
					ohistPingLG->Fill(channel_nb,Charge);
				}
				
				Histo_Coarse_time_ping[channel_nb]->Fill(Coarse_time);
				//cout <<  "Ping --> channel_nb : "<< channel_nb << " Coarse_time : " <<Coarse_time <<  endl;

				vector_Coarse_time_ping[channel_nb].push_back(Coarse_time);
				vector_Evt_ID_ping[channel_nb].push_back(Evt_ID);

				if (vector_Coarse_time_ping[channel_nb].size() > 1 && vector_Coarse_time_ping[channel_nb][vector_Coarse_time_ping[channel_nb].size()-1] < vector_Coarse_time_ping[channel_nb][vector_Coarse_time_ping[channel_nb].size()-2])
				{
					n_CT_reset_ping[channel_nb] = n_CT_reset_ping[channel_nb] + 1;
				}


			}

			else 	// If PONG
			{
				if (Gain == 1 )
				{
					Histo_Charge_pong_HG[channel_nb]->Fill(Charge);
					ohistPong->Fill(channel_nb,Charge);
				}
				else if (Gain == 0)
				{
					Histo_Charge_pong_LG[channel_nb]->Fill(Charge);
					ohistPongLG->Fill(channel_nb,Charge);
				}
				Histo_Coarse_time_pong[channel_nb]->Fill(Coarse_time);
				vector_Coarse_time_pong[channel_nb].push_back(Coarse_time);
				vector_Evt_ID_pong[channel_nb].push_back(Evt_ID);
				if (vector_Coarse_time_pong[channel_nb].size() > 1 && vector_Coarse_time_pong[channel_nb][vector_Coarse_time_pong[channel_nb].size()-1] < vector_Coarse_time_pong[channel_nb][vector_Coarse_time_pong[channel_nb].size()-2])
				{
					n_CT_reset_pong[channel_nb] = n_CT_reset_pong[channel_nb] + 1;
				}

			}

		}
		
		//cout << "Taille --> " << vector_TFT[8].size() << endl;
		//cout << "Taille Coarse time--> " << vector_Coarse_time_ping[8].size() << endl;
		TString name_pdf;
		int compteur_canvas = 0;
		int compteur_plots = 0;
		
		TCanvas* graph_TFT_pdf = new TCanvas("graph_TFT_vs_EvtID_pdf");
		graph_TFT_pdf->Divide(4,2);
		for(int i=0;i<128; i++)
		{
			//cout << "Vector size " << vector_TFT[i].size() << endl;
			graph_TFT_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			graph_TFT[i] = new TGraph(vector_TFT[i].size(),&vector_Evt_ID[i][0],&vector_TFT[i][0]);
			TString name_graph_TFT = "graph_TFT_vs_EvtID_channel_" + TString(Form("%i",i));
			TString title_graph_TFT = "Trigger Falling Time wrt EvtID for channel " + TString(Form("%i",i));
			graph_TFT[i]->SetNameTitle(name_graph_TFT,title_graph_TFT);
			graph_TFT[i]->GetXaxis()->SetTitle("Evt_ID");
			graph_TFT[i]->GetYaxis()->SetTitle("Time Stamp (1*ns)");
			graph_TFT[i]->Draw("AL");
			compteur_plots++;
			
			if (DrawTFT_vs_EvtID)
			{
				if (compteur_plots == 8 || i == 127)
				{
					compteur_canvas++;

					compteur_plots = 0;
					
					if (compteur_canvas == 1)
					{
						name_pdf = OutputDirectory+"pdf/"+ token + "_TFT_vs_EvtID_ping.pdf(";
						graph_TFT_pdf->Print(name_pdf,"pdf");
					}
					else if (i==127)
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_TFT_vs_EvtID_ping.pdf)";
						graph_TFT_pdf->Print(name_pdf,"pdf");
					}
					else
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_TFT_vs_EvtID_ping.pdf";
						graph_TFT_pdf->Print(name_pdf,"pdf");
					}
					graph_TFT_pdf->Clear();
					graph_TFT_pdf->Divide(4,2);
				}
			}
			
		}

		compteur_canvas = 0;
		compteur_plots = 0;
			
		TCanvas* Histo_Coarse_time_ping_pdf = new TCanvas("Histo_Coarse_time_ping_pdf");
		Histo_Coarse_time_ping_pdf->Divide(4,2);
		for(int i=0;i<128; i++)
		{
			Histo_Coarse_time_ping_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			graph_Coarse_time_ping[i] = new TGraph(vector_Coarse_time_ping[i].size(),&vector_Evt_ID_ping[i][0],&vector_Coarse_time_ping[i][0]);
			TString name_graph_CT = "graph_CT_vs_EvtID_channel_" + TString(Form("%i",i));
			TString title_graph_CT = "Coarse Time wrt EvtID for channel " + TString(Form("%i",i));
			graph_Coarse_time_ping[i]->SetNameTitle(name_graph_CT,title_graph_CT);
			graph_Coarse_time_ping[i]->GetXaxis()->SetTitle("Evt_ID");
			graph_Coarse_time_ping[i]->GetYaxis()->SetTitle("Time Stamp (25*ns)");
			graph_Coarse_time_ping[i]->Draw("AP");

			graph_Coarse_time_ping[i]->Write();
			
			
			//Histo_Coarse_time_ping_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			//gPad->SetLogy();
			//Histo_Coarse_time_ping[i]->Draw();
			compteur_plots++;
			if (DrawCoarseTimePlot)
			{
				if (compteur_plots == 8 || i == 127)
				{
					compteur_canvas++;
					compteur_plots = 0;
					
					if (compteur_canvas == 1)
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_Coarse_time_ping.pdf(";
						Histo_Coarse_time_ping_pdf->Print(name_pdf,"pdf");
					}
					else if (i==127)
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_Coarse_time_ping.pdf)";
						Histo_Coarse_time_ping_pdf->Print(name_pdf,"pdf");
					}
					else
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_Coarse_time_ping.pdf";
						Histo_Coarse_time_ping_pdf->Print(name_pdf,"pdf");
					}
					Histo_Coarse_time_ping_pdf->Clear();
					Histo_Coarse_time_ping_pdf->Divide(4,2);
				}
			}
			
		}
		
		
		compteur_canvas = 0;
		compteur_plots = 0;

	//------------- Trigger Width ------------------------
	/*
		compteur_canvas = 0;
		compteur_plots = 0;

		TCanvas* Histo_Trigger_width_pdf = new TCanvas("Histo_Trigger_width_pdf");
		//gPad->SetLogy();		
		Histo_Trigger_width->Draw();
		name_pdf = OutputDirectory+token + "_Trigger_width.png";
		Histo_Trigger_width_pdf->Print(name_pdf,"png");
	*/
	//-----------------------------------------------

		TString CanvasName = "c1"+token;
		TCanvas *c1 = new TCanvas(CanvasName,CanvasName,1);
		c1->Divide(2,1);
		c1->cd(1);
		gPad->SetLogz();
		ohistPing->GetXaxis()->SetTitle("Channel ID");
		// ohistPing->GetYaxis()->SetTitle("Charge [ADCu]");
		// 单独保存ping直方图到新的root文件
		TFile* file_ping = new TFile((OutputDirectory+"root/"+token+"_ping_Hist.root").c_str(),"RECREATE");
		ohistPing->Write();
		file_ping->Close();

		ohistPing->GetYaxis()->SetLabelFont( 63  );
		ohistPing->GetYaxis()->SetLabelSize( 11 );
		gStyle->SetOptStat(kFALSE);
		ohistPing->Draw("COLZ");
		ohistPing->Write();
		c1->Update();
		TPaletteAxis *palette1 = (TPaletteAxis*)ohistPing->GetListOfFunctions()->FindObject("palette");
		if (palette1 != nullptr)
		{
			palette1->SetX1NDC(0.902);
			palette1->SetX2NDC(0.932);
			c1->Modified();
			c1->Update();
		}
		c1->cd(2);
		gPad->SetLogz();
		ohistPong->GetXaxis()->SetTitle("Channel ID");
		// ohistPong->GetYaxis()->SetTitle("Charge [ADCu]");
		ohistPong->GetYaxis()->SetLabelFont( 63  );
		ohistPong->GetYaxis()->SetLabelSize( 11 );
		gStyle->SetOptStat(kFALSE);
		ohistPong->Draw("COLZ");
		c1->Update();
		TPaletteAxis *palette2 = (TPaletteAxis*)ohistPong->GetListOfFunctions()->FindObject("palette");
		if (palette2 != nullptr)
		{
			palette2->SetX1NDC(0.902);
			palette2->SetX2NDC(0.932);
			c1->Modified();
			c1->Update();
		}

		CanvasName = "c2"+token;
		TCanvas *c2 = new TCanvas(CanvasName,CanvasName,1);
		c2->Divide(2,1);
		c2->cd(1);
		gPad->SetLogz();
		ohistPingLG->GetXaxis()->SetTitle("Channel ID");
		ohistPingLG->GetYaxis()->SetTitle("Charge [ADCu]");
		ohistPingLG->GetYaxis()->SetLabelFont( 63  );
		ohistPingLG->GetYaxis()->SetLabelSize( 11 );
		gStyle->SetOptStat(kFALSE);
		ohistPingLG->Draw("COLZ");
		c2->Update();
		TPaletteAxis *palette3 = (TPaletteAxis*)ohistPingLG->GetListOfFunctions()->FindObject("palette");
		if (palette3 != nullptr)
		{
			palette3->SetX1NDC(0.902);
			palette3->SetX2NDC(0.932);
			c2->Modified();
			c2->Update();
		}
		
		

		c2->cd(2);
		gPad->SetLogz();
		ohistPongLG->GetXaxis()->SetTitle("Channel ID");
		ohistPongLG->GetYaxis()->SetTitle("Charge [ADCu]");
		ohistPongLG->GetYaxis()->SetLabelFont( 63  );
		ohistPongLG->GetYaxis()->SetLabelSize( 11 );
		gStyle->SetOptStat(kFALSE);
		ohistPongLG->Draw("COLZ");
		c2->Update();
		TPaletteAxis *palette4 = (TPaletteAxis*)ohistPongLG->GetListOfFunctions()->FindObject("palette");
		if (palette4 != nullptr)
		{
			palette4->SetX1NDC(0.902);
			palette4->SetX2NDC(0.932);
			c2->Modified();
			c2->Update();
		}

		c1->Print((OutputDirectory+"pdf/"+token+".pdf(").c_str(),"pdf");
		c2->Print((OutputDirectory+"pdf/"+token+".pdf").c_str(),"pdf");

		TH1* HistRMSPingHG = new TH1F("RMS Ping HG","RMS Ping HG",128,0,128);
		TH1* HistRMSPongHG = new TH1F("RMS Pong HG","RMS Pong HG",128,0,128);
		TH1* HistRMSPingLG = new TH1F("RMS Ping LG","RMS Ping LG",128,0,128);
		TH1* HistRMSPongLG = new TH1F("RMS Pong LG","RMS Pong LG",128,0,128);

		TH1* HistMeanPingHG = new TH1F("Mean Ping HG","Mean Ping HG",128,0,128);
		TH1* HistMeanPongHG = new TH1F("Mean Pong HG","Mean Pong HG",128,0,128);
		TH1* HistMeanPingLG = new TH1F("Mean Ping LG","Mean Ping LG",128,0,128);
		TH1* HistMeanPongLG = new TH1F("Mean Pong LG","Mean Pong LG",128,0,128);

		for (int iChn=0; iChn<128; iChn++) 
		{
			double RMSHGPing = Histo_Charge_ping_HG[iChn] -> GetRMS();
			HistRMSPingHG->SetBinContent(iChn+1,RMSHGPing);
			double RMSLGPing = Histo_Charge_ping_LG[iChn] -> GetRMS();
			HistRMSPingLG->SetBinContent(iChn+1,RMSLGPing);
			double MeanHGPing = Histo_Charge_ping_HG[iChn] -> GetMean();
			HistMeanPingHG->SetBinContent(iChn+1,MeanHGPing);
			double MeanLGPing = Histo_Charge_ping_LG[iChn] -> GetMean();
			HistMeanPingLG->SetBinContent(iChn+1,MeanLGPing);

			double RMSHGPong = Histo_Charge_pong_HG[iChn] -> GetRMS();
			HistRMSPongHG->SetBinContent(iChn+1,RMSHGPong);
			double RMSLGPong = Histo_Charge_pong_LG[iChn] -> GetRMS();
			HistRMSPongLG->SetBinContent(iChn+1,RMSLGPong);
			double MeanHGPong = Histo_Charge_pong_HG[iChn] -> GetMean();
			HistMeanPongHG->SetBinContent(iChn+1,MeanHGPong);
			double MeanLGPong = Histo_Charge_pong_LG[iChn] -> GetMean();
			HistMeanPongLG->SetBinContent(iChn+1,MeanLGPong);
		}
			
		

		// HistMeanPingHG->GetYaxis()->SetRangeUser(0,140);
		// HistMeanPongHG->GetYaxis()->SetRangeUser(0,140);
		// HistMeanPingLG->GetYaxis()->SetRangeUser(0,140);
		// HistMeanPongLG->GetYaxis()->SetRangeUser(0,140);

		// HistRMSPingHG->GetYaxis()->SetRangeUser(0,14);
		// HistRMSPongHG->GetYaxis()->SetRangeUser(0,14);
		// HistRMSPingLG->GetYaxis()->SetRangeUser(0,14);
		// HistRMSPongLG->GetYaxis()->SetRangeUser(0,14);

		TCanvas *c5;
		CanvasName = "c5"+token;
		c5 = new TCanvas(CanvasName);
		c5->Divide(2,1);
		c5->cd(1);
		HistRMSPingHG->Draw();
		c5->Update();
		c5->cd(2);
		HistRMSPongHG->Draw();
		c5->Update();
		ofile->cd();
		ofile->cd("canvas");
		c5->Write();
		c5->Print((OutputDirectory+"pdf/"+token+".pdf").c_str(),"pdf");

		TCanvas *c6;
		CanvasName = "c6"+token;
		c6 = new TCanvas(CanvasName);
		c6->Divide(2,1);
		c6->cd(1);
		HistRMSPingLG->Draw();
		c6->Update();
		c6->cd(2);
		HistRMSPongLG->Draw();
		c6->Update();
		ofile->cd();
		ofile->cd("canvas");
		c6->Write();
		c6->Print((OutputDirectory+"pdf/"+token+".pdf").c_str(),"pdf");

		TCanvas *c7;
		CanvasName = "c7"+token;
		c7 = new TCanvas(CanvasName);
		c7->Divide(2,1);
		c7->cd(1);
		HistMeanPingHG->Draw();
		c7->Update();
		c7->cd(2);
		HistMeanPongHG->Draw();
		c7->Update();
		ofile->cd();
		ofile->cd("canvas");
		c7->Write();
		c7->Print((OutputDirectory+"pdf/"+token+".pdf").c_str(),"pdf");

		TCanvas *c8;
		CanvasName = "c8"+token;
		c8 = new TCanvas(CanvasName);
		c8->Divide(2,1);
		c8->cd(1);
		HistMeanPingLG->Draw();
		c8->Update();
		c8->cd(2);
		HistMeanPongLG->Draw();
		c8->Update();
		ofile->cd();
		ofile->cd("canvas");
		c8->Write();
		c8->Print((OutputDirectory+"pdf/"+token+".pdf").c_str(),"pdf");

		compteur_canvas = 0;
		compteur_plots = 0;
		
		TCanvas* Histo_Charge_ping_HG_pdf = new TCanvas("Histo_Charge_ping_HG_pdf");
		Histo_Charge_ping_HG_pdf->Divide(4,2);
		for(int i=0;i<128; i++)
		{
			Histo_Charge_ping_HG_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			//gPad->SetLogy();
			Histo_Charge_ping_HG[i]->Draw();
			compteur_plots++;
			
			if (DrawChargeHist)
			{
				if (compteur_plots == 8 || i == 127)
				{
					compteur_canvas++;
					compteur_plots = 0;
					
					// if (compteur_canvas == 1)
					// {
					// 	name_pdf = OutputDirectory+token + ".pdf(";
					// 	Histo_Charge_ping_HG_pdf->Print(name_pdf,"pdf");
					// }
					// else
					{
						name_pdf = OutputDirectory+"pdf/"+token + ".pdf";
						Histo_Charge_ping_HG_pdf->Print(name_pdf,"pdf");
					}
					Histo_Charge_ping_HG_pdf->Clear();
					Histo_Charge_ping_HG_pdf->Divide(4,2);
				}
			}
			
		}

		compteur_canvas = 0;
		compteur_plots = 0;
		
		TCanvas* Histo_Charge_ping_LG_pdf = new TCanvas("Histo_Charge_ping_LG_pdf");
		Histo_Charge_ping_LG_pdf->Divide(4,2);
		for(int i=0;i<128; i++)
		{
			Histo_Charge_ping_LG_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			// gPad->SetLogy();
			Histo_Charge_ping_LG[i]->Draw();
			compteur_plots++;
			
			if (DrawChargeHist)
			{
				if (compteur_plots == 8 || i == 127)
				{
					compteur_canvas++;
					compteur_plots = 0;
					
					// if (compteur_canvas == 1)
					// {
					// 	name_pdf = OutputDirectory+token + ".pdf(";
					// 	Histo_Charge_ping_LG_pdf->Print(name_pdf,"pdf");
					// }
					// else if (i==127)
					// {
					// 	name_pdf = OutputDirectory+token + ".pdf)";
					// 	Histo_Charge_ping_LG_pdf->Print(name_pdf,"pdf");
					// }
					// else
					{
						name_pdf = OutputDirectory+"pdf/"+token + ".pdf";
						Histo_Charge_ping_LG_pdf->Print(name_pdf,"pdf");
					}
					Histo_Charge_ping_LG_pdf->Clear();
					Histo_Charge_ping_LG_pdf->Divide(4,2);
				}
			}
			
		}

		compteur_canvas = 0;
		compteur_plots = 0;
		
		TCanvas* Histo_Charge_pong_HG_pdf = new TCanvas("Histo_Charge_pong_HG_pdf");
		Histo_Charge_pong_HG_pdf->Divide(4,2);
		for(int i=0;i<128; i++)
		{
			Histo_Charge_pong_HG_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			// gPad->SetLogy();
			Histo_Charge_pong_HG[i]->Draw();
			compteur_plots++;
			
			if (DrawChargeHist)
			{
				if (compteur_plots == 8 || i == 127)
				{
					compteur_canvas++;
					compteur_plots = 0;
					
					// if (compteur_canvas == 1)
					// {
					// 	name_pdf = OutputDirectory+token + ".pdf(";
					// 	Histo_Charge_pong_HG_pdf->Print(name_pdf,"pdf");
					// }
					// else if (i==127)
					// {
					// 	name_pdf = OutputDirectory+token + ".pdf)";
					// 	Histo_Charge_pong_HG_pdf->Print(name_pdf,"pdf");
					// }
					// else
					{
						name_pdf = OutputDirectory+"pdf/"+token + ".pdf";
						Histo_Charge_pong_HG_pdf->Print(name_pdf,"pdf");
					}
					Histo_Charge_pong_HG_pdf->Clear();
					Histo_Charge_pong_HG_pdf->Divide(4,2);
				}
			}
			
		}

		compteur_canvas = 0;
		compteur_plots = 0;
		
		TCanvas* Histo_Charge_pong_LG_pdf = new TCanvas("Histo_Charge_pong_LG_pdf");
		Histo_Charge_pong_LG_pdf->Divide(4,2);
		for(int i=0;i<128; i++)
		{
			Histo_Charge_pong_LG_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			// gPad->SetLogy();
			Histo_Charge_pong_LG[i]->Draw();
			compteur_plots++;
			
			if (DrawChargeHist)
			{
				if (compteur_plots == 8 || i == 127)
				{
					compteur_canvas++;
					compteur_plots = 0;
					
					// if (compteur_canvas == 1)
					// {
					// 	name_pdf = OutputDirectory+token + ".pdf(";
					// 	Histo_Charge_pong_LG_pdf->Print(name_pdf,"pdf");
					// }
					if (i==127)
					{
						name_pdf = OutputDirectory+"pdf/"+token + ".pdf)";
						Histo_Charge_pong_LG_pdf->Print(name_pdf,"pdf");
					}
					else
					{
						name_pdf = OutputDirectory+"pdf/"+token + ".pdf";
						Histo_Charge_pong_LG_pdf->Print(name_pdf,"pdf");
					}
					Histo_Charge_pong_LG_pdf->Clear();
					Histo_Charge_pong_LG_pdf->Divide(4,2);
				}
			}
			
		}

				
		

		
		compteur_canvas = 0;
		compteur_plots = 0;
	/*
		TCanvas* Histo_Charge_pong_HG_pdf = new TCanvas("Histo_Charge_pong_HG_pdf");
		Histo_Charge_pong_HG_pdf->Divide(4,2);
		for(int i=0;i<128; i++)
		{
			Histo_Charge_pong_HG_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			gPad->SetLogy();
			Histo_Charge_pong_HG[i]->Draw();
			compteur_plots++;
			
			if (compteur_plots == 8 || i == 127)
			{
				compteur_canvas++;
				compteur_plots = 0;
				
				if (compteur_canvas == 1)
				{
					name_pdf = OutputDirectory+token + "_Charge_pong.pdf(";
					Histo_Charge_pong_HG_pdf->Print(name_pdf,"pdf");
				}
				else if (i==127)
				{
					name_pdf = OutputDirectory+token + "_Charge_pong.pdf)";
					Histo_Charge_pong_HG_pdf->Print(name_pdf,"pdf");
				}
				else
				{
					name_pdf = OutputDirectory+token + "_Charge_pong.pdf";
					Histo_Charge_pong_HG_pdf->Print(name_pdf,"pdf");
				}
				Histo_Charge_pong_HG_pdf->Clear();
				Histo_Charge_pong_HG_pdf->Divide(4,2);
			}
		}

	*/

		compteur_canvas = 0;
		compteur_plots = 0;

		TCanvas* Histo_Trigger_width_pdf = new TCanvas("Histo_Trigger_width_pdf");
		Histo_Trigger_width_pdf->Divide(4,2);
		
		for(int i=0;i<128; i++)
		{
			
			Histo_Trigger_width_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			// gPad->SetLogy();
			Histo_Trigger_width[i]->Draw();
			compteur_plots++;
			
			if (DrawTrigger_width)
			{
				if (compteur_plots == 8 || i == 127)
				{
					compteur_canvas++;
					compteur_plots = 0;
					
					if (compteur_canvas == 1)
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_Trigger_width.pdf(";
						Histo_Trigger_width_pdf->Print(name_pdf,"pdf");
					}
					else if (i==127)
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_Trigger_width.pdf)";
						Histo_Trigger_width_pdf->Print(name_pdf,"pdf");
					}
					else
					{
						name_pdf = OutputDirectory+"pdf/"+token + "_Trigger_width.pdf";
						Histo_Trigger_width_pdf->Print(name_pdf,"pdf");
					}
					Histo_Trigger_width_pdf->Clear();
					Histo_Trigger_width_pdf->Divide(4,2);
				}
			}
			
		}

		for (int i=0;i<128;i++)
		{
			Histo_Charge_ping_HG[i]->Write();
			Histo_Charge_pong_HG[i]->Write();
			Histo_Charge_ping_LG[i]->Write();
			Histo_Charge_pong_LG[i]->Write();
		}
		/*
		TCanvas* Histo_Trigger_falling_time_pdf = new TCanvas("Histo_Trigger_falling_time_pdf");
		Histo_Trigger_falling_time_pdf->Divide(4,2);
		
		for(int i=0;i<128; i++)
		{
			
			Histo_Trigger_falling_time_pdf->cd(compteur_plots+1);
			//gPad->SetLogz();
			//gPad->SetLogy();
			Histo_Trigger_falling_time[i]->Draw();
			compteur_plots++;
			
			if (compteur_plots == 8 || i == 127)
			{
				compteur_canvas++;
				compteur_plots = 0;
				
				if (compteur_canvas == 1)
				{
					name_pdf = OutputDirectory+token + "_Trigger_falling_time.pdf(";
					Histo_Trigger_falling_time_pdf->Print(name_pdf,"pdf");
				}
				else if (i==127)
				{
					name_pdf = OutputDirectory+token + "_Trigger_falling_time.pdf)";
					Histo_Trigger_falling_time_pdf->Print(name_pdf,"pdf");
				}
				else
				{
					name_pdf = OutputDirectory+token + "_Trigger_falling_time.pdf";
					Histo_Trigger_falling_time_pdf->Print(name_pdf,"pdf");
				}
				Histo_Trigger_falling_time_pdf->Clear();
				Histo_Trigger_falling_time_pdf->Divide(4,2);
			}
		}
		
		*/
		
		
		// TCanvas* Histo_Charge_trigger_width_pdf = new TCanvas("Histo_Charge_trigger_width_pdf");
		// Histo_Charge_trigger_width_pdf->Divide(2,2);
		// Histo_Charge_trigger_width_pdf->cd(1);
		// //gPad->SetLogy();
		// Histo_Trigger_width[8]->Draw();
		// Histo_Charge_trigger_width_pdf->cd(2);
		// graph_TFT[8]->Draw("AL");
		// Histo_Charge_trigger_width_pdf->cd(3);
		// Histo_Charge_ping_HG[8]->Draw();
		// Histo_Charge_trigger_width_pdf->cd(4);
		// graph_Coarse_time_ping[8]->Draw("AL");
		// string name = OutputDirectory+token + "_Charge_trigger_width.png";
		// Histo_Charge_trigger_width_pdf->SaveAs(name.c_str());
		
		
	/*

		TCanvas *c1 = new TCanvas("c","c1",1700,1500);  


		c1->Divide(2,2);
		c1->cd(1);

		TGraph* gr = new TGraph(128,x,mean_charge_ping);
		gr->SetTitle("");
		gr->GetYaxis()->SetTitle("Charge-Mean-PING");
		gr->GetXaxis()->SetTitle("channel_nb");
		gr->SetFillColor(38);
		gr->Draw("AB");	

		c1->cd(2);

		TGraph* gr1 = new TGraph(128,x,charge_RMS_ping);
		gr1->SetTitle("");
		gr1->GetYaxis()->SetTitle("Charge-RMS-PING");
		gr1->GetXaxis()->SetTitle("channel_nb");
		gr1->SetFillColor(38);
		gr1->Draw("AB");	


		c1->cd(3);

		TGraph* gr3 = new TGraph(128,x,mean_charge_pong);
		gr3->SetTitle("");
		gr3->GetYaxis()->SetTitle("Charge-Mean-PONG");
		gr3->GetXaxis()->SetTitle("channel_nb");
		gr3->SetFillColor(38);
		gr3->Draw("AB");	

		c1->cd(4);

		TGraph* gr4 = new TGraph(128,x,charge_RMS_pong);
		gr4->SetTitle("");
		gr4->GetYaxis()->SetTitle("Charge-RMS-PONG");
		gr4->GetXaxis()->SetTitle("channel_nb");
		gr4->SetFillColor(38);
		gr4->Draw("AB");	


		string name = OutputDirectory+"Mean_Sigma_plots_for_"+filename+".png";
		c1->SaveAs(name.c_str());
		
		*/
		
		cout << "Cat_data_counter : " <<  cat_data_counter << endl;
		cout << "DDS_data_counter : " <<  dds_data_counter << endl;

		OutTree->Write();

	}
	
	return 0;

} 

