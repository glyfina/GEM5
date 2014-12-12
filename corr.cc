
#include <stdio.h>
#include "base/bitfield.hh"
#include "base/intmath.hh"
#include "base/misc.hh"
#include "base/trace.hh"
#include "cpu/pred/corr.hh"
#include "debug/Fetch.hh"


corrBP::corrBP(const Params *params ):BPredUnit(params)
     
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


 DPRINTF(Fetch, "reset\n");
 sprintf (msg , " reset called ");
 // fl.log(msg);
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

branch_lower_order= branch_addr & 15;

index=historytable[branch_lower_order];

count=countertable[index];

taken= (count >> 1);   // get the msb of the sat count




sprintf (msg , " addr= 0x%lu", branch_addr);
// fl.log(msg);


DPRINTF(Fetch, "branch lower order=%i\n", branch_lower_order);
DPRINTF(Fetch, "lookup complete\n");
   

return taken;
}

//update


void
corrBP:: update(Addr branch_addr, bool taken, void *bp_history, bool squashed ){

    
   branch_lower_order = branch_addr & 15;
   
  
   

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




sprintf (msg , " addr= 0x%lu", branch_addr);
// fl.log(msg);

DPRINTF(Fetch, "update complete\n");


}


void
corrBP::btbUpdate(Addr branch_addr, void * &bp_history)
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
corrBP::uncondBranch(void *&bp_history,Addr branch_addr)
{

  branch_lower_order = branch_addr & 15;
  DPRINTF(Fetch, "uncondBranch\n");
    updateGlobalHistTaken(branch_lower_order);


  sprintf (msg , " uncond branch called ");
   // fl.log(msg);

}



void
corrBP::squash(void *bp_history){

DPRINTF(Fetch, "squash\n");
  //  updateGlobalHistNotTaken();

  assert(bp_history == NULL);

  sprintf(msg,"squash called ");
  // fl.log(msg);

}

corrBP:: ~ corrBP (void){


//sprintf (msg , " update_count=%d ",update_count);

 //fl.log(msg);
//fl.closelog();


}
















