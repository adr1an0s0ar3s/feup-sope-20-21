#ifndef SRC_LOG_H_
#define SRC_LOG_H_

#include "message.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

//Operations
#define RECVD "RECVD"
#define TSKEX "TSKEX"
#define TSKDN "TSKDN"
#define TLATE "2LATE"
#define FAILD "FAILD"

/**
 * @brief Writes a certain operation constituted by certain message in log
 * 
 * @param message Message of client
 * @param operation Operation of client
 */
void write_operation(Message message, char* operation);

#endif  // SRC_LOG_H_