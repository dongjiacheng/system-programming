/**
 * Extreme Edge Cases Lab
 * CS 241 - Spring 2017
 */
#include "camelCaser_tests.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "camelCaser.h"

/*
 * Testing function for various implementations of camelCaser.
 *
 * @param  camelCaser   A pointer to the target camelCaser function.
 * @return              Correctness of the program (0 for wrong, 1 for correct).
 */
int test_camelCaser(char **(*camelCaser)(const char *)) {
  // TODO: Return 1 if the passed in function works properly; 0 if it doesn't.
  char * input[] = {
        "heLLo..World.",
        "...H..",
        NULL,
        "Guo nian .hao",
        "                     you.234.",
        "1yi 2er . 3san.",
        "1 yi 2 er . 3 san.",
        "yi1,2er,s3ni",
        "on1e two 22 t3three.",
        "1.2.3",
        "yi.er.san"
    	};

    int count=sizeof(input)/sizeof(char*); //3
    int StrNum[count];

    char** Usage;
    //how many results
    char ***output=malloc(count*sizeof(Usage));



    //case 1 "heLLo..World."
    char * temp1[]={"hello",
	"",
	"world",
	NULL
	};
    StrNum[0]=sizeof(temp1)/sizeof(char*);
    *(output+0)=temp1;

    //case 2 "...H.."
    char* temp2[]={	"",
	"",
	"",
	"h",
	"",
	NULL};
    StrNum[1]=sizeof(temp2)/sizeof(char*);
    *(output+1)=temp2;

    //case 3 NULL
    StrNum[2]=0;
    *(output+2)=NULL;

    //case 4 "Guo nian .hao"
    char* temp4[]={	"guoNian",
	NULL
};
    StrNum[3]=sizeof(temp4)/sizeof(char*);
    *(output+3)=temp4;

    //case 5 "                     you.234."
    char* temp5[]={	"you",
	"234",
	NULL
	};
    StrNum[4]=sizeof(temp5)/sizeof(char*);
    *(output+4)=temp5;

    //case 6: "1yi 2er . 3san."
    char* temp6[]={	"1yi2Er",
	"3san",
	NULL
};
    StrNum[5]=sizeof(temp6)/sizeof(char*);
    *(output+5)=temp6;

    //case 7: "1 yi 2 er . 3 san."
    char* temp7[]={	"1Yi2Er",
	"3San",
	NULL
};
    StrNum[6]=sizeof(temp7)/sizeof(char*);
    *(output+6)=temp7;

    //case 8: "yi1,2er,s3ni"
    char* temp8[]={	"yi1",
	"2er",
	NULL};
    StrNum[7]=sizeof(temp8)/sizeof(char*);
    *(output+7)=temp8;

    //case 9: "on1e two 22 t3three."
    char* temp9[]={	"on1eTwo22T3three",
	NULL
};
    StrNum[8]=sizeof(temp9)/sizeof(char*);
    *(output+8)=temp9;

    //case 10: "1.2.3"
    char* temp10[]={	"1",
	"2",
	NULL};
    StrNum[9]=sizeof(temp10)/sizeof(char*);
    *(output+9)=temp10;

    //case 11: "yi.er.san"
    char* temp11[]={	"yi",
	"er",
	NULL};
    StrNum[10]=sizeof(temp11)/sizeof(char*);
    *(output+10)=temp11;
    
  
    for(int i=0;i<count;i++){
    	if(input[i]==NULL){
    		if(camelCaser(input[i]))
    		{    			
    			return 0;//check NULL
    		} 
    	}
    	else{
	    	char**test = camelCaser(input[i]); 


            int PtrNum=0;
          //  for(;test[PtrNum];PtrNum++);
            //add Null ptr
       //     PtrNum++;
		while(test[PtrNum]!= NULL)
			PtrNum++;
			PtrNum++;

			for(int m=0;m<(PtrNum-1);m++)
				printf("%s\n",test[m]);
            if(PtrNum != StrNum[i])
            {
                return 0;
            }


	    	for(int j=0;j<StrNum[i];j++){

	    		char* tempo =(*(output+i))[j];
	    		
	    		if(   (j!=StrNum[i]-1) && (strcmp(test[j],tempo) !=0) ) {
                   return 0;
	    		}
	    		else if( ( j==(StrNum[i]-1))&&( tempo!=NULL ) ){
	                    return 0;
	    		}     
	    	}
    	}
    	
    }
 
  return 1;
}
