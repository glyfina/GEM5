
#include <stdio.h>
#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/gselect.hh"
#include "debug/Fetch.hh"


gselectBP::gselectBP(const Params *params ):BPredUnit(params),InstShiftAmt(params->InstShiftAmt)
     
{


// initialise history table and counter table

 globalhistory=0;


   for (int i=0; i<256;i++)
   {

    countertable[i]=0;
  }

  
  global_pred=0;
  fl.openlog();
  sprintf (msg , " constructor called ");
  fl.log(msg);   
   
}

// reset function

void gselectBP:: reset() {

 
  globalhistory=0;


   for (int i=0; i<256;i++)
   {

    countertable[i]=0;
  }
 
  
  global_pred=0;
  fl.openlog();
  sprintf (msg , " constructor called ");
  fl.log(msg);

 DPRINTF(Fetch, "reset\n");
 sprintf (msg , " reset called ");
  fl.log(msg);
}



inline
void
gselectBP::updateGlobalHistTaken()
{
     globalhistory = (globalhistory << 1) | 1;
    globalhistory = globalhistory & 255;
    DPRINTF(Fetch, "global branch taken\n");
   
}



inline
void
gselectBP::updateGlobalHistNotTaken()
{
   globalhistory = (globalhistory << 1);
    globalhistory = globalhistory & 255;

    DPRINTF(Fetch, "global branch not taken\n");
   
}

inline
unsigned
gselectBP::calcLocHistIdx(Addr &branch_addr)
{
    // Get low order bits after removing instruction offset.
    return (branch_addr >> InstShiftAmt) & (15);
}

// lookup

bool gselectBP:: lookup(Addr branch_addr, void * &bp_history ){

bool taken=0;

branch_lower_order= calcLocHistIdx(branch_addr);
index= branch_lower_order ^ globalhistory; // xor

index=index & 255;

count=countertable[index];

taken= (count >> 1);   // get the msb of the sat count




sprintf (msg , " addr= 0x%lu", branch_addr);
 fl.log(msg);


DPRINTF(Fetch, "branch lower order=%i\n", branch_lower_order);
DPRINTF(Fetch, "lookup complete\n");
  
// Create BPHistory and pass it back to be recorded.
    BPHistory *history = new BPHistory;
    history->global_pred = taken;
    bp_history = (void *)history;
DPRINTF(Fetch, "branch lower order=%i\n", branch_lower_order);
DPRINTF(Fetch, "lookup complete\n"); 

return taken;
}

//update


void
gselectBP:: update(Addr branch_addr, bool taken, void *bp_history, bool squashed ){

    
   branch_lower_order = calcLocHistIdx(branch_addr);
      
if (bp_history) {
        BPHistory *history = static_cast<BPHistory *>(bp_history); 
   

// update local history
 if (taken) {
        DPRINTF(Fetch, "Branch updated as taken.\n");
       
        index= branch_lower_order ^ globalhistory;
        index=index & 255;
        count=countertable[index];
        if ((count>=0) & (count<3))
        count++;
        countertable[index]=count;

              

       
        updateGlobalHistTaken();
        DPRINTF(Fetch, "global update.\n");        

    } else {
        DPRINTF(Fetch, "Branch updated as not taken.\n");
        
         index= branch_lower_order ^ globalhistory;
         index=index & 255;
         count=countertable[index];  
       
        if ((count>0) & (count<=3))
        count--;
        countertable[index]=count;

                
        updateGlobalHistNotTaken();
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
gselectBP::btbUpdate(Addr branch_addr, void * &bp_history)
{

// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.

branch_lower_order = calcLocHistIdx(branch_addr);

// get the stored predictor history
  BPHistory *history = static_cast<BPHistory *>(bp_history);
      
// update History to not taken
   
   index= branch_lower_order ^ globalhistory;
         index=index & 255;
         count=countertable[index];  
       
        if ((count>0) & (count<=3))
        count--;
        countertable[index]=count;
         
        updateGlobalHistNotTaken();

 //pass it back to be recorded
    history->global_pred = 0;
    
    bp_history = static_cast<void *>(history);

  
DPRINTF(Fetch, "btbUpdate\n");

sprintf (msg , " addr= 0x%lu", branch_addr);
fl.log(msg);


}


void
gselectBP::uncondBranch(void *&bp_history,Addr branch_addr)
{
  // Create BPHistory and pass it back to be recorded.
    BPHistory *history = new BPHistory;
    history->global_pred= 1;
    bp_history = static_cast<void *>(history);
  
    DPRINTF(Fetch, "uncondBranch\n");
    branch_lower_order = calcLocHistIdx(branch_addr);
  
    index= branch_lower_order ^ globalhistory;
        index=index & 255;
        count=countertable[index];
        if ((count>=0) & (count<3))
        count++;
        countertable[index]=count;
   
   DPRINTF(Fetch, "uncondBranch\n");
    updateGlobalHistTaken();


  sprintf (msg , " uncond branch called ");
  fl.log(msg);

}



void
gselectBP::squash(void *bp_history){

DPRINTF(Fetch, "squash\n");
  //  updateGlobalHistNotTaken();

  updateGlobalHistNotTaken();
  sprintf(msg,"squash called ");
  // fl.log(msg);

}


void
gselectBP::squash2(Addr &branch_addr){
 
  
     branch_lower_order = calcLocHistIdx(branch_addr);
     // decrement counters since branch was mispredicted  
       
     index= branch_lower_order ^ globalhistory;
         index=index & 255;
         count=countertable[index];  
       
        if ((count>0) & (count<=3))
        count--;
        countertable[index]=count;

                  


}

gselectBP:: ~ gselectBP (void){


}

#ifdef DEBUG
int
gselectBP::BPHistory::newCount = 0;
#endif

