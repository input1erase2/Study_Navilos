#include "stdint.h"
#include "stdbool.h"

#include "synch.h"

static int32_t semMax;
static int32_t sem;
static KernelMutex_t mutex;

void kernel_sem_init(int32_t max) {
    semMax = (max <= 0) ? SEM_MAX : max;
    semMax = (max >= SEM_MAX) ? SEM_MAX : max;
    sem = semMax;
}

bool kernel_sem_test(void) {
    if (sem <= 0) {
        return false;
    }
    sem--;
    return true;
}

void kernel_sem_release(void) {
    sem++;
    if (sem >= semMax) {
        sem = semMax;
    }
}

void kernel_mutex_init(void) {
    mutex.owner = 0;
    mutex.lock = false;
}

bool kernel_mutex_lock(uint32_t owner) {
    if (mutex.lock) {
        return false;
    }
    mutex.owner = owner;
    mutex.lock = true;
    return true;
}

bool kernel_mutex_unlock(uint32_t owner) {
    // Unlocking mutex is only available which locks the mutex.
    if (mutex.owner == owner) {
        mutex.lock = false;
        return true;
    }
    return false;
}