#ifndef SRC_LOG_H_
#define SRC_LOG_H_

#include "message.h"

//Operations
#define IWANT "IWANT"
#define GOTRS "GOTRS"
#define CLOSD "CLOSD"
#define GAVUP "GAVUP"

/**
 * @brief Writes a certain operation constituted by certain message in log
 * 
 * @param message Message of client
 * @param operation Operation of client
 */
void write_operation(Message message, char* operation);

#endif  // SRC_LOG_H_