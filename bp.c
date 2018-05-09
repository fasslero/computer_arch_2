/* 046267 Computer Architecture - Spring 2016 - HW #2 */
/* This file should hold your implementation of the predictor simulator */

#include "bp_api.h"

typedef enum  {WNT, NT, WT, T} Prediction;

typedef struct  {

	int32_t target;
	int32_t tag;
	int32_t history;
	Prediction pred;

} TableLine;


/*struct of Branch predictor*/
typedef struct {
	SIM_stats stats;
	unsigned btbsize;
	unsigned historySize;
	unsigned tagSize;
	bool isGlobalHist;
	bool isGlobalTable;
	int shared;
	TableLine *BTB;

} BP;

/*Declaration of our BP*/
BP MyBP;

int BP_init(unsigned btbSize, unsigned historySize, unsigned tagSize,
             bool isGlobalHist, bool isGlobalTable, int Shared){

	MyBP.btbsize = btbSize;
	MyBP.tagSize = tagSize;
	MyBP.historySize = historySize;
	MyBP.isGlobalHist = isGlobalHist;
	MyBP.isGlobalTable = isGlobalTable;
	MyBP.shared = Shared;

	MyBP.BTB = malloc(sizeof(TableLine)*btbSize);

	if(MyBP.BTB == NULL)
		return -1;

	for (int i = 0; i < btbSize; i++) {
		MyBP.BTB[i].pred = WNT;
		MyBP.BTB[i].history = 0;
		MyBP.BTB[i].tag = 0;
		MyBP.BTB[i].target = 0;
	}

	return 0;

}

bool BP_predict(uint32_t pc, uint32_t *dst){
	return false;
}

void BP_update(uint32_t pc, uint32_t targetPc, bool taken, uint32_t pred_dst){
	return;
}

void BP_GetStats(SIM_stats *curStats) {
	return;
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