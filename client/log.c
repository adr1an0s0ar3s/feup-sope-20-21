#include "log.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void write_operation(Message message,char* operation) {
    printf("%ld;%d;%d;%d;%ld;%d;%s\n", time(NULL), message.rid, message.tskload, message.pid, message.tid, message.tskres, operation);
}
