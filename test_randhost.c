#include <stdio.h>
#include <stdlib.h>
int main(void)
{
   int i;
   int host[i];
   for( host[i] = 0 ; host[i] < 0xFFFFFFFF ; host[i]++ ) {
           host[1]=rand()%0xFF;
           host[2]=rand()%0xFF;
           host[3]=rand()%0xFF;
           host[4]=rand()%0xFF;
      printf("%d.%d.%d.%d\n", host[1],host[2],host[3],host[4]);
   }
   return(0);
}


//just a test or a draft
