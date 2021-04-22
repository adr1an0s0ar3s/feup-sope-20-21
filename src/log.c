#include "log.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>


void write_operation(Message mensage,char* operation){
    printf("%ld;%d;%d;%d;%ld;%d;%s\n",time(NULL),mensage.rid,mensage.tskload,mensage.pid,mensage.tid,mensage.tskres,operation);
   
}
