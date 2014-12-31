


#ifndef __CPU_PRED_CORR_PRED_HH__
#define __CPU_PRED_CORR_PRED_HH__

#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "cpu/pred/sat_counter.hh"
#include "cpu/pred/file.hh"

using namespace std;

class corrBP : public BPredUnit
{

public:

corrBP(const Params *params );    // constructor

~corrBP(void);

unsigned calcLocHistIdx(Addr &branch_addr);

void uncondBranch(void * &bp_history,Addr branch_addr);


bool lookup(Addr branch_addr, void * &bp_history);/* looks up the branch address in the bp_history table*/

void btbUpdate(Addr branch_addr, void * &bp_history);

void update(Addr branch_addr, bool taken, void *bp_history, bool squashed); /* updates the branch address as taken , not taken or squashed in the bp_history table*/


 void squash(void *bp_history);
   
 void squash2(Addr &branch_addr);

void reset();



private:


 void updateGlobalHistTaken(unsigned branch_lower_order);

 void updateGlobalHistNotTaken(unsigned branch_lower_order);
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
     
unsigned historytable[16];
unsigned countertable[16];
unsigned local_pred;
unsigned global_pred;
unsigned InstShiftAmt;
unsigned index;
unsigned count;
unsigned branch_lower_order;

char msg[1000];

file fl;
};

#endif // __CPU_PRED_CORR_PRED_HH__
