
#ifndef __CPU_PRED_gselect_PRED_HH__
#define __CPU_PRED_gselect_PRED_HH__

#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include "base/types.hh"
#include "cpu/pred/bpred_unit.hh"
#include "cpu/pred/sat_counter.hh"
#include "cpu/pred/file.hh"
using namespace std;


class gselectBP : public BPredUnit
{

public:

gselectBP(const Params *params );    // constructor

~gselectBP(void);

unsigned calcLocHistIdx(Addr &branch_addr);

void uncondBranch(void * &bp_history,Addr branch_addr);


bool lookup(Addr branch_addr, void * &bp_history);/* looks up the branch address in the bp_history table*/

void btbUpdate(Addr branch_addr, void * &bp_history);

void update(Addr branch_addr, bool taken, void *bp_history, bool squashed); /* updates the branch address as taken , not taken or squashed in the bp_history table*/


 void squash(void *bp_history);
 void squash2(Addr &branch_addr);  


void reset();



private:
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

unsigned global_pred;
unsigned InstShiftAmt;
 void updateGlobalHistTaken();

 void updateGlobalHistNotTaken();

     
unsigned globalhistory;
unsigned countertable[256];

uint8_t index;
uint8_t count;
unsigned branch_lower_order;

char msg[1000];

file fl;
};

#endif // __CPU_PRED_gselect_PRED_HH__
