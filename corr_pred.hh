



#ifndef __CPU_PRED_CORR_PRED_PRED_HH__
#define __CPU_PRED_CORR_PRED_PRED_HH__

#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "cpu/pred/sat_counter.hh"
#include "cpu/pred/file.hh"
using namespace std;

class corr_predBP : public BPredUnit
{

public:

corr_predBP(const Params *params );    // constructor

~corr_predBP(void);

void uncondBranch(void * &bp_history,Addr branch_addr);


bool lookup(Addr branch_addr, void * &bp_history);/* looks up the branch address in the bp_history table*/

void btbUpdate(Addr branch_addr, void * &bp_history);

void update(Addr branch_addr, bool taken, void *bp_history, bool squashed); /* updates the branch address as taken , not taken or squashed in the bp_history table*/


 void squash(void *bp_history);
   


void reset();



private:


 void updateGlobalHistTaken();

 void updateGlobalHistNotTaken();

    /** Number of bits for each entry of the local history table. */
   // unsigned localHistoryBits_corr;

    
    /** Number of entries in the local history table. */
    //unsigned localHistoryTableSize_corr;
/*struct BPHistory {
#ifdef DEBUG
        BPHistory()
        { newCount++; }
        ~BPHistory()
        { newCount--; }

        static int newCount;
#endif
        unsigned globalHistory;
        
        
    };
*/

/** Array of local history table entries. */
//std::vector<SatCounter> localCtrs;
//  vector < vector<SatCounter>> localHistoryTable;
// vector < localCtrs> localHistoryTable;


int value[16][4];


// BPHistory *history; 
   
unsigned PredSize;
unsigned PredCtrs;
unsigned instShiftAmt;
unsigned globalHistory;
unsigned flag;  

uint8_t c;
unsigned localPredictorSets;

uint8_t count;
unsigned branch_lower_order;

char msg[1000];

file fl;
};

#endif // __CPU_PRED_corr_pred_PRED_HH__
