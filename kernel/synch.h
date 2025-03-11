#ifndef KERNEL_SYNCH_H_
#define KERNEL_SYNCH_H_

#define SEM_MAX     8

typedef struct KernelMutex_t {
    uint32_t owner;
    bool lock;
} KernelMutex_t;

void kernel_sem_init(int32_t max);
bool kernel_sem_test(void);
void kernel_sem_release(void);

void kernel_mutex_init(void);
bool kernel_mutex_lock(uint32_t owner);
bool kernel_mutex_unlock(uint32_t owner);

#endif /* KERNEL_SYNCH_H_ */