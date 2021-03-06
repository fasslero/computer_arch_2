/* 046267 Computer Architecture - Spring 2016 - HW #2 */
/* This file should hold your implementation of the predictor simulator */

#include "bp_api.h"
#define HIGH_BIT 29

typedef enum  {SNT=0, WNT, WT, ST} Prediction;

typedef struct  {

	int32_t target;
	int32_t tag;
	int8_t history;
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
    int8_t globalHistory;
    Prediction globalPrediction;
    bool usingShareLsb;
    bool usingShareMid;
    int8_t historyMask;


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
    MyBP.globalHistory = 0;
    MyBP.globalPrediction = WNT;

    MyBP.usingShareLsb = (Shared == 1) ? 1 : 0;
    MyBP.usingShareMid = (Shared == 2) ? 1 : 0;

    MyBP.historyMask = 0x11111111;

	MyBP.BTB = malloc(sizeof(TableLine)*MyBP.btbsize);
	if(MyBP.BTB == NULL)
		return -1;

    // todo - separate to local or global history and global or local prediction state machine

	for (int i = 0; i < MyBP.btbsize; i++) {
		MyBP.BTB[i].pred = WNT;
		MyBP.BTB[i].history = 0;
		MyBP.BTB[i].tag = 0;
		MyBP.BTB[i].target = 0;
	}

	//init sim stat
	MyBP.stats.br_num = 0;
	MyBP.stats.flush_num = 0;
	MyBP.stats.size = 0; //TODO calc

    //set the history mask according to the history size (zeros all the bit left to the history size)
    for (i=0; i<(8-MyBP.historySize); i++)
        MyBP.historyMask >> 1;
    MyBP.historyMask = MyBP.historyMask - 1;
	return 0;

}


bool BP_predict(uint32_t pc, uint32_t *dst){
	TableLine btbLine;
	uint32_t defaultDstAddress = pc + 4;

	//get the BTB line
    btbLine = *getBtbLine(pc);


	//check whether the line is the desired one, if not set dst to default dst
    if (btbLine.tag != pc){
        *dst = defaultDstAddress;
        return false;}
	//if the address is there, check the Bimodal and return dst according
        // todo - add global state machine option
    else{
        if(btbLine.pred==ST || btbLine.pred==WT){
            *dst = btbLine.target;
            return true;
        }
    }
    //predict WNT or SNT
    *dst = defaultDstAddress;
    return false;
}


void BP_update(uint32_t pc, uint32_t targetPc, bool taken, uint32_t pred_dst){
    pTableLine btbLine;


    //get the BTB line
    btbLine = getBtbLine(pc);
	
	//update the stats
	MyBP.stats.br_num++;
	if ((btbLine->pred == ST || btbLine->pred == WT) && !taken)  //predicted T and was wrong
		MyBP.stats.flush_num++;
	else if ((btbLine->pred == SNT || btbLine->pred == WNT) && taken) //predicted NT and was wrong
		MyBP.stats.flush_num++;
	

	///////
    /// update the btb line according to the parameters and local/global history
	///////

	//get the local or global history and update the history
	if (MyBP.isGlobalHist) {
		updateHistory(MyBP.globalHistory,taken);
	}
    else {
		updateHistory(btbLine->history,taken);
	}


	// update the prediction
	if (MyBP.isGlobalTable)
		updatePrediction(MyBP.globalPrediction, taken);
	else
		updatePrediction(MyBP.BTB->pred, taken);

	//update the predicted dest
	//btbLine->pred = pred_dst;	already updated the prediction above^
	btbLine->tag = getNumber(pc, HIGH_BIT - MyBP.tagSize, HIGH_BIT); 
	return;
}


void BP_GetStats(SIM_stats *curStats) {
	return MyBP.stats;
}


/*!
 * updates the prediction according to the given taken state
 */
void updatePrediction(Prediction *prediction, bool taken){
	if (taken && prediction != ST)
		prediction += 1;
	else if (!taken && prediction !=SNT)
		prediction -= 1;
}

/*!
 * updates the history according to the given taken state
 */
void updateHistory(int32_t *history, bool taken){
	//todo - check that pointers are right

	*history <<= 1;
    if (taken)
        *history |= 0x01;
	    *history &= (MyBP.historyMask && taken);
}

/*!
 * returns the pointer btb line according to the pc provided
 */
pTableLine getBtbLine(uint32_t pc){
    //todo - what about shared history? neet to add support for usingShareLsb and usingShareMid

	uint32_t num = getNumber(pc, HIGH_BIT - MyBP.btbsize, HIGH_BIT);
	return (MyBP.BTB + num);
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
