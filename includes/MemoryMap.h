#define INST_ADDR_START     0x00000000

#define USRSYS_STACK_START  0x00100000
#define SVC_STACK_START     0x00300000
#define IRQ_STACK_START     0x00400000
#define FIQ_STACK_START     0x00500000
#define ABT_STACK_START     0x00600000
#define UND_STACK_START     0x00700000
#define TASK_STACK_START    0x00800000
#define GLOBAL_STACK_START  0x04800000
#define DALLOC_STACK_START  0x04900000

#define INST_MEM_SIZE       (USRSYS_STACK_START - INST_ADDR_START)

#define USR_TASK_STACK_SIZE 0x100000
#define USRSYS_STACK_SIZE   (SVC_STACK_START - USRSYS_STACK_START)  // 1MB Each
#define SVC_STACK_SIZE      (IRQ_STACK_START - SVC_STACK_START)     // 1MB Each
#define IRQ_STACK_SIZE      (FIQ_STACK_START - IRQ_STACK_START)     // 1MB Each
#define FIQ_STACK_SIZE      (ABT_STACK_START - FIQ_STACK_START)     // 1MB Each
#define ABT_STACK_SIZE      (UND_STACK_START - ABT_STACK_START)     // 1MB Each
#define UND_STACK_SIZE      (TASK_STACK_START - UND_STACK_START)    // 1MB Each
#define TASK_STACK_SIZE     (GLOBAL_STACK_START - TASK_STACK_START)  // 1MB Each
#define DALLOC_MEM_SIZE     (55 * 1024 * 1024)                      // 55MB

// Stack grows top to bottom (Full descending stack)
// There are 4-byte gap among individual stacks
#define USRSYS_STACK_TOP    (USRSYS_STACK_START + USRSYS_STACK_SIZE - 4)
#define SVC_STACK_TOP       (SVC_STACK_START + SVC_STACK_SIZE - 4)
#define IRQ_STACK_TOP       (IRQ_STACK_START + IRQ_STACK_SIZE - 4)
#define FIQ_STACK_TOP       (FIQ_STACK_START + FIQ_STACK_SIZE - 4)
#define ABT_STACK_TOP       (ABT_STACK_START + ABT_STACK_SIZE - 4)
#define UND_STACK_TOP       (UND_STACK_START + UND_STACK_SIZE - 4)
