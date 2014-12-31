



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

unsigned calcLocHistIdx(Addr &branch_addr);

 void squash(void *bp_history);
   
 void squash2(Addr &branch_addr);

void reset();



private:



unsigned branch_lower_order;

 void updateGlobalHistTaken();

 void updateGlobalHistNotTaken();

   struct BPHistory {
#ifdef DEBUG
        BPHistory()
        { newCount++; }
        ~BPHistory()
        { newCount--; }

        static int newCount;
#endif
        unsigned local_pred;
        unsigned global_pred;
    };

unsigned value[4][16];
unsigned local_pred;
unsigned global_pred;
unsigned PredSize;
unsigned PredCtrs;
unsigned instShiftAmt;
unsigned globalHistory_size;
unsigned globalHistory;
unsigned globalHistory_value;
unsigned localPredictorSets;
unsigned count;
unsigned InstShiftAmt;

file fl;

};

#endif // __CPU_PRED_corr_pred_PRED_HH__
