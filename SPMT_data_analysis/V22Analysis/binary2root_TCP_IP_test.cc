//g++ binary2root.cc -o bin2root `root-config --cflags --libs`

#include <iostream>
#include <fstream>
#include <vector>
#include <TFile.h>
#include <TTree.h>
#include <string>
#include <TString.h>

using namespace std;

// Define the structures to hold the data

struct BlockData {
	int marker;
	struct {
		struct {
			int blockId;
			int empty;
			int abcTimestamp;
		} header;
		struct {
			int eventType;
			int channelNumber;
			int coarseTime;
			int gain;
			int eventCounter;
			int chargeValue;
			int fineTime;
		} event;
		struct {
			int numChannels;
			int empty;
			int coarseTimeOverflow;
		} trailer;
	} data;
};


uint32_t getBlockId(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << block.marker) >> (64 - block.data.header.blockId));
}

uint32_t getABCTimeStamp(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (64 - block.data.header.abcTimestamp)) >> (64 - block.data.header.abcTimestamp));
}

uint32_t getEventType(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>(data >> (64 - block.data.event.eventType));
}

uint32_t getChannelNumber(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << block.data.event.eventType) >> (64 - block.data.event.channelNumber));
}

uint32_t getCoarseTime(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (block.data.event.eventType + block.data.event.channelNumber)) >> (64 - block.data.event.coarseTime));
}

uint32_t getGain(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (block.data.event.eventType + block.data.event.channelNumber + block.data.event.coarseTime)) >> (64 - block.data.event.gain));
}

uint32_t getEventCounter(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (64 - block.data.event.eventCounter - block.data.event.chargeValue - block.data.event.fineTime)) >> (64 - block.data.event.eventCounter));
}

uint32_t getChargeValue(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (64 - block.data.event.chargeValue - block.data.event.fineTime)) >> (64 - block.data.event.chargeValue));
}

uint32_t getFineTime(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (64 - block.data.event.fineTime)) >> (64 - block.data.event.fineTime));
}

uint32_t getNumberChannels(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (64 - block.marker)) >> (64 - block.data.trailer.numChannels));
}

uint32_t getOverflowCounter(uint64_t data, BlockData &block) {
	return static_cast<uint32_t>((data << (64 - block.data.trailer.coarseTimeOverflow)) >> (64 - block.data.trailer.coarseTimeOverflow));
}

uint64_t interRaw(char* data) {
	uint64_t d0 = static_cast<uint64_t>(data[0] & 0xFFu) << 56;
	uint64_t d1 = static_cast<uint64_t>(data[1] & 0xFFu) << 48;
	uint64_t d2 = static_cast<uint64_t>(data[2] & 0xFFu) << 40;
	uint64_t d3 = static_cast<uint64_t>(data[3] & 0xFFu) << 32;
	uint64_t d4 = static_cast<uint64_t>(data[4] & 0xFFu) << 24;
	uint64_t d5 = static_cast<uint64_t>(data[5] & 0xFFu) << 16;
	uint64_t d6 = static_cast<uint64_t>(data[6] & 0xFFu) << 8;
	uint64_t d7 = static_cast<uint64_t>(data[7] & 0xFFu);

	return d0 + d1 + d2 + d3
			  + d4 + d5 + d6 + d7;
}

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
	block.data.header.blockId = 4;
	block.data.header.empty = 2;
	block.data.header.abcTimestamp = 26; // This value should be set to an appropriate value
	
	// Event values
	block.data.event.eventType = 2;
	block.data.event.channelNumber = 4;
	block.data.event.coarseTime = 26; // This value should be set to an appropriate value
	block.data.event.gain = 1;
	block.data.event.eventCounter = 11; // This value should be set to an appropriate value
	block.data.event.chargeValue = 10;
	block.data.event.fineTime = 10;
	
	// Block trailer values
	block.data.trailer.numChannels = 8;
	block.data.trailer.empty = 8;
	block.data.trailer.coarseTimeOverflow = 16;

	block.data.header.blockId = 4;

	// Create a TTree to hold the data
	TTree tree("EventData", "Data Tree");
	TTree trailTree("BlockTrailer", "Trailer Tree");

	// Declare variables to hold the data
	uint32_t marker;
	uint32_t blockId;
	uint32_t emptyHeader;
	uint32_t abcTimestamp;
	uint32_t eventType;
	uint32_t channelNumber;
	uint32_t blockChannelNumber;
	uint32_t coarseTime;
	uint32_t gain;
	uint32_t eventCounter;
	uint32_t chargeValue;
	uint32_t fineTime;
	uint32_t numChannels;
	uint32_t emptyTrailer;
	uint32_t coarseTimeOverflow;
	uint32_t eventCounterPerCh;
	uint32_t dataPackId;
	uint32_t sign;

	dataPackId = 0;

	char *rawGroup = new char[8];
	uint64_t group;
	// Attach branches to the TTree
	tree.Branch("BlockId", &blockId, "blockId/i");
	tree.Branch("DataPackId", &dataPackId, "dataPackId/i");
	tree.Branch("ABCTimestamp", &abcTimestamp, "abcTimestamp/i");
	tree.Branch("EventType", &eventType, "eventType/i");
	tree.Branch("ChannelNumber", &blockChannelNumber, "blockChannelNumber/i");
	tree.Branch("ABCChannelNumber", &channelNumber, "channelNumber/i");
	tree.Branch("CoarseTime", &coarseTime, "coarseTime/i");
	tree.Branch("Gain", &gain, "gain/i");
	tree.Branch("Sign", &sign, "sign/i");
	tree.Branch("EventCounterPerChannel", &eventCounterPerCh, "eventCounterPerCh/i");
	tree.Branch("EventCounterPerChannelUser", &eventCounter, "eventCounter/i");
	tree.Branch("Charge", &chargeValue, "chargeValue/i");
	tree.Branch("FineTime", &fineTime, "fineTime/i");

	int nChs = 0;
	trailTree.Branch("NumberChannels", &numChannels, "numChannels/i");
	trailTree.Branch("NumberChannelsUser", &nChs, "nChs/i");
	trailTree.Branch("CoarseTimeOverflow", &coarseTimeOverflow, "coarseTimeOverflow/i");
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

	while (inputFile.read(rawGroup, sizeof(group))) {
		group = interRaw(rawGroup);
		marker = static_cast<uint32_t>(group >> 32);

		if (marker == 0xBB1234BB) { // Block header
			dataPackId ++;
			// cout << "Size of blockId: " << block.data.header.blockId << endl;
			blockId = getBlockId(group, block);
			abcTimestamp = getABCTimeStamp(group, block);
		} else if (marker == 0xBB4321BB) { // Block trailer
			numChannels = getNumberChannels(group, block);
			coarseTimeOverflow = getOverflowCounter(group, block);
			trailTree.Fill();
			nChs = 0;
		} else { // Event channel data
			nChs ++;
			eventType = getEventType(group, block);
			blockChannelNumber = getChannelNumber(group, block);
			channelNumber = blockId * 8 + blockChannelNumber;
			coarseTime = getCoarseTime(group, block);
			gain = getGain(group, block); //1 = HG, 0 = LG
			eventCounterPerCh = getEventCounter(group, block);
			eventCounter = evtCounter[channelNumber];
			sign = eventCounterPerCh % 2;
			evtCounter[channelNumber] ++;
			chargeValue = getChargeValue(group, block);
			fineTime = getFineTime(group, block);
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
