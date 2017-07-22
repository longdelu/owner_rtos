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
#include "rtos_config.h"
#include "rtos_task_event.h"


/** \brief  任务延时队列 */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief 同一个优先级任务的链表头结点 */
extern rtos_task_list_t task_table[TASK_COUNT];


/** \brief 系统滴答计数值 */
uint32_t rtos_systick = 0;

  
/**
 * \brief  任务延时滴答处理
 */
static void __rtos_task_delay_tick_handler (void)
{
    volatile uint32_t i = 0;
    
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 
    
     rtos_task_t * p_task  = NULL;
       
    /*　临界区保护　*/
    uint32_t status = rtos_task_critical_entry();
       
    /* 获得第一个用户结点 */
    p_next  = dlist_begin_get(&rtos_task_delayedlist.head_node);
    
    /* 结束位置为头结点本身 */
    p_end  = dlist_end_get(&rtos_task_delayedlist.head_node);
    
    
     /* 检查所有任务延时列表时面所有任务的delayTicks数，如果不0的话，减1 */
    while(p_next != p_end) {   

        /* 先纪录下当前结点 */
        p_tmp =  p_next;
           
        /* 先记录下一个结点的信息 */
        p_next = dlist_next_get(&rtos_task_delayedlist.head_node, p_next);
        
        
        p_task = RTOS_CONTAINER_OF(p_tmp, rtos_task_t, delay_node);
        
        if (--p_task->delay_ticks == 0) {
            
             /* 如果任务还处于等待事件的状态，则将其从事件等待队列中唤醒 */
            if (p_task->p_event) {
                
                /* 此时，消息为空，等待结果为超时 */
                rtos_task_event_del(p_task, NULL, -RTOS_TIMEOUT);
                
            }
            
            /* 将任务从延时队列中移除 */
            rtos_task_wake_up_delayed_list(p_task);
            
            /* 将任务登记到就绪列表中 */
            rtos_task_sched_ready(p_task);
        }
        
              
    }
    
    /* 检查下当前任务的时间片是否已经到了 */
    if (--p_current_task->slice == 0)
    {
        /*
         * 如果当前任务中还有其它任务的话，那么切换到下一个任务
         * 方法是将当前任务从队列的头部移除，插入到尾部
         * 这样后面执rtos_task_sched()时就会从头部取出新的任务取出新的任务作为当前任务运行
         */
        if (rtos_task_list_count(&task_table[p_current_task->prio]) > 0)
        {
            rtos_task_list_remove_first(&task_table[p_current_task->prio]);
            rtos_task_list_add_tail(&task_table[p_current_task->prio], &(p_current_task->prio_node));

            /*  重置计数器 */
            p_current_task->slice = RTOS_SLICE_MAX;
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

