#ifndef LOG_H
#define LOG_H 

#include "message.h"
#include "time.h"

//Operations
#define IWANT "IWANT"
#define GOTRS "GOTRS"
#define CLOSD "CLOSD"
#define GAVUP "GAVUP"

/**
 * @brief Writes a certain operation constituted by certain message in log
 * 
 * @param mensage Message of client
 * @param operation Operation of client
 */
void write_operation(Message mensage,char* operation);

#endif