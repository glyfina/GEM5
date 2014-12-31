
#include <stdio.h>
#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/corr.hh"
#include "debug/Fetch.hh"


corrBP::corrBP(const Params *params ):BPredUnit(params),InstShiftAmt(params->InstShiftAmt)
     
{

// initialise history table and counter table

 for (int i=0; i<16;i++)
   {

    historytable[i]=0;
  } 


   for (int i=0; i<16;i++)
   {

    countertable[i]=0;
  }   
   
  local_pred=0;
  global_pred=0;
  fl.openlog();
  sprintf (msg , " constructor called ");
  fl.log(msg);
}

// reset function

void corrBP:: reset() {

 
   for (int i=0; i<16;i++)
   {

    historytable[i]=0;
  } 


   for (int i=0; i<16;i++)
   {

    countertable[i]=0;
  }

  local_pred=0;
  global_pred=0;

}

inline
unsigned
corrBP::calcLocHistIdx(Addr &branch_addr)
{
    // Get low order bits after removing instruction offset.
    return (branch_addr >> InstShiftAmt) & (15);
}

inline
void
corrBP::updateGlobalHistTaken(unsigned branch_lower_order)
{
   historytable[branch_lower_order]=(historytable[branch_lower_order] <<1) | 1;
   historytable[branch_lower_order]=(historytable[branch_lower_order] & 15); 
    
    
    DPRINTF(Fetch, "global branch taken\n");
   
}



inline
void
corrBP::updateGlobalHistNotTaken(unsigned branch_lower_order)
{
   historytable[branch_lower_order]=(historytable[branch_lower_order] <<1) ;
   historytable[branch_lower_order]=(historytable[branch_lower_order] & 15); 
    
       

    DPRINTF(Fetch, "global branch not taken\n");
   
}



// lookup

bool corrBP:: lookup(Addr branch_addr, void * &bp_history ){

bool taken=0;

branch_lower_order= calcLocHistIdx(branch_addr);

index=historytable[branch_lower_order];

count=countertable[index];

taken= (count >> 1);   // get the msb of the sat count


sprintf (msg , " addr= 0x%lu", branch_addr);
 fl.log(msg);

// Create BPHistory and pass it back to be recorded.
    BPHistory *history = new BPHistory;
    history->global_pred = taken;
    bp_history = (void *)history;
DPRINTF(Fetch, "branch lower order=%i\n", branch_lower_order);
DPRINTF(Fetch, "lookup complete\n");
   

return taken;
}



// update
void
corrBP:: update(Addr branch_addr, bool taken, void *bp_history, bool squashed ){

    
   branch_lower_order = calcLocHistIdx(branch_addr);
      
if (bp_history) {
        BPHistory *history = static_cast<BPHistory *>(bp_history);
// update local history
 if (taken) {
        DPRINTF(Fetch, "Branch updated as taken.\n");
       
        index=historytable[branch_lower_order];
        count=countertable[index];
        if ((count>=0) & (count<3))
        count++;
        countertable[index]=count;
    
        updateGlobalHistTaken(branch_lower_order);
        DPRINTF(Fetch, "global update.\n");        



    } else {
        DPRINTF(Fetch, "Branch updated as not taken.\n");
                
       index=historytable[branch_lower_order];
        count=countertable[index];
        if ((count>0) & (count<=3))
        count--;
        countertable[index]=count;
       
        updateGlobalHistNotTaken(branch_lower_order);
        DPRINTF(Fetch, "global update.\n");
   }
  //pass it back to be recorded
    history->global_pred = taken;
    
    bp_history = static_cast<void *>(history);

}

sprintf (msg , " addr= 0x%lu", branch_addr);
 fl.log(msg);

DPRINTF(Fetch, "update complete\n");


}


void
corrBP::btbUpdate(Addr branch_addr, void * &bp_history)
{
// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.

branch_lower_order = calcLocHistIdx(branch_addr);

// get the stored predictor history
  BPHistory *history = static_cast<BPHistory *>(bp_history);
      
// update History to not taken

   index=historytable[branch_lower_order];
        count=countertable[index];
        if ((count>0) & (count<=3))
        count--;
        countertable[index]=count;
       
        updateGlobalHistNotTaken(branch_lower_order); 

//pass it back to be recorded
    history->global_pred = 0;
    
    bp_history = static_cast<void *>(history);


DPRINTF(Fetch, "btbUpdate\n");

sprintf (msg , " addr= 0x%lu", branch_addr);
fl.log(msg);

}


void
corrBP::uncondBranch(void *&bp_history,Addr branch_addr)
{

// Create BPHistory and pass it back to be recorded.
    BPHistory *history = new BPHistory;
    history->global_pred= 1;
    bp_history = static_cast<void *>(history);
  
    DPRINTF(Fetch, "uncondBranch\n");
    branch_lower_order = calcLocHistIdx(branch_addr);
    
        index=historytable[branch_lower_order];
        count=countertable[index];
        if ((count>=0) & (count<3))
        count++;
        countertable[index]=count;
    
        updateGlobalHistTaken(branch_lower_order); 


  sprintf (msg , " uncond branch called ");
  fl.log(msg);

}



void
corrBP::squash(void *bp_history){

DPRINTF(Fetch, "squash\n");
  //  updateGlobalHistNotTaken();

  BPHistory *history = static_cast<BPHistory *>(bp_history);

       // Delete this BPHistory now that we're done with it.
    delete history;

  sprintf(msg,"squash called ");
  fl.log(msg);

}

void
corrBP::squash2(Addr &branch_addr){
 
  
     branch_lower_order = calcLocHistIdx(branch_addr);
     // decrement counters since branch was mispredicted  
       index=historytable[branch_lower_order];
        count=countertable[index];
        if ((count>0) & (count<=3))
        count--;
        countertable[index]=count;
       
        updateGlobalHistNotTaken(branch_lower_order);


}

corrBP:: ~ corrBP (void){


sprintf (msg , " destructor");

fl.log(msg);
fl.closelog();


}


#ifdef DEBUG
int
corrBP::BPHistory::newCount = 0;
#endif




















