
#include <stdio.h>
#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/gselect.hh"
#include "debug/Fetch.hh"


gselectBP::gselectBP(const Params *params ):BPredUnit(params)
     
{


// initialise history table and counter table

 globalhistory=0;


   for (int i=0; i<256;i++)
   {

    countertable[i]=0;
  }   
   
}

// reset function

void gselectBP:: reset() {

 
  globalhistory=0;


   for (int i=0; i<256;i++)
   {

    countertable[i]=0;
  }


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



// lookup

bool gselectBP:: lookup(Addr branch_addr, void * &bp_history ){

bool taken=0;

branch_lower_order= branch_addr & 255;
index= branch_lower_order ^ globalhistory; // xor

index=index & 255;

count=countertable[index];

taken= (count >> 1);   // get the msb of the sat count




sprintf (msg , " addr= 0x%lu", branch_addr);
fl.log(msg);


DPRINTF(Fetch, "branch lower order=%i\n", branch_lower_order);
DPRINTF(Fetch, "lookup complete\n");
   

return taken;
}

//update


void
gselectBP:: update(Addr branch_addr, bool taken, void *bp_history, bool squashed ){

    
   branch_lower_order = branch_addr & 255;
   
  
   

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




sprintf (msg , " addr= 0x%lu", branch_addr);
 fl.log(msg);

DPRINTF(Fetch, "update complete\n");


}


void
gselectBP::btbUpdate(Addr branch_addr, void * &bp_history)
{
// Place holder for a function that is called to update predictor history when
// a BTB entry is invalid or not found.
        //  updateGlobalHistNotTaken();


    

DPRINTF(Fetch, "btbUpdate\n");

sprintf (msg , " addr= 0x%lu", branch_addr);
 fl.log(msg);


}


void
gselectBP::uncondBranch(void *&bp_history,Addr branch_addr)
{

  
  DPRINTF(Fetch, "uncondBranch\n");
    updateGlobalHistTaken();


  sprintf (msg , " uncond branch called ");
    fl.log(msg);

}



void
gselectBP::squash(void *bp_history){

DPRINTF(Fetch, "squash\n");
  

  assert(bp_history == NULL);

  sprintf(msg,"squash called ");
   fl.log(msg);

}

gselectBP:: ~ gselectBP (void){


sprintf (msg , " update_count=%d ",update_count);

fl.log(msg);
fl.closelog();


}
















