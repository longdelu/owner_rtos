/*******************************************************************************
*                                 Embedded_rtos
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Free soft.
* All rights reserved.
*
* Contact information:
* web site:    
* e-mail:      
*******************************************************************************/

/**
 * \file
 * \brief 操作系统临界区保护（通过开关总中断来实现）
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_task_critical.h"
#include "c_lib.h"
#include "rtos_task_switch.h"
#include "rtos_task.h"

/** \brief 调度锁计数器 */
uint16_t __schedlock_count = 0;
 

/**
 * \brief  进入临界区
 */
uint32_t rtos_task_critical_entry (void)
{
    uint32_t primask = __get_PRIMASK();
    __disable_irq();        /*  CPSID I */
    return primask;    
}

/**
 * \brief  退出临界区
 */
void rtos_task_critical_exit (uint32_t status)
{
     __set_PRIMASK(status);       
}

/**
 * \brief  调度锁打开，禁止任务调度
 */
void rtos_task_sched_disable (void)
{
     uint32_t status = rtos_task_critical_entry();
    
     if (__schedlock_count < 65535) {
         __schedlock_count++;
     }
     
     rtos_task_critical_exit(status); 
         
}

/**
 * \brief  调度锁关闭，允许任务调度
 */
void rtos_task_sched_enable (void)
{
    uint32_t status = rtos_task_critical_entry();
    
    if (__schedlock_count > 0) {
        
        if (--__schedlock_count == 0) {
            
            /* 
             * 如果当前任务开启调度锁的时候，刚好可以执行任务调度，
             * 如果当前任务的优先级不为最高，则执行任务调度
             */
            rtos_task_sched();
        }
    }
        
    rtos_task_critical_exit(status);            
}

/**
 * \brief  查询调度锁状诚
 */
uint8_t rtos_task_schedlock_status (void)
{
    uint32_t status = rtos_task_critical_entry();
    
    uint8_t schedlock_status = 0;
    
    if (__schedlock_count > 0) {
        schedlock_status =  TASKSCHED_LOCK;
    } else {
        schedlock_status =  TASKSCHED_UNLOCK;
    }
        
    rtos_task_critical_exit(status); 

    return  schedlock_status;   
}

 /* end of file */
