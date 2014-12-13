

#ifndef __CPU_PRED_FILE_HH__
#define __CPU_PRED_FILE_HH__



# include <string.h>
# include <iostream>
# include <stdio.h>

class file{

public:

FILE * fp;
int fp_error;


// openlog
void openlog(){

fp=fopen ("predictor.txt","wb");

if (fp!=NULL)
{ fp_error=0;
   }


else{ fp_error=-1;
        }

printf("\n opening predictor file");

}

// close log
void closelog(void){

if (fp_error!=-1)

fclose(fp);


printf("\n closing predictor file");
}

// log 

void log(char* msg){

if (fp_error!=-1)
  {
     fwrite(msg , sizeof(double) , sizeof(msg) , fp );

 }
}




};

#endif // __CPU_PRED_FILE_HH__
