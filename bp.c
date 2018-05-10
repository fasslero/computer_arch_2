/* 046267 Computer Architecture - Spring 2016 - HW #2 */
/* This file should hold your implementation of the predictor simulator */

#include "bp_api.h"

typedef enum  {SNT=0, WNT, WT, ST} Prediction;

typedef struct  {

	int32_t target;
	int32_t tag;
	int32_t history;
	Prediction pred;

} TableLine, *pTableLine;


/*struct of Branch predictor*/
typedef struct {
	SIM_stats stats;
	unsigned btbsize;
	unsigned historySize;
	unsigned tagSize;
	bool isGlobalHist;
	bool isGlobalTable;
    int32_t globalHistory;
    Prediction globalPrediction;

    int shared;
	TableLine *BTB;

} BP;

/*Declaration of our BP*/
BP MyBP;

int BP_init(unsigned btbSize, unsigned historySize, unsigned tagSize,
             bool isGlobalHist, bool isGlobalTable, int Shared){

	if (btbSize > 1)
		MyBP.btbsize = log2(btbSize);
	else MyBP.btbsize = 1;
	
	MyBP.tagSize = tagSize;
	MyBP.historySize = historySize;
	MyBP.isGlobalHist = isGlobalHist;
	MyBP.isGlobalTable = isGlobalTable;
	MyBP.shared = Shared;

	
	MyBP.BTB = malloc(sizeof(TableLine)*MyBP.btbsize);
	if(MyBP.BTB == NULL)
		return -1;

	for (int i = 0; i < MyBP.btbsize; i++) {
		MyBP.BTB[i].pred = WNT;
		MyBP.BTB[i].history = 0;
		MyBP.BTB[i].tag = 0;
		MyBP.BTB[i].target = 0;
	}

	return 0;


	//init sim stat
	MyBP.stats.br_num = 0;
	MyBP.stats.flush_num = 0;
	MyBP.stats.size = 0; //TODO calc

	return 0;

}


bool BP_predict(uint32_t pc, uint32_t *dst){
	TableLine btbLine;
	uint32_t defaultDstAddress = pc + 4;

	//get the BTB line
    btbLine = getBtbLine(pc);
    //todo

	//check whether the line is the desired one, if not set dst to default dst
    if (btbLine.tag != pc){
        *dst = defaultDstAddress;
        return false;}
	//if the address is there, check the Bimodal and return dst according
    else{
        if(btbLine.pred==ST || btbLine.pred==WT){
            *dst = btbLine.target;
            return true;
        }
    }
    *dst = defaultDstAddress;
    return false;
}


void BP_update(uint32_t pc, uint32_t targetPc, bool taken, uint32_t pred_dst){
    pTableLine btbLine;
    int32_t history;

    //get the BTB line
    //todo
    //update the btb line according to the parameters and local/global history

    //get the local or global history and assign to history var
    if (MyBP.isGlobalHist)
        history = MyBP.globalHistory;
    else
        history = btbLine->history;


	return;
}


void BP_GetStats(SIM_stats *curStats) {
	return MyBP.stats;
}

/*!
 * returns the pointer btb line according to the pc provided
 */
pTableLine getBtbLine(uint32_t pc){

}

/*!
 * createBitMask - Creates a bit mask from the bit indexes
 * param[in] start - The starting index
 * param[in] end - The ending index
 */
uint32_t createBitMask(uint32_t start, uint32_t end) {
    return 0xFFFFFFFF >> start  << start  << (31 - end) >> (31 - end);
}

/*!
* getNumber - Returns an unsigned sub-number of an address
* param[in] address - The number from which to extract
* param[in] start - The bit number from which to start (0 to 31)
* param[in] end - The bit number from which to end (0 to 31)
*/
uint32_t getNumber(uint32_t address, uint32_t start, uint32_t end) {
    // The bit mask of the start & end marks
    uint32_t mask = createBitMask(start, end);
    return (address & mask) >> start;
}
