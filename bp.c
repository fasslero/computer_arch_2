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
