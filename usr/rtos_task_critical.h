#ifndef __RTOS_TASK_CRITICAL_H
#define __RTOS_TASK_CRITICAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "arm.h"
    
/*   
 * \par 嵌套使用示例：
 * \code 
 * uint32_t key0, key1;
 *
 * key0 = rtos_task_critical_entry();  // 中断马上关闭
 * key1 = rtos_task_critical_entry();  // 中断保持关闭
 * 
 * rtos_task_critical_exit(key1);   // 中断保持关闭
 * rtos_task_critical_exit(key0);   // 中断马上打开
 * \endcode
 */    

    
    
/**
 * \brief  进入临界区
 * \param[in] 无
 * \return    进入之前的临界区CPU中断状态值
 */
uint32_t rtos_task_critical_entry (void);
    
    
/**
 * \brief  退出临界区
 * \param[in] status : 进入临界区之前的CPU中断使能或禁能状态
 * \return    无
 */
void rtos_task_critical_exit (uint32_t status);





#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CRITICAL_H */
