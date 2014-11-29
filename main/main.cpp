#include <stdio.h>

//Libraries
#include "arvteledyne.h" 
#include "uavcv.h"

int main(){
Teledyne* TSC4096 = new Teledyne();
compVision* ip = new compVision;
printf("Made Camera with H=%d, W =%d \n", TSC4096->imgheight, TSC4096->imgwidth);
ip->showImage();

waitKey(0);
delete TSC4096;
return 0;
}
