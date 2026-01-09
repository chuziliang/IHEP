#include "MyFunc.hh"
#include <iostream>
#include <sstream>
#include <fstream>
#include <TMath.h>

#include "Identifier/CdID.h"
#include "Identifier/Identifier.h"
#include "Identifier/IDService.h"
#include "Identifier/JunoDetectorID.h"
#include "Identifier/TtID.h"
#include "Identifier/WpID.h"

using namespace std;
unsigned int testHeaderID;

unsigned int headerDcode(char *charName, int len){
    
    testHeaderID = 0;
    int k = len;
    for (int i=0; i<len; i++) {
        k--;
        int shiftBit = 8*k;
        testHeaderID += ( (charName[i] & 0xFFu) << shiftBit );
    }
    return testHeaderID;
}

int main(){
    IDService* idServ = IDService::getIdServ();
    idServ -> init();
    idServ -> copyNo2Id(1);
	unsigned int  num;
	char A[4];
	A[0] = 0xCAu;
	A[1] = 0xFEu;
	A[2] = 0xCAu;
	A[3] = 0xFEu;
	num = headerDcode(A,4);
	std::cout<<num<<std::endl;
    cout<<TMath::Pi()<<endl;
}
