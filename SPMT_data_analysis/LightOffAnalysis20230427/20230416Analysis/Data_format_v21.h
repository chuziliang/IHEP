
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

//#include "Event/SimHeader.h"

using namespace std;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool isHeader(char *data)
{
	unsigned int Marker_A;
	unsigned int Marker_B;
	unsigned int Marker_C;
	unsigned int Marker_D;
	unsigned int Marker;
	
	Marker_A = data[0] & 0xFFu;
	Marker_A = Marker_A << 24;
   	Marker_B = data[1] & 0xFFu;
	Marker_B = Marker_B << 16;
	Marker_C = data[2] & 0xFFu;
	Marker_C = Marker_C << 8;
	Marker_D = data[3] & 0xFFu;
	Marker = Marker_A + Marker_B + Marker_C + Marker_D;
	
	if (Marker == 3138532539) //Header
		{
		return true;
		}
	else
		{
		return false;
		}
}

unsigned int get_Block_ID(char *data)
{
	
	unsigned int Block_ID;
	Block_ID = data[0] & 0xF0u;
	Block_ID = Block_ID >> 4 ;
	
	return Block_ID;
}

unsigned int get_Local_Time_Stamp(char *data)
{
    // Local Time stamp on 28 bits
	unsigned int Time_Stamp_A;
	unsigned int Time_Stamp_B;
	unsigned int Time_Stamp_C;
	unsigned int Time_Stamp_D;
	unsigned int Time_Stamp;
	
	Time_Stamp_A = data[0] & 0x0Fu;
	Time_Stamp_A = Time_Stamp_A << 24;
	Time_Stamp_B = data[1] & 0xFFu;
	Time_Stamp_B = Time_Stamp_B << 16; 
	Time_Stamp_C = data[2] & 0xFFu;
	Time_Stamp_C = Time_Stamp_C << 8;
	Time_Stamp_D = data[3] & 0xFFu;
	Time_Stamp = Time_Stamp_A + Time_Stamp_B + Time_Stamp_C + Time_Stamp_D;	
				
	return Time_Stamp;
}

bool isTrailer(char *data)
{
	unsigned int Marker_A;
	unsigned int Marker_B;
	unsigned int Marker_C;
	unsigned int Marker_D;
	unsigned int Marker;
	
	Marker_A = data[0] & 0xFFu;
	Marker_A = Marker_A << 24;
   	Marker_B = data[1] & 0xFFu;
	Marker_B = Marker_B << 16;
	Marker_C = data[2] & 0xFFu;
	Marker_C = Marker_C << 8;
	Marker_D = data[3] & 0xFFu;
	Marker = Marker_A + Marker_B + Marker_C + Marker_D;
	
	if (Marker == 3141738939) //Trailer
		{
		return true;
		}
	else
		{
		return false;
		}
}

unsigned int get_NbChannels_Readed(char *data)
{
	
	unsigned int Total_Length_B;
	
	Total_Length_B = data[0] & 0xFFu;
	
	return Total_Length_B;
}




unsigned int get_CT_0verflow(char *data)
{
	
	unsigned int CT_Overflow_Counter_C;
	unsigned int CT_Overflow_Counter_D;
	unsigned int CT_Overflow_Counter;
	CT_Overflow_Counter_C = data[2] & 0xFFu;
	CT_Overflow_Counter_C = CT_Overflow_Counter_C << 8;
	CT_Overflow_Counter_D = data[3] & 0xFFu; 
	CT_Overflow_Counter = CT_Overflow_Counter_C + CT_Overflow_Counter_D;
	
	return CT_Overflow_Counter;
}

unsigned int get_Evt_Type(char *data)
{
	unsigned int evt_type;
	evt_type = data[0] & 0xC0;
	evt_type = evt_type >> 6;
	return evt_type;
}		
unsigned int get_cat_channel_nb(char *data)
{
	unsigned int channel_nb;
	channel_nb = data[0] >> 2 & 0x0Fu;	
	
	return channel_nb;
}	
unsigned int get_coarse_time(char *data)
{
	unsigned int Coarse_time_A;
	unsigned int Coarse_time_B;
	unsigned int Coarse_time_C;
	unsigned int Coarse_time_D;
	unsigned int Coarse_time;
	Coarse_time_A = data[0] & 0x03u;
	Coarse_time_A = Coarse_time_A << 24;
	Coarse_time_B = data[1] & 0xFFu;
	Coarse_time_B = Coarse_time_B << 16;
	Coarse_time_C = data[2] & 0xFFu;
	Coarse_time_C = Coarse_time_C << 8;
	Coarse_time_D = data[3] & 0xFFu;
				
	Coarse_time = Coarse_time_A + Coarse_time_B + Coarse_time_C + Coarse_time_D;
	
	return Coarse_time;
}
unsigned int get_gain(char *data)
{
	
	unsigned int Gain;
	Gain = data[0] & 0x80u;
	Gain = Gain >> 7;
	
	return Gain;
}
unsigned int get_Evt_counter_per_channel(char *data)
{
	unsigned int Evt_counter_per_channel=0;
	unsigned int Evt_counter_per_channel_A;
	unsigned int Evt_counter_per_channel_B;
	unsigned int Evt_counter_per_channel_C;
	unsigned int Evt_counter_per_channel_D;
	Evt_counter_per_channel_A = data[0] & 0x7Fu;
	Evt_counter_per_channel_A = Evt_counter_per_channel_A << 4;
	Evt_counter_per_channel_B = data[1] & 0xF0u;
	Evt_counter_per_channel_B = Evt_counter_per_channel_B >> 4;
	Evt_counter_per_channel = Evt_counter_per_channel_A + Evt_counter_per_channel_B;
	return Evt_counter_per_channel;
}

unsigned int get_Charge(char *data)
{
	int Charge_A;
	int Charge_B;
	int Charge;
	Charge_A = data[1] & 0x0Fu;
	Charge_A = Charge_A << 6;					
	Charge_B = data[2] & 0xFCu;
	Charge_B = Charge_B >> 2;
	Charge = Charge_A + Charge_B;
	return Charge;	
}
unsigned int get_Fine_time(char *data)
{
	unsigned int Fine_time_A;
	unsigned int Fine_time_B;
	unsigned int Fine_time;
	Fine_time_A = data[2] & 0x03u;
	Fine_time_A = Fine_time_A << 8;
	Fine_time_B = data[3] & 0xFFu;
	Fine_time = Fine_time_A + Fine_time_B;	
	return Fine_time;			
}

/// Fonctions pour les données type DDS

unsigned int get_dds_channel_nb(char *data)
{
	unsigned int channel_nb;
	//channel_nb = (((data[0] & 0x1Fu) << 8) +  (data[1] & 0xC0u)) >> 6;	
	channel_nb = ((data[0] & 0x1Eu) >> 1) ;	
	
	return channel_nb;
}


unsigned int get_discri_Evt_counter_per_channel(char *data)
{
	// Event Counter on 18 bits
	unsigned int Evt_counter_per_channel_discri_A;
	unsigned int Evt_counter_per_channel_discri_B;
	unsigned int Evt_counter_per_channel_discri_C;
	unsigned int Evt_counter_per_channel_discri_D;
	unsigned int Evt_counter_per_channel_discri;
	
	Evt_counter_per_channel_discri_A = data[0] & 0x01u;
	Evt_counter_per_channel_discri_A = Evt_counter_per_channel_discri_A << 17;
	Evt_counter_per_channel_discri_B = data[1] & 0xFFu;
	Evt_counter_per_channel_discri_B = Evt_counter_per_channel_discri_B << 9;
	Evt_counter_per_channel_discri_C = data[2] & 0xFFu;
	Evt_counter_per_channel_discri_C = Evt_counter_per_channel_discri_C <<  1;
	Evt_counter_per_channel_discri_D = data[3] & 0x80u;
	Evt_counter_per_channel_discri_D = Evt_counter_per_channel_discri_D >>  7;
	
	
	Evt_counter_per_channel_discri = Evt_counter_per_channel_discri_A + Evt_counter_per_channel_discri_B + Evt_counter_per_channel_discri_C + Evt_counter_per_channel_discri_D ;
	return Evt_counter_per_channel_discri;
}

unsigned int get_Trigger_width_time(char *data)
{
	// Trigger width Time  on 7 bits
	unsigned int Trigger_width_time;
	
	Trigger_width_time = data[3] & 0x7Fu;
	
	return Trigger_width_time;
}


unsigned int get_Trigger_falling_time(char *data)
{
	// Trigger fine falling Time  on 26 bits
	unsigned int Trigger_falling_time_A;
	unsigned int Trigger_falling_time_B;
	unsigned int Trigger_falling_time_C;
	unsigned int Trigger_falling_time_D;
	unsigned int Trigger_falling_time;
	Trigger_falling_time_A = data[0] & 0xFFu;
	Trigger_falling_time_A = Trigger_falling_time_A << 24;
	Trigger_falling_time_B = data[1] & 0xFFu;
	Trigger_falling_time_B = Trigger_falling_time_B << 16 ;
	Trigger_falling_time_C = data[2] & 0xFFu;
	Trigger_falling_time_C = Trigger_falling_time_C << 8 ;
	Trigger_falling_time_D = data[3] & 0xC0u;
	Trigger_falling_time_D = Trigger_falling_time_D  ;
	Trigger_falling_time = Trigger_falling_time_A + Trigger_falling_time_B + Trigger_falling_time_C + Trigger_falling_time_D ;
	Trigger_falling_time = Trigger_falling_time >> 6;
	return Trigger_falling_time;
}

unsigned int get_discri_fine_time(char *data)
{
	// Trigger fine Time  on 6 bits
	unsigned int Discri_fine_time;
	
	Discri_fine_time = data[3] & 0x3Fu;
	
	return Discri_fine_time;
}



//End of main, and do of vertex_reconstruction
