#include <stdio.h>
#include <stdlib.h>
int main(void)
{
int i,n;
n=0xFFFFFFFF;
for(i=0; i<0xFFFFFFFF;i++){
printf("%d.%d.%d.%d\n", rand()%0xFF,rand()%0xFF,rand()%0xFF,rand()%0xFF);
}
return(0);
}
//just a test or a draft
