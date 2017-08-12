#ifndef __RTOS_TASK_SWITCH_H
#define __RTOS_TASK_SWITCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "arm.h"


/**
 * \brief 触发PendSVC
 * \param[in] 无
 * \return    无
 */
void PendSVC_Trigger(void);
    
    
/**
 * \brief 任务切换
 * \param[in] 无
 * \return    无
 */
void rtos_task_switch (void);





#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_SWITCH_H */



