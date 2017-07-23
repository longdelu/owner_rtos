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
 * \brief RTOS 任务事件控制块相关实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_task_event.h"
 #include "rtos_task.h"
 #include "rtos_task_critical.h"
 
 /**
 * \brief 初始化RTOS任务事件控制块
 */  
void rtos_task_event_init (rtos_task_event_t *p_event, rtos_task_event_type_t event_type)
{
    p_event->event_type = event_type;

    rtos_task_list_init(&p_event->event_wait_list);   
    
}


/**
 * \brief 让RTOS任务指定在事件控制块上等待事件发生
 */  
void rtos_task_event_wait (rtos_task_t *p_task, 
                           rtos_task_event_t *p_event, 
                           void *p_event_msg, 
                           uint32_t state, 
                           uint32_t time_out) 
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    
    p_task->task_state       |= state;         /* 标记任务处于等待某种事件的状态 */ 
    p_task->p_event           = p_event;       /* 设置任务等待的事件结构         */
    p_task->p_event_msg       = p_event_msg;   /* 设置任务等待事件的消息存储位置 */   
    p_task->event_wait_result = RTOS_OK;       /* 清空事件的等待结果             */  
    
    /* 将任务从就绪队列中移除 */
    rtos_task_sched_unready(p_task);
    
    /* 将任务插入到等待事件队列中 */
    rtos_task_list_add_tail(&p_event->event_wait_list, &p_task->event_node);

    /*
     * 如果发现有设置超时，在同时插入到延时队列中
     * 当时间到达时，由延时处理机制负责将任务从延时列表中移除，同时从事件列表中移除
     */
    if (time_out > 0) {
        
        /* 将任务加入到延时队列 */
        rtos_task_add_delayed_list(p_task, time_out);
    }

    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
}    
    
/**
 * \brief 从指定事件控制块上唤醒首个等待的RTOS任务，
 */  
rtos_task_t  *rtos_task_event_wake_up (rtos_task_event_t *p_event, void *p_event_msg, int32_t event_result) 
{   
    dlist_node_t *p_event_node = NULL;
    
    rtos_task_t  *p_task = NULL;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();


    /* 取出该事件控制块等待队列中的第一个结点 */
    if ((p_event_node = rtos_task_list_remove_first(&p_event->event_wait_list)) != NULL) {
        
         /* 转换成对应的任务结构 */
         p_task = RTOS_CONTAINER_OF(p_event_node, rtos_task_t, event_node);
        
         p_task->task_state       &= ~RTOS_TASK_EVENT_WAIT_MASK;         /* 标记任务未处于等待某种事件的状态 */ 
         p_task->p_event           = NULL;                               /* 设置任务等待的事件结构         */
         p_task->p_event_msg       = p_event_msg;                        /* 设置任务等待事件的消息存储位置 */   
         p_task->event_wait_result = event_result;                       /* 设置事件的等待结果             */  
        
        /* 任务申请了超时等待，这里检查下，将其从延时队列中移除 */
        if (p_task->delay_ticks != 0) {
            
            rtos_task_wake_up_delayed_list(p_task);
            
            /* todo: 可以加入唤醒超时等待的事件上任务，并使其delay_ticks为0 */
        }
        
        /* 将任务加入就绪队列 */
        rtos_task_sched_ready(p_task) ;

    }        


    /* 退出临界区 */
    rtos_task_critical_exit(status);    

   return p_task;    
    

}

/**
 * \brief 从指定事件控制块上删除等待的RTOS任务，
 */  
void rtos_task_event_del (rtos_task_t  *p_task, void *p_event_msg, int32_t event_result) 
{   
   
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
    
    /*
     * 将任务从所在的等待队列中移除。注意，这里没有检查waitEvent是否为空。既然是从事件中移除，那么认为就不可能为空 
     */
    rtos_task_list_remove(&p_task->p_event->event_wait_list, &p_task->event_node); 


    {
        
         p_task->task_state       &= ~RTOS_TASK_EVENT_WAIT_MASK;         /* 标记任务未处于等待某种事件的状态 */ 
         p_task->p_event           = NULL;                               /* 设置任务等待的事件结构         */
         p_task->p_event_msg       = p_event_msg;                        /* 设置任务等待事件的消息存储位置 */   
         p_task->event_wait_result = event_result;                       /* 设置事件的等待结果             */  
        
        
        /* 不能在这里将任务加入就绪队列 */
      //  rtos_task_sched_ready(p_task) ;

    }        


    /* 退出临界区 */
    rtos_task_critical_exit(status);    
    

}  
    
    
 
 
 
 /* end of file */


