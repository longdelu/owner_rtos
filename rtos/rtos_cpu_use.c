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
 * \brief RTOS 占有率测试
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_cpu_use.h"
#include "rtos_task_delay.h"
#include "rtos_task_critical.h"

/* \brief  时钟同步 */
static uint32_t rtos_enable_use_stat = 0; 

/* \brief  CPU占有率 */
static float  rtos_cpu_float_use  = 0.0f;
 
/* \brief  空闲任务计数与最大计数 */
static uint32_t idle_count = 0;
static uint32_t idle_max_count = 0; 

 
 
/**
 * \brief 为检查cpu使用率与系统时钟节拍同步
 */ 
void rtos_cpu_use_sync_with_systick (void)
{
    while (rtos_enable_use_stat == 0) {
       ;
    }
 
}

/**
 * \brief 空闲任务计数运行次数++
 */ 
void rtos_cpu_idle_count_inc (void )
{
    idle_count++; 
}
 
/**
 * \brief  初始化cpu统计
 */  
void rtos_cpu_use_init (void)
{
    idle_count = 0;
    idle_max_count = 0;
    
    rtos_cpu_float_use = 0;
    
    rtos_enable_use_stat = 0;
}

/**
 * \brief 检查cpu使用率
 */ 
void rtos_cpu_use_check (void)  
{
    /* 与空闲任务进行时钟同步 */
    if (rtos_enable_use_stat == 0) {
        rtos_enable_use_stat = 1;
        rtos_set_systick(0);  
        
        return;
    }
    
    if (rtos_get_systick() == RTOS_PER_SEC_SYSTICK_COUNT) {
        /* 统计最初1s内的最大计数值 */
        idle_max_count = idle_count;
        idle_count = 0;
        
        /* 计数完毕，开启调度器，允许切换到其它任务 */   
        rtos_task_sched_enable();
    } else if (rtos_get_systick() % RTOS_PER_SEC_SYSTICK_COUNT == 0) {
        
        /* 之后每隔1s统计一次，同时计算cpu利用率 */
        rtos_cpu_float_use = 100 - (idle_count * 100.0f / idle_max_count);
        idle_count = 0;

    }        
}

/**
 * \brief cpu使用率获取
 */ 
float rtos_cpu_use_get (void) 
{
    float cpu_use = 0.0f;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    cpu_use = rtos_cpu_float_use;
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
      
    return cpu_use;
}







 
 
 
 /* end of file */

