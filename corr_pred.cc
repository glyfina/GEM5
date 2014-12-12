

#include <stdio.h>
#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/corr_pred.hh"
#include "debug/Fetch.hh"


corr_predBP::corr_predBP(const Params *params ):BPredUnit(params),PredSize(params->PredSize),PredCtrs(params->PredCtrs),globalHistory(params->globalHistory)
     
{
sprintf (msg, " constructor called ");
  
   // fl.openlog ();
   // // fl.log(msg);
    

 localPredictorSets= (PredSize)/(PredCtrs);

   
// Setup the array of counters for the local predictor.
   // localCtrs.resize(localPredictorSets);
/*
// set the PredCtrs
   for (int i = 0; i < localPredictorSets; ++i)
     {   localCtrs[i].setBits(PredCtrs);
         
     }
//Setup the history table for the local table
    //localHistoryTable_sub.resize(localPredictorSets);
    localHistoryTable.resize(4);


    for (int i = 0; i < localPredictorSets; ++i){
        localCtrs[i] = 0;
         }


     for(int i=0; i<4 ; i++){
     localHistoryTable.push_back(localCtrs);}
  */
     for(int i=0;i<32 ;i++){
         for (int j=0; j<16 ;j++){
        value[i][j]=0;
   }}
     
    DPRINTF(Fetch, "local Predictor Size:%i\n", PredSize);
    DPRINTF(Fetch, "local Predictor Sets:%i\n", localPredictorSets);
    
   
    globalHistory=0; 
    flag=0;
    
   
}

// reset function

void corr_predBP:: reset() {

 /*for(int i=0;i<4;i++){

   for(int j=0;j<16;j++){

   localHistoryTable[i][j].reset();
   
   }
}*/

    for(int i=0;i<32;i++){
         for (int j=0; j<16 ;j++){
        value[i][j]=0;
   }}



 DPRINTF(Fetch, "reset\n");
 sprintf (msg , " reset called ");
 // fl.log(msg);
}



inline
void
corr_predBP::updateGlobalHistTaken()
{
    globalHistory = (globalHistory << 1) | 1;
    globalHistory = globalHistory & 31;
    DPRINTF(Fetch, "global branch taken\n");
   
}



inline
void
corr_predBP::updateGlobalHistNotTaken()
{
    globalHistory = (globalHistory << 1);
    globalHistory = globalHistory & 31;
    DPRINTF(Fetch, "global branch not taken\n");
   
}



// lookup

bool corr_predBP:: lookup(Addr branch_addr, void * &bp_history ){

bool taken=0;

branch_lower_order= branch_addr & 15;


//count=localHistoryTable[globalHistory][branch_lower_order].read();

count=value[globalHistory][branch_lower_order];

taken= (count >> (PredCtrs - 1));   // get the msb of the sat count




sprintf (msg , " addr= 0x%lu", branch_addr);
// fl.log(msg);


DPRINTF(Fetch, "branch lower order=%i\n", branch_lower_order);
DPRINTF(Fetch, "lookup complete\n");
   

return taken;
}

//update


void
corr_predBP:: update(Addr branch_addr, bool taken, void *bp_history, bool squashed ){

    
   branch_lower_order = branch_addr & 15;
   
  
   

// update local history
 if (taken) {
        DPRINTF(Fetch, "Branch updated as taken.\n");
       // localHistoryTable[globalHistory][branch_lower_order].increment();
       
        count=value[globalHistory][branch_lower_order];
        if ((count>=0) & (count<3))
        count++;
        value[globalHistory][branch_lower_order]=count;

              

       
        updateGlobalHistTaken();
        DPRINTF(Fetch, "global update.\n");        



    } else {
        DPRINTF(Fetch, "Branch updated as not taken.\n");
        //localHistoryTable[globalHistory][branch_lower_order].decrement();
         
       count=value[globalHistory][branch_lower_order];
       if ((count>0) && (count<=3)) 
       count--;
       value[globalHistory][branch_lower_order]=count;


        updateGlobalHistNotTaken();
        DPRINTF(Fetch, "global update.\n");
   }



 // update global history
/* if(taken){
        updateGlobalHistTaken();
        DPRINTF(Fetch, "global update.\n");     
   }

  else{
       updateGlobalHistNotTaken();
       DPRINTF(Fetch, "global update.\n");
}
 */

sprintf (msg , " addr= 0x%lu", branch_addr);
// fl.log(msg);

DPRINTF(Fetch, "update complete\n");


}


void
corr_predBP::btbUpdate(Addr branch_addr, void * &bp_history)
{
// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.
        //  updateGlobalHistNotTaken();

//Update Global History to Not Taken (clear LSB)
    

DPRINTF(Fetch, "btbUpdate\n");

sprintf (msg , " addr= 0x%lu", branch_addr);
// fl.log(msg);


}


void
corr_predBP::uncondBranch(void *&bp_history,Addr branch_addr)
{

  
  DPRINTF(Fetch, "uncondBranch\n");
    updateGlobalHistTaken();


  sprintf (msg , " uncond branch called ");
   // fl.log(msg);

}



void
corr_predBP::squash(void *bp_history){

DPRINTF(Fetch, "squash\n");
  //  updateGlobalHistNotTaken();

  assert(bp_history == NULL);

  sprintf(msg,"squash called ");
  // fl.log(msg);

}

corr_predBP:: ~ corr_predBP (void){


//sprintf (msg , " update_count=%d ",update_count);

 //fl.log(msg);
//fl.closelog();


}

/*
#ifdef DEBUG
int
corr_predBP::BPHistory::newCount = 0;
#endif
*/














