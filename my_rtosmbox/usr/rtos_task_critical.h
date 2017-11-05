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


/**
 * \brief  初始化调度锁
 * \param[in] 无
 * \return    无
 */
void rtos_task_schedlock_init (void);


/**
 * \brief  调度锁打开，禁止任务调度
 * \param[in] 无
 * \return    无
 */
void rtos_task_sched_disable (void);


/**
 * \brief  调度锁关闭，允许任务调度
 * \param[in] 无
 * \return    无
 */
void rtos_task_sched_enable (void);


/**
 * \brief  查询调度锁状态
 * \param[in] 无
 * \return    TASKSCHED_UNLOCK: 允许任务调度; TASKSCHED_LOCK: 禁止任务调度
 */
uint8_t rtos_task_schedlock_status (void);





#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CRITICAL_H */
