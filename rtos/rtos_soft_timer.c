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
 * \brief RTOS 软件定时器原理实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#include "rtos_soft_timer.h"
#include "rtos_sem.h"

#if RTOS_ENABLE_TIMER == 1

static  rtos_task_list_t __rtos_timer_hard_list;
static  rtos_task_list_t __rtos_timer_soft_list;

/** \brief 用于访问软定时器列表的信号量 */
static rtos_sem_t __rtos_softtimer_sem;

/** \brief 用于访问软定时器列表的信号量 */
static rtos_sem_t __rtos_timer_tick_sem;


/**
 * \brief 初始化定时器
 */
void rtos_timer_init (rtos_timer_t * p_timer, 
                      uint32_t start_delay_ticks, 
                      uint32_t duration_ticks,
                      void (*pfn_timer_func)(void *p_arg), 
                      void *p_arg, 
                      uint32_t config)
{
    
    dlist_init(&p_timer->timer_node);
    p_timer->start_delay_ticks = start_delay_ticks;
    p_timer->duration_ticks =  duration_ticks;
    
    p_timer->pfn_timer_func = pfn_timer_func;
    p_timer->p_arg          = p_arg;
    p_timer->config         = config;
    
    if (start_delay_ticks == 0) {
        p_timer->delay_ticks = duration_ticks;
             
    } else {
        p_timer->delay_ticks = start_delay_ticks;
    }
    
    
    p_timer->state = RTOS_TIMER_CREATED;
     
}

/**
 * \brief 启动定时器
 */
void rtos_timer_start (rtos_timer_t * p_timer)
{
    uint32_t status  = 0;
    
    switch ((int32_t)p_timer->state) {
    case RTOS_TIMER_CREATED:
    case RTOS_TIMER_STOPPED:
        p_timer->delay_ticks = (p_timer->start_delay_ticks > 0) ? p_timer->start_delay_ticks :p_timer->duration_ticks;
        p_timer->state = RTOS_TIMER_STARTED;
    
        /* 根据定时器类型加入相应的定时器列表 */
        if (p_timer->config & TIMER_CONFIG_TYPE_HARD) {
           
           /* 硬定时器，在时钟节拍中断中处理，所以使用critical来防护*/    
           status = rtos_task_critical_entry();

           /* 加入到硬件定时器列表 */
           rtos_task_list_add_tail(&__rtos_timer_hard_list, &p_timer->timer_node); 

           /* 退出临界区 */
           rtos_task_critical_exit(status);           
   
        } else {
            
            /* 
             * 获取软定时器列表的访问权限
             * 软定时器，先获取信号量。以处理此时定时器任务此时同时在访问软定时器列表导致的冲突问题
             */ 
            rtos_sem_wait(&__rtos_softtimer_sem, 0);
            
            /* 处理软定时器列表 */ 
            rtos_task_list_add_tail(&__rtos_timer_soft_list, &p_timer->timer_node);
            
            /* 释放定时器列表访问权限 */ 
            rtos_sem_notify(&__rtos_softtimer_sem);            
           
        }
       
        break;
        
    default:
        break;    
        
    }
}


/**
 * \brief 启动定时器
 */
void rtos_timer_stop (rtos_timer_t * p_timer)
{
    uint32_t status  = 0;
    
    switch ((int32_t)p_timer->state) {
    case RTOS_TIMER_RUNNING:
    case RTOS_TIMER_STARTED:
    
        /* 如果已经启动，判断定时器类型，然后从相应的延时列表中移除 */
        if (p_timer->config & TIMER_CONFIG_TYPE_HARD) {
           
           /* 硬定时器，在时钟节拍中断中处理，所以使用critical来防护*/    
           status = rtos_task_critical_entry();

           /* 从硬定时器列表中移除 */
           rtos_task_list_remove(&__rtos_timer_hard_list, &p_timer->timer_node); 

           /* 退出临界区 */
           rtos_task_critical_exit(status);           
   
        } else {
            
            /* 
             * 获取软定时器列表的访问权限
             * 软定时器，先获取信号量。以处理此时定时器任务此时同时在访问软定时器列表导致的冲突问题
             */ 
            rtos_sem_wait(&__rtos_softtimer_sem, 0);
            
            /* 处理软定时器列表 */ 
            rtos_task_list_remove(&__rtos_timer_soft_list, &p_timer->timer_node);
            
            /* 释放定时器列表访问权限 */ 
            rtos_sem_notify(&__rtos_softtimer_sem);            
           
        }
        
        p_timer->state = RTOS_TIMER_STOPPED;
       
        break;
        
    default:
        break;    
        
    }
}

/**
 * \brief 销毁定时器
 */
void rtso_timer_destroy (rtos_timer_t * p_timer)    
{   
    /* 先停止定时器 */
    rtos_timer_stop(p_timer);
   
    p_timer->state = RTOS_TIMER_DESTROYED;
 
}

/**
 * \brief 查询定时器状态信息
 */
void rtos_timer_info_get (rtos_timer_t *p_timer, rtos_timer_info_get_t *p_info)    
{
    uint32_t status = 0; 
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
     status = rtos_task_critical_entry(); 
    
     p_info->config  =  p_timer->config;
     p_info->start_delay_ticks = p_timer->start_delay_ticks;
     p_info->duration_ticks =  p_timer->duration_ticks;
     p_info->p_arg = p_timer->p_arg;
     p_info->pfn_timer_func = p_timer->pfn_timer_func;
     p_info->state = p_timer->state;
    
    
     /* 退出临界区 */
     rtos_task_critical_exit(status); 
 
}




/** \brief 任务堆栈 */
taskstack_t __rtos_timer_task_stack_buf[TIMER_TASK_STACK_SIZE];

/** \brief 当前任务结构体 */
rtos_task_t __rtos_timer_task_task;


/**
 * \brief 定时器链表处理函数
 */
static void rtos_timersoft_list_call_fuc (rtos_task_list_t *p_softtimer_list) 
{
    dlist_node_t *p_timer_node = NULL;
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 
    
    rtos_timer_t * p_softtimer = NULL;
    
        /* 获得延时队列第一个用户结点 */
    p_next  = dlist_begin_get(&p_softtimer_list->head_node);
    
    /*  获得延时队列的结束位置: 为头结点本身 */
    p_end  = dlist_end_get(&p_softtimer_list->head_node);
    
    

    /* 检查所有定时器的delay_ticks数，如果不0的话，减1。*/
    while(p_next != p_end) {   

        /* 先纪录下当前结点 */
        p_timer_node =  p_next;
           
        /* 先记录下一个结点的信息 */
        p_next = dlist_next_get(&p_softtimer_list->head_node, p_next);
        
           
        p_softtimer = RTOS_CONTAINER_OF(p_timer_node, rtos_timer_t, timer_node);
        
        /* 如果延时已到，则调用定时器处理函数 */
        if ((p_softtimer->delay_ticks == 0) || (--p_softtimer->delay_ticks == 0)) {
            
            /* 切换为正在运行状态 */
            p_softtimer->state = RTOS_TIMER_RUNNING;
            
            /* 调用定时器处理函数 */
            p_softtimer->pfn_timer_func(p_softtimer->p_arg);
            
            /* 切换为已经启动状态 */
            p_softtimer->state = RTOS_TIMER_STARTED;
            
            
            if (p_softtimer->duration_ticks > 0) {
                
                /* 如果是周期性的，则重复延时计数值 */
                p_softtimer->delay_ticks = p_softtimer->duration_ticks;
            } else {
                /* 否则，是一次性计数器，中止定时器 */
                rtos_task_list_remove(p_softtimer_list, &p_softtimer->timer_node);
                p_softtimer->state = RTOS_TIMER_STOPPED;
            }
            
        }

        
    }
    
}

/**
 * \brief 软件定时器任务入口函数
 */
static void rtos_timersoft_task (void *p_parg) 
{
    
    for (; ;) {
        
        /* 等待系统节拍发送的中断事件信号 */  
        rtos_sem_wait(&__rtos_timer_tick_sem, 0);
        
        /* 获取软定时器列表的访问权限 */ 
        rtos_sem_wait(&__rtos_softtimer_sem, 0);
        
        /* 处理软定时器列表 */ 
        rtos_timersoft_list_call_fuc(&__rtos_timer_soft_list);
        
        /* 释放定时器列表访问权限 */ 
        rtos_sem_notify(&__rtos_softtimer_sem);
        
            
    }
    
}

/**
 * \brief 通知系统滴答时钟已经发生了，因为它是在硬件中断处理函数中调用的，也可以在里面调用硬件定时器的处理
 */
void rtos_timer_moudule_tick_notify (void)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    rtos_timersoft_list_call_fuc(&__rtos_timer_hard_list);
    
    /* 退出临界区 */
    rtos_task_critical_exit(status);   

    /* 通知软件定时器节拍变化 */ 
    rtos_sem_notify(&__rtos_timer_tick_sem);
     
}

/**
 * \brief 定时器模块初始化函数
 */
void  rtos_timer_moudule_init (void) 
{
    /* 定时器列表初始化 */
    rtos_task_list_init(&__rtos_timer_soft_list);
    rtos_task_list_init(&__rtos_timer_hard_list);  

    
    /* 相关信号量初始化 */    
    rtos_sem_init(&__rtos_timer_tick_sem, 0, 0);
    rtos_sem_init(&__rtos_softtimer_sem, 1, 1);    
}

/**
 * \brief 初始化软定时器任务
 */
void rtos_timer_task_init (void)
{    
#if RTOS_TIMER_TASK_PRIO >= (RTOS_PRIO_COUNT - 1)
    #error "The proprity of timer task must be greater then (RTOS_PRIO_COUNT - 1)"
#endif
     
    /* 定时器任务初始化函数 */
    rtos_task_init(&__rtos_timer_task_task, rtos_timersoft_task, NULL, RTOS_TIMER_TASK_PRIO, __rtos_timer_task_stack_buf, sizeof(__rtos_timer_task_stack_buf)); 
       
}

#endif

/* end of file */




