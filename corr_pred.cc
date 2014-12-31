


#include <stdio.h>
#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/corr_pred.hh"
#include "debug/Fetch.hh"



corr_predBP::corr_predBP(const Params *params ):BPredUnit(params),PredSize(params->PredSize),PredCtrs(params->PredCtrs),globalHistory_size(params->globalHistory_size),InstShiftAmt(params->InstShiftAmt)
     
{

 // constructor called
 localPredictorSets= (PredSize)/(PredCtrs);

 if (!isPowerOf2(localPredictorSets)) {
        fatal("Invalid local predictor size!\n");
    }  


   globalHistory= 1<<globalHistory_size; 

   

    for(int i=0;i<globalHistory ;i++){
         for (int j=0; j<localPredictorSets ;j++){
        value[i][j]=0;
   }}
     
    globalHistory_value=0;
    local_pred=0;
    global_pred=0;
    
    DPRINTF(Fetch, "local Predictor Size:%i\n", PredSize);
    DPRINTF(Fetch, "local Predictor Sets:%i\n", localPredictorSets);
    
       
   
}



void corr_predBP:: reset() {

 // reset function

    for(int i=0;i<globalHistory;i++){
         for (int j=0; j<localPredictorSets ;j++){
        value[i][j]=0;
   }}

  globalHistory_value=0;
  local_pred=0;
  global_pred=0;

 DPRINTF(Fetch, "reset\n");

}



inline
void
corr_predBP::updateGlobalHistTaken()
{
    globalHistory_value = (globalHistory_value << 1) | 1;
    globalHistory_value = globalHistory_value & (globalHistory-1) ;
    DPRINTF(Fetch, "global branch taken\n");
   
}



inline
void
corr_predBP::updateGlobalHistNotTaken()
{
    globalHistory_value = (globalHistory_value << 1);
    globalHistory_value = globalHistory_value & (globalHistory-1);
    DPRINTF(Fetch, "global branch not taken\n");
   
}

inline
unsigned
corr_predBP::calcLocHistIdx(Addr &branch_addr)
{
    // Get low order bits after removing instruction offset.
    return (branch_addr >> InstShiftAmt) & (localPredictorSets - 1);
}



bool corr_predBP:: lookup(Addr branch_addr, void * &bp_history ){

// lookup

bool taken=0;

branch_lower_order= calcLocHistIdx(branch_addr);


count=value[globalHistory_value][branch_lower_order];

taken= (count >> (PredCtrs - 1));   // get the msb of the sat count


// Create BPHistory and pass it back to be recorded.
    BPHistory *history = new BPHistory;
    history->local_pred = taken;
    history->global_pred = globalHistory_value & 1;
    bp_history = (void *)history;



DPRINTF(Fetch, "branch lower order=%i\n", branch_lower_order);
DPRINTF(Fetch, "lookup complete\n");
   

return taken;
}




void
corr_predBP:: update(Addr branch_addr, bool taken, void *bp_history, bool squashed ){

   // update
    
   branch_lower_order = calcLocHistIdx(branch_addr);
   
  if (bp_history) {
        BPHistory *history = static_cast<BPHistory *>(bp_history);

 if (taken) {
        DPRINTF(Fetch, "Branch updated as taken.\n");
      
       
        count=value[globalHistory_value][branch_lower_order];
        if ((count>=0) & (count<3))
        count++;
        value[globalHistory_value][branch_lower_order]=count;

              

       
        updateGlobalHistTaken();
        DPRINTF(Fetch, "global update.\n");        



    } else {
        DPRINTF(Fetch, "Branch updated as not taken.\n");
        
         
       count=value[globalHistory_value][branch_lower_order];
       if ((count>0) && (count<=3)) 
       count--;
       value[globalHistory_value][branch_lower_order]=count;


        updateGlobalHistNotTaken();
        DPRINTF(Fetch, "global update.\n");
   }


   //pass it back to be recorded
    history->local_pred = taken;
    history->global_pred = globalHistory_value & 1;
    bp_history = static_cast<void *>(history);

DPRINTF(Fetch, "update complete\n");

}
}


void
corr_predBP::btbUpdate(Addr branch_addr, void * &bp_history)
{
// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.
   
  branch_lower_order = calcLocHistIdx(branch_addr);

// get the stored predictor history
  BPHistory *history = static_cast<BPHistory *>(bp_history);
      
// update local History to not taken

  
       count=value[globalHistory_value][branch_lower_order];
       if ((count>0) && (count<=3)) 
       count--;
       value[globalHistory_value][branch_lower_order]=count;



// update global History to not taken

      
        updateGlobalHistNotTaken();
        
//pass it back to be recorded
    history->local_pred = 0;
    history->global_pred = globalHistory_value &1;
    bp_history = static_cast<void *>(history);


        DPRINTF(Fetch, "btbUpdate\n");

   
}


void
corr_predBP::uncondBranch(void *&bp_history,Addr branch_addr)
{

   // Create BPHistory and pass it back to be recorded.
    BPHistory *history = new BPHistory;
    history->local_pred=1;
    history->global_pred= 1;
    bp_history = static_cast<void *>(history);
  
    DPRINTF(Fetch, "uncondBranch\n");
    updateGlobalHistTaken();


}



void
corr_predBP::squash(void *bp_history){

DPRINTF(Fetch, "squash\n");
  
BPHistory *history = static_cast<BPHistory *>(bp_history);

    // Restore global history to state prior to this branch.
    globalHistory_value = (globalHistory_value>>1) & (globalHistory-1);

    
    // Delete this BPHistory now that we're done with it.
    delete history;

}

void
corr_predBP::squash2(Addr &branch_addr){
 
   branch_lower_order = calcLocHistIdx(branch_addr);
     // decrement counters since branch was mispredicted  
      count=value[globalHistory_value][branch_lower_order];
       if ((count>0) && (count<=3)) 
       count--;
       value[globalHistory_value][branch_lower_order]=count;
         

     

}

corr_predBP:: ~ corr_predBP (void){



}


#ifdef DEBUG
int
corr_predBP::BPHistory::newCount = 0;
#endif
















