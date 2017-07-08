#ifndef __RTOS_TASK_H
#define __RTOS_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "c_lib.h"  
    
typedef struct rtos_task {
    
    uint32_t *task_stack;
    
}rtos_task_t;


#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_H */

/* end of file */

