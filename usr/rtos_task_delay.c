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
 * \brief 操作系统普通延时函数
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "c_lib.h"
 #include "rtos_task.h"
 
extern  uint32_t rtos_systick;
 
/**
 * \brief  返回系统当前滴答计数
 */ 
uint32_t rtos_get_systick(void) 
{ 
     return rtos_systick; 
}

/**
 * \brief  操作系统普通延时，没有任务调度, 以10ms为一个时基单位
 */
void rtos_mdelay (uint32_t ms) { 
    
  uint32_t ms_end = rtos_systick + ms;
    
  while ((ms_end - rtos_systick) > 0) {
      ;
  }
}

/**
 * \brief  操作系延时，存在任务调度, 以10ms为一个时基单位
 */
void rtos_sched_mdelay ( uint32_t ms)
{
    p_current_task->delay_ticks = ms;

    /*
     * 然后进行任务切换，切换至另一个任务，或者空闲任务
     * delay_tikcs会在时钟中断中自动减1.当减至0时，会切换回来继续运行
     */
    rtos_task_sched();              
}
    
    
 
 
 /* end of file */

