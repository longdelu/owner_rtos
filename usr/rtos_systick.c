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
 * \brief 操作系统滴答时钟
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "arm.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
#include "rtos_task_bitmap.h"


/** \brief 系统滴答计数值 */
uint32_t rtos_systick = 0;

  
/**
 * \brief  任务延时滴答处理
 */
static void __rtos_task_delay_tick_handler (void)
{
    volatile uint32_t i = 0;
       
    /*　临界区保护　*/
    uint32_t status = rtos_task_critical_entry();
    
      
    /* 检查所有任务的delayTicks数，如果不0的话，减1 */
    
    for (i = 0; i < TASK_COUNT; i++) {
        
        if (p_task_table[i]->delay_ticks > 0) {
            
            p_task_table[i]->delay_ticks--;
            
        } else {
            
            /* 使优先级就绪 */
            rtos_task_bitmap_set(&task_priobitmap, i);            
            
        }
        
    }

    /* 退出临界区保护 */
    rtos_task_critical_exit(status);    
}
 
/**
 * \brief  操作系统滴答时钟初始化函数
 */
void rtos_systick_init (uint32_t ms)
{
    SysTick->LOAD  = ms * SystemCoreClock / 1000 - 1; 
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    SysTick->VAL   = 0;                           
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk; 
}

/**
 * \brief  操作系统滴答中断处理函数
 *
 * \param[in] 无 
 *
 * \return    无
 */
void SysTick_Handler (void) 
{
    /*　临界区保户　*/
    uint32_t status = rtos_task_critical_entry();
    
    /* 滴答计数 */
    rtos_systick++;
        
    __rtos_task_delay_tick_handler();
    
    /* 退出临界区保护 */
    rtos_task_critical_exit(status);
            
    /* 这个过程中可能有任务延时完毕(delayTicks = 0)，进行一次调度 */
    rtos_task_sched(); 
}


 
 
 /* end of file */

