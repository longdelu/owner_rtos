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
#include "rtos_task_switch.h"

#define   TASK_STACK_SIZE  1024

int g_task_flag1 = 0;

int g_task_flag2 = 0;

/** \brief 任务堆栈 */
taskstack_t run_task_stack_buf[TASK_STACK_SIZE];
taskstack_t next_task_stack_buf[TASK_STACK_SIZE];

/** \brief 当前任务结构体 */
rtos_task_t run_task;

/** \brief 下一任务结构体 */
rtos_task_t next_task;

/** \brief 当前任务：记录当前是哪个任务正在运行 */
rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
rtos_task_t * p_next_task;


/** \brief 所有任务的指针数组：简单起见，只使用两个任务 */
rtos_task_t * p_task_table[TASK_COUNT]; 


/**
 * \brief 当前任务入口函数
 */
void run_task_entry (void *p_arg)
{
          
     for (; ;) {
         
        *((uint32_t*) p_arg) = 1;
        rtos_mdelay(100); 
        *((uint32_t*) p_arg) = 0;
        rtos_mdelay(100);          
        rtos_task_sched();
         
     }
}

/**
 * \brief 下一个任务入口函数
 */
void next_task_entry (void *p_arg)
{    
    for (; ;) {
        
        *((uint32_t*) p_arg) = 1;
        rtos_mdelay(100); 
        *((uint32_t*) p_arg) = 0;
        rtos_mdelay(100); 
        rtos_task_sched();        
    }
}

/**
 * \brief 入口函数
 */
int main (void)
{    
   
    /* 系统节拍周期为1ms */
    rtos_systick_init(1);
    
    /* 组优先级有4位，次优先级也有4位 */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
    
    /* 任务初始化函数 */
    rtos_task_init(&run_task, run_task_entry, &g_task_flag1, 0,  run_task_stack_buf, sizeof(run_task_stack_buf)); 
    rtos_task_init(&next_task, next_task_entry, &g_task_flag2, 0,  next_task_stack_buf, sizeof(next_task_stack_buf));
    
    /* 初始化任务结构体列表 */
    p_task_table[0] = &run_task;
    p_task_table[1] = &next_task;
    
    /* 下一个运行的任务是run_task */
    p_next_task =  p_task_table[0];
    
    /*  切换到p_next_task 指向的任务，这个函数永远不会返回 */
    rtos_task_run_first();
      
    return 0;    
}




/* end of file */

