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
 
 #include "rtos_init.h"
 
/**
 * \name RTOS 相关全局变量声明
 * @{
 */

/** \brief 任务优先级的就绪表定义变量 */
rtos_task_bitmap_t task_priobitmap = {0};

/** \brief  任务延时队列 */
rtos_task_list_t rtos_task_delayedlist;

/** \brief 当前任务：记录当前是哪个任务正在运行 */
rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
rtos_task_t * p_next_task;

/** \brief 空闲任务结构体 */
rtos_task_t idle_task;

/** \brief 空闲任务堆栈 */
taskstack_t idle_task_stack_buf[RTOS_IDLE_TASK_STACK_SIZE];

/** \brief 同一个优先级任务的链表头结点 */
rtos_task_list_t task_table[TASK_COUNT];

/** @} */

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
 * \brief rtos初始化函数
 */
int rtos_init (void)
{
    /* 初始化调度锁 */
    rtos_task_schedlock_init();
    
    /* 初始化任务任务就绪表位图数据 */
    rtos_task_bitmap_init(&task_priobitmap);
    
    /* 初始化优先级任务调度链表 */
    rtos_task_sched_init(task_table);
    
    /* 初始化任务延时队列 */    
    rtos_task_delayed_init(&rtos_task_delayedlist);
    
    /* 初始化定时器模块 */  
    rtos_timer_moudule_init(); 

    /* 空闲任务初始化 */
    rtos_task_init(&idle_task, idle_task_entry, NULL, RTOS_PRIO_COUNT - 1,  idle_task_stack_buf, sizeof(idle_task_stack_buf));

    return RTOS_OK;
}


/**
 * \brief rtos操作系统启动函数
 */
void rtos_start (void)
{
    /* 自动查找最高优先级的任务运行 */
    p_next_task =  rtos_task_highest_ready();
    
    /*  切换到p_next_task 指向的任务，这个函数永远不会返回 */
    rtos_task_run_first();	
}







/* end of file */

