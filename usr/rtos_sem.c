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
 * \brief RTOS 计数信号量相关实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_sem.h"
 #include "rtos_task.h"
 
 /**
 * \brief 初始化计数信号量，start_count表明该信号量一开始就有几个计数信号量可用
 */  
void rtos_sem_init(rtos_sem_t *p_sem, uint32_t start_count, uint32_t max_count)
{
    rtos_task_event_init(&p_sem->sem_event, RTOS_EVENT_TYPE_SEM);
    
    if (max_count == 0) {
        
        p_sem->sem_count =  start_count;
        
    } else {
        
        p_sem->sem_count =  (start_count > max_count) ? max_count : start_count;       
    }
                 
}


/**
 * \brief 等待计数信号量
 */  
int32_t rtos_sem_wait (rtos_sem_t *p_sem, uint32_t wait_ticks)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    if (p_sem->sem_count > 0) {
        
        --p_sem->sem_count;
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 

        return RTOS_OK;     
        
    } else {
        
        rtos_task_event_wait(p_current_task, &p_sem->sem_event, NULL, RTOS_EVENT_TYPE_SEM, wait_ticks); 
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 
        
        /* 最后再执行一次事件调度，以便于切换到其它任务 */
        rtos_task_sched(); 
        
        /* 当由于等待超时或者计数可用时，执行会返回到这里，然后取出等待结果 */
        
        return  p_current_task->event_wait_result;
    } 
}

/**
 * \brief 获取信号量，如果信号量计数不可用，则立即退回
 */  
int32_t rtos_sem_get (rtos_sem_t *p_sem)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    
    if (p_sem->sem_count > 0) {
        
        --p_sem->sem_count;
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 

        return RTOS_OK; 
        
    }  else {
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 

        return -RTOS_UNUSED;           
    }  
}


/**
 * \brief 通知信号量可用，唤醒等待队列中的一个任务，或者将计数+1
 */  
void rtos_sem_notify (rtos_sem_t *p_sem)
{
     /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* 检查是否有任务等待 */
    if (rtos_event_wait_count(&p_sem->sem_event) > 0){
        /* 如果有的话，则直接唤醒位于队列首部（最先等待）的任务　*/
        rtos_task_t * task = rtos_task_event_wake_up(&p_sem->sem_event, (void *)0, RTOS_OK );

        /* 如果这个任务的优先级更高，就执行调度，切换过去 */
        if (task->prio < p_current_task->prio) {
             rtos_task_sched(); 
        }
    } else {
        /* 如果没有任务等待的话，增加计数 */
        if ((uint64_t)p_sem->sem_count < 0xFFFFFFFF) {
            
            ++p_sem->sem_count;
        } else {
            p_sem->sem_count = 0xFFFFFFFF;
        }

        // 如果这个计数超过了最大允许的计数，则递减
        if ((p_sem->max_count != 0) && (p_sem->sem_count > p_sem->max_count)) {    
            p_sem->sem_count = p_sem->max_count;
        }
                  
    }
        
    /* 退出临界区 */
    rtos_task_critical_exit(status);    
}

/**
 * \brief查询信号量的状态信息
 */  
void rtos_sem_info_get (rtos_sem_t * p_sem, rtos_sem_info_t *p_info)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    p_info->count      =  p_sem->sem_count;
    p_info->max_count  =  p_sem->max_count;  

    /* 待待该信号量的任务数量 */    
    p_info->task_count =  rtos_event_wait_count(&p_sem->sem_event);
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
}

/**       
 * \brief 销毁信号量
 */  
uint32_t rtos_sem_destroy (rtos_sem_t *p_sem)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    /* 清空事件控制块中的任务 */
    uint32_t count  = rtos_task_event_all_remove(&p_sem->sem_event, NULL, -RTOS_DEL);  

    p_sem->sem_count = 0;    
    
    /* 退出临界区 */
    rtos_task_critical_exit(status);  
    
    /* 清空过程中可能有更高优先级任务就绪，执行一次调度 */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
    
    return count;
    
    
}
 
 
 
 
 
 
 
 /* end of file */

