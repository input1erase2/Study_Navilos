#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"

#include "msg.h"

KernelCirQ_t msgQ[KernelMsgQ_Num];

void kernel_msgQ_init(void) {
    for (uint32_t i = 0; i < KernelMsgQ_Num; ++i) {
        msgQ[i].front = 0;
        msgQ[i].rear = 0;
        memclr(msgQ[i].queue, MSG_Q_SIZE_BYTE);
    }
}

bool kernel_msgQ_is_empty(KernelMsgQ_t q) {
    if (q >= KernelMsgQ_Num) {
        return false;
    }
    return (msgQ[q].front == msgQ[q].rear);
}

bool kernel_msgQ_is_full(KernelMsgQ_t q) {
    if (q >= KernelMsgQ_Num) {
        return false;
    }
    return (msgQ[q].front == (msgQ[q].rear + 1) % MSG_Q_SIZE_BYTE);
}
// q[++rear] <- data
bool kernel_msgQ_enqueue(KernelMsgQ_t q, uint8_t data) {
    if (q >= KernelMsgQ_Num || kernel_msgQ_is_full(q)) {
        return false;
    }
    msgQ[q].rear = (msgQ[q].rear + 1) % MSG_Q_SIZE_BYTE;
    msgQ[q].queue[msgQ[q].rear] = data;
    
    return true;
}
// q[++front] -> data
bool kernel_msgQ_dequeue(KernelMsgQ_t q, uint8_t* out) {
    if (q >= KernelMsgQ_Num || kernel_msgQ_is_empty(q)) {
        return false;
    }
    msgQ[q].front = (msgQ[q].front + 1) % MSG_Q_SIZE_BYTE;
    *out = msgQ[q].queue[msgQ[q].front];

    return true;
}