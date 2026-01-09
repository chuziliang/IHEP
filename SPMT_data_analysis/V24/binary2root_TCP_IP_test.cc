//g++ binary2root.cc -o bin2root `root-config --cflags --libs`

#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <TString.h>
#include <array>

using namespace std;

// Define the structures to hold the data

struct BlockData {
	int marker;
	struct {
		struct {
			int empty;
			int blockId;
			int BlockEventCounter;
			int BECTimeStamp;
		} header;
		struct {
			int eventType;
			int channelNumber;
			int fineTime;
			int coarseTime;
			int gain;
			int eventCounterPerChannel;
			int chargeValue;
			int coarseTimeOverflow;
		} event;
		struct {
			int empty;
			int blockId;
			int BlockEventCounter;
			int FirmwareVersion;
			int numChannels;
		} trailer;
	} data;
};


uint32_t getBlockId(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = static_cast<uint32_t>(data[4] & 0x0Fu);
	return result; 
}

uint32_t getBECTimeStamp(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[7] & 0xFFu) << 16)
					 +(static_cast<uint32_t>(data[8] & 0xFFu) << 8)
					 +(static_cast<uint32_t>(data[9] & 0xFFu));
	return result;
}

uint32_t getEventType(const std::array<char, 10>& data, BlockData &block) {
	unsigned char databyte = static_cast<unsigned char>(data[0]); // Skip the first 4 bytes header marker
	return static_cast<uint32_t>(databyte >> 7);
}

uint32_t getChannelNumber(const std::array<char, 10>& data, BlockData &block) {
	unsigned char databyte = static_cast<unsigned char>(data[0]); // Skip the first 4 bytes header marker
	uint32_t result = (static_cast<uint32_t>(data[0] & 0x70u) >> 4);
	// return static_cast<uint32_t>((databyte << block.data.event.eventType) >> (8 - block.data.event.channelNumber-block.data.event.eventType));
	return result;
}

uint32_t getCoarseTime(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[1] & 0x03u) << 24)
					 +(static_cast<uint32_t>(data[2] & 0xFFu) << 16)
					 +(static_cast<uint32_t>(data[3] & 0xFFu) << 8)
					 +(static_cast<uint32_t>(data[4] & 0xFFu));
	return result;
}

uint32_t getGain(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = static_cast<uint32_t>((data[5] & 0xFFu) >> 7);
	return result;
}

uint32_t getEventCounter(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[5] & 0x7Fu) << 8)
					 +(static_cast<uint32_t>(data[6] & 0xFCu));
	result = result >> 2;
	return result;
}

uint32_t getChargeValue(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[6] & 0x03u) << 8)
					 +(static_cast<uint32_t>(data[7] & 0xFFu));
	return result;
}

uint32_t getFineTime(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[0] & 0x0Fu) << 8)
					 +(static_cast<uint32_t>(data[1] & 0xFCu));
	result = result >> 2;
	return result;
}

uint32_t getBlockEventCounter(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[5] & 0xFFu) << 8)
					 +(static_cast<uint32_t>(data[6] & 0xFFu));
	return result;
}

uint32_t getNumberChannels(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[9] & 0xFFu) );
	return result;
}

uint32_t getOverflowCounter(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[8] & 0xFFu) << 8)
					 +(static_cast<uint32_t>(data[9] & 0xFFu));
	return result;
}

uint32_t getMarker(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[0] & 0xFFu) << 24)
					 +(static_cast<uint32_t>(data[1] & 0xFFu) << 16)
					 +(static_cast<uint32_t>(data[2] & 0xFFu) << 8)
					 +(static_cast<uint32_t>(data[3] & 0xFFu));
	return result;
}

uint32_t getFirmwareVersion(const std::array<char, 10>& data, BlockData &block) {
	uint32_t result = (static_cast<uint32_t>(data[7] & 0xFFu) << 8)
					 +(static_cast<uint32_t>(data[8] & 0xFFu));
	return result;
}

// uint64_t interRaw(char* data) {
// 	uint64_t d0 = static_cast<uint64_t>(data[0] & 0xFFu) << 56;
// 	uint64_t d1 = static_cast<uint64_t>(data[1] & 0xFFu) << 48;
// 	uint64_t d2 = static_cast<uint64_t>(data[2] & 0xFFu) << 40;
// 	uint64_t d3 = static_cast<uint64_t>(data[3] & 0xFFu) << 32;
// 	uint64_t d4 = static_cast<uint64_t>(data[4] & 0xFFu) << 24;
// 	uint64_t d5 = static_cast<uint64_t>(data[5] & 0xFFu) << 16;
// 	uint64_t d6 = static_cast<uint64_t>(data[6] & 0xFFu) << 8;
// 	uint64_t d7 = static_cast<uint64_t>(data[7] & 0xFFu);

// 	return d0 + d1 + d2 + d3
// 			  + d4 + d5 + d6 + d7;
// }

int main(int argc, char **argv) {
	if (argc != 3) {
		std::cerr << "Wrong number of input arguments" << std::endl;
		return 1;
	}
	std::string inDir(argv[1]);
	TString outDir(argv[2]);

	// Open the binary file for reading
	std::ifstream inputFile(inDir, std::ios::binary);
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open the input binary file." << std::endl;
		return 1;
	}

	// Create a ROOT file for writing
	TFile outputFile(outDir, "RECREATE");
	if (outputFile.IsZombie()) {
		std::cerr << "Failed to create the output ROOT file." << std::endl;
		return 1;
	}

	BlockData block;
	
	// Assign values
	block.marker = 32;
	
	// Block header values
	block.data.header.empty = 4;
	block.data.header.blockId = 4;
	block.data.header.BlockEventCounter = 16;
	block.data.header.BECTimeStamp = 24; // This value should be set to an appropriate value
	
	// Event values
	block.data.event.eventType = 1;
	block.data.event.channelNumber = 3;
	block.data.event.fineTime = 10;
	block.data.event.coarseTime = 26; // This value should be set to an appropriate value
	block.data.event.gain = 1;
	block.data.event.eventCounterPerChannel = 13; // This value should be set to an appropriate value
	block.data.event.chargeValue = 10;
	block.data.event.coarseTimeOverflow = 16;
	
	
	// Block trailer values
	block.data.trailer.empty = 4;
	block.data.trailer.blockId = 4;
	block.data.trailer.BlockEventCounter = 16;
	block.data.trailer.FirmwareVersion = 16;
	block.data.trailer.numChannels = 8;

	// Create a TTree to hold the data
	TTree tree("EventData", "Data Tree");
	TTree trailTree("BlockTrailer", "Trailer Tree");

	// Declare variables to hold the data
	uint32_t marker;
	uint32_t blockId;
	uint32_t emptyHeader;
	uint32_t BECTimeStamp;
	uint32_t eventType;
	uint32_t channelNumber;
	uint32_t blockChannelNumber;
	uint32_t coarseTime;
	uint32_t gain;
	uint32_t eventCounterPerChannel;
	uint32_t chargeValue;
	uint32_t fineTime;
	uint32_t numChannels;
	uint32_t emptyTrailer;
	uint32_t coarseTimeOverflow;
	uint32_t eventCounterPerCh;
	uint32_t dataPackId;
	uint32_t sign;
	uint32_t FirmwareVersion;
	uint32_t BlockEventCounter;

	dataPackId = 0;

	// char *rawGroup = new char[10];
	std::array<char, 10> rawGroup;
	// Attach branches to the TTree
	tree.Branch("BlockId", &blockId, "blockId/i");
	tree.Branch("DataPackId", &dataPackId, "dataPackId/i");
	tree.Branch("BECTimeStamp", &BECTimeStamp, "BECTimeStamp/i");
	tree.Branch("EventType", &eventType, "eventType/i");
	tree.Branch("ChannelNumber", &blockChannelNumber, "blockChannelNumber/i");
	tree.Branch("ABCChannelNumber", &channelNumber, "channelNumber/i");
	tree.Branch("CoarseTime", &coarseTime, "coarseTime/i");
	tree.Branch("CoarseTimeOverflow", &coarseTimeOverflow, "coarseTimeOverflow/i");
	tree.Branch("Gain", &gain, "gain/i");
	tree.Branch("Sign", &sign, "sign/i");
	tree.Branch("EventCounterPerChannel", &eventCounterPerCh, "eventCounterPerCh/i");
	tree.Branch("EventCounterPerChannelUser", &eventCounterPerChannel, "eventCounterPerChannel/i");
	tree.Branch("Charge", &chargeValue, "chargeValue/i");
	tree.Branch("FineTime", &fineTime, "fineTime/i");

	int nChs = 0;
	trailTree.Branch("NumberChannels", &numChannels, "numChannels/i");
	trailTree.Branch("NumberChannelsUser", &nChs, "nChs/i");
	trailTree.Branch("FirmwareFersion", &FirmwareVersion, "FirmwareVersion/i");
	trailTree.Branch("BECTimeStamp", &BECTimeStamp, "BECTimeStamp/i");
	trailTree.Branch("BlockEventCounter", &BlockEventCounter, "BlockEventCounter/i");
	
	// tree.Branch("CoarseTimeOverflow", &coarseTimeOverflow, "coarseTimeOverflow/i");

	int evtCounter[128] = {0};
	// Loop through the binary file and read data
	std::vector<char> buffer(4); // 4 bytes buffer
	while (inputFile.read(buffer.data(), buffer.size())) 
	{
		if (*reinterpret_cast<int*>(buffer.data()) == 0xBB3412BB) 
		{
			std::cout << "Found the pattern 0xBB1234BB" << std::endl;
			// 移动文件指针前移4个字节
			inputFile.seekg(-4, std::ios::cur);
			break;
		}
	}

	bool isHeader;


	while (inputFile.read(rawGroup.data(), rawGroup.size())) {
		// group = interRaw(rawGroup);
		marker = getMarker(rawGroup, block);
		// cout<< marker << endl;

		if (marker == 0xBB1234BB) { // Block header
			
			// cout << "Size of blockId: " << block.data.header.blockId << endl;
			blockId = getBlockId(rawGroup, block);
			BECTimeStamp = getBECTimeStamp(rawGroup, block);
			BlockEventCounter = getBlockEventCounter(rawGroup, block);
			if (isHeader)
			{
				cout<<"Double Header"<<endl;
				cout<<"dataPackId: "<<dataPackId<<endl;
			}
			
			isHeader = true;
		} else if (marker == 0xBB4321BB) { // Block trailer
			numChannels = getNumberChannels(rawGroup, block);
			if(getBlockEventCounter(rawGroup, block) != BlockEventCounter) cout<< "Wrong Block Event Counter" << endl;
			if(getBlockId(rawGroup, block)!= blockId) cout<< "Wrong blockId" << endl;
			FirmwareVersion = getFirmwareVersion(rawGroup, block);
			dataPackId ++;
			trailTree.Fill();
			nChs = 0;
			if (!isHeader)
			{
				cout<<"Double Trailer"<<endl;
				cout<<"dataPackId: "<<dataPackId<<endl;
			}
			
			isHeader = false;
		} else { // Event channel data
			nChs ++;
			eventType = getEventType(rawGroup, block);
			blockChannelNumber = getChannelNumber(rawGroup, block);
			channelNumber = blockId * 8 + blockChannelNumber;
			coarseTime = getCoarseTime(rawGroup, block);
			gain = getGain(rawGroup, block); //1 = HG, 0 = LG
			eventCounterPerCh = getEventCounter(rawGroup, block);
			eventCounterPerChannel = evtCounter[channelNumber];
			sign = eventCounterPerCh % 2;
			evtCounter[channelNumber] ++;
			chargeValue = getChargeValue(rawGroup, block);
			fineTime = getFineTime(rawGroup, block);
			coarseTimeOverflow = getOverflowCounter(rawGroup, block);
			tree.Fill();
		}
	}

	// Write the TTree to the ROOT file
	tree.Write();
	trailTree.Write();

	// Close the ROOT file
	outputFile.Close();

	// Close the input binary file
	inputFile.close();

	std::cout << "Data has been successfully converted to a ROOT file." << std::endl;

	return 0;
}
