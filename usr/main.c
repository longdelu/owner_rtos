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
#include "rtos_task_critical.h"
#include "rtos_task_delay.h"
#include "rtos_task_switch.h"
#include "rtos_task_bitmap.h"

#define   TASK_STACK_SIZE  1024


int g_task_flag1 = 0;

int g_task_flag2 = 0;

/** \brief 任务优先级的标记位置结构全变量 */
rtos_task_bitmap_t task_priobitmap = {0};

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


/** \brief 空闲任务结构体 */
rtos_task_t idle_task;

/** \brief 空闲任务堆栈 */
taskstack_t idle_task_stack_buf[TASK_STACK_SIZE];

/** \brief  空闲任务结构体指针 */
rtos_task_t * p_idle_task;

/**
 * \brief 空闲任务入口函数
 */
void idle_task_entry (void *p_arg)
{
    for (; ;) {
        /* 空闲任务暂时什么都不做，它可以被用户任务抢占 */
        
    }
}



/**
 * \brief 当前任务入口函数
 * \note  系统节拍初始化函数要等任务初始化函数完成后才能调用
 */
void run_task_entry (void *p_arg)
{    
     /* 系统节拍周期为10ms */
     rtos_systick_init(10); 
    
     for (; ;) {
         
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0;
        rtos_sched_mdelay(10);                  
     }
}

/**
 * \brief 下一个任务入口函数
 */
void next_task_entry (void *p_arg)
{    
    for (; ;) {
        
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(10);       
    }
}

/**
 * \brief 入口函数
 */
int main (void)
{    
      
    /* 组优先级有4位，次优先级也有4位 */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
    
    /* 初始化调度器 */
    rtos_task_schedlock_init();
    
    /* 初始化任务位图数据结构体变量 */
    rtos_task_bitmap_init(&task_priobitmap);
    
    /* 任务初始化函数 */
    rtos_task_init(&run_task, run_task_entry, &g_task_flag1, 0,  run_task_stack_buf, sizeof(run_task_stack_buf)); 
    rtos_task_init(&next_task, next_task_entry, &g_task_flag2, 1,  next_task_stack_buf, sizeof(next_task_stack_buf));
        
    /* 空闲任务初始化 */
    rtos_task_init(&idle_task, idle_task_entry, NULL, RTOS_PRIO_COUNT - 1,  idle_task_stack_buf, sizeof(idle_task_stack_buf));

#if 0    
    
    /* 初始化任务结构体列表 */
    p_task_table[0] = &run_task;
    p_task_table[1] = &next_task;  
    p_idle_task = &idle_task;     
    p_next_task = p_task_table[0];
    
#endif
    
    /* 自动查找最高优先级的任务运行 */
    p_next_task =  rtos_task_highest_ready();
    
    /*  切换到p_next_task 指向的任务，这个函数永远不会返回 */
    rtos_task_run_first();
      
    return 0;    
}




/* end of file */

