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
 * \brief main函数入口
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "arm.h"
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_delay.h"

int task_flag1 = 0;

uint32_t task_stack_buf[1024];

rtos_task_t run_task;

/** \brief 当前任务：记录当前是哪个任务正在运行 */
rtos_task_t * currentTask;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
rtos_task_t * nextTask;

void PendSVC_Trigger(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}

int main (void)
{    
    
    /* 系统节拍周期为1ms */
    rtos_systick_init(10);
    
//    /* 组优先级有4位，次优先级也有4位 */
//    NVIC_SetPriorityGrouping(0x03);
//    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
    
    
    run_task.task_stack = &task_stack_buf[1024];
    
    currentTask = &run_task;
    

    
    
    while(1) {
        
        task_flag1 = 0;
        
        /* 触发PendSVC异常中断 */
        PendSVC_Trigger();

        rtos_mdelay(100);
        
        task_flag1 = 1;
        
        /* 触发PendSVC异常中断 */
        PendSVC_Trigger();

        rtos_mdelay(100);        
               
    }
    
    
    return 0;    
}




/* end of file */

