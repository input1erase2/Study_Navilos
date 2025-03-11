#ifndef KERNEL_MSG_H_
#define KERNEL_MSG_H_

#define MSG_Q_SIZE_BYTE     512     // 512-Byte

typedef enum KernelMsgQ_t { // Message Queue per task
    KernelMsgQ_Task0,       // Message Queue 0
    KernelMsgQ_Task1,       // Message Queue 1
    KernelMsgQ_Task2,       // Message Queue 2
    KernelMsgQ_Num
} KernelMsgQ_t;

// Circular Queue
typedef struct KernelCirQ_t {
    uint32_t front;
    uint32_t rear;
    uint8_t queue[MSG_Q_SIZE_BYTE];
}KernelCirQ_t;

void kernel_msgQ_init(void);
bool kernel_msgQ_is_empty(KernelMsgQ_t q);
bool kernel_msgQ_is_full(KernelMsgQ_t q);
bool kernel_msgQ_enqueue(KernelMsgQ_t q, uint8_t data);
bool kernel_msgQ_dequeue(KernelMsgQ_t q, uint8_t* out);
uint32_t kernel_msgQ_count(KernelMsgQ_t q);

#endif /* KERNEL_MSG_H_ */