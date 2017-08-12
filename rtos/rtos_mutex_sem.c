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
 * \brief RTOS 互斥信号量实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_mutex_sem.h"

#if RTOS_ENABLE_MUTEX_SEM == 1
 
 /**
 * \brief 初始化互斥信号量
 */  
void rtos_mutex_sem_init (rtos_mutex_sem_t *p_mutex_sem)
{
    rtos_task_event_init(&p_mutex_sem->mutex_sem_event, RTOS_EVENT_TYPE_MUTEX_SEM);
    
    p_mutex_sem->p_task_ower = NULL;   

    p_mutex_sem->loced_count = 0;
    
    p_mutex_sem->task_ower_prior = RTOS_PRIO_COUNT;
}


/**
 * \brief 等待互斥信号量
 */  
int32_t rtos_mutex_sem_wait (rtos_mutex_sem_t *p_mutex_sem, uint32_t wait_ticks)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致p_current_task和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    if (p_mutex_sem->loced_count <= 0) {
        
        /* 如果没有锁定，则使用当前任务锁定 */
        p_mutex_sem->p_task_ower = p_current_task;
        p_mutex_sem->task_ower_prior =  p_current_task->prio;
        p_mutex_sem->loced_count++ ;
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 

        return RTOS_OK;  
        
    } else {
        
        /* 信号量已经被锁定,信号量拥有者任务再一次wait */
        if (p_mutex_sem->p_task_ower == p_current_task) {
            
            /* 如果是信号量的拥有者再次wait，简单增加计数 */
            p_mutex_sem->loced_count++; 
            
            /* 退出临界区 */
            rtos_task_critical_exit(status); 

            return RTOS_OK; 
            
        } else {
            
            /*
             * 如果是信号量拥有者之外的任务wait，则要检查下是否需要使用
             * 优先级继承方式处理
             */
            if (p_current_task->prio < p_mutex_sem->p_task_ower->prio) {
                
                /* 需要进行优先级反转，把当前任务的优先级提高到请求的高优先级任务的优先级一致 */
                
                rtos_task_t *p_tmp_task_ower = p_mutex_sem->p_task_ower;
                
                /*
                 * 如果当前任务的优先级比拥有者优先级更高，则使用优先级继承
                 * 提升原拥有者的优先
                 */
                if (p_tmp_task_ower->task_state & RTOS_TASK_STATE_REDDY) {
                    
                    /* 任务处于就绪状态时，更改任务在就绪表中的位置 */
                    rtos_task_sched_unready(p_tmp_task_ower);
                    
                    /* 改变了拥有者(任务的优先级 */
                    p_tmp_task_ower->prio =  p_current_task->prio;
                    
                    rtos_task_sched_ready(p_tmp_task_ower);
    
                    
                } else {
                
                    /* 其它状态，只需要修改优先级 */
                    p_tmp_task_ower->prio =  p_current_task->prio;
                      
                }
                
                        
            }
            
            /* 将当前任务放入事件等待队列中 */
            rtos_task_event_wait(p_current_task, &p_mutex_sem->mutex_sem_event, NULL, RTOS_EVENT_TYPE_MUTEX_SEM, wait_ticks); 
            
            /* 退出临界区 */
            rtos_task_critical_exit(status);

           /* 再执行一次事件调度，以便于切换到其它任务 */
            rtos_task_sched();  
            
            
           /* 当由于等待超时或者计数信号量执行rtos_mutex_sem_notify可用时，执行会返回到这里，然后取出等待结果 */      
           return  p_current_task->event_wait_result;
      
        }            
    }
}

/**
 * \brief 获取互斥信号量，如果互斥信号量不可用，则立即退回
 */  
int32_t rtos_mutex_sem_get (rtos_mutex_sem_t *p_mutex_sem)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致p_current_task和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    if (p_mutex_sem->loced_count <= 0) {
        
        /* 如果没有锁定，则使用当前任务锁定 */
        p_mutex_sem->p_task_ower = p_current_task;
        p_mutex_sem->task_ower_prior =  p_current_task->prio;
        p_mutex_sem->loced_count++;
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 

        return RTOS_OK;  
        
    } else {
        
        /* 信号量已经被锁定,信号量拥有者任务再一次wait */
        if (p_mutex_sem->p_task_ower == p_current_task) {
            
            /* 如果是信号量的拥有者再次wait，简单增加计数 */
            p_mutex_sem->loced_count++; 
            
            /* 退出临界区 */
            rtos_task_critical_exit(status); 

            return RTOS_OK; 
            
        } 
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 

        return -RTOS_UNUSED;   
    }        
}


/**
 * \brief 通知互斥信号量可用，唤醒等待队列中的一个任务,
 *        调用这个接口表明该任务占有些互斥信号量结束，其它任务可以获得这个互斥信号量的机会
 */  
int32_t rtos_mutex_sem_notify (rtos_mutex_sem_t *p_mutex_sem)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致p_current_task和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    if (p_mutex_sem->loced_count <= 0) {
        
        /* 锁定计数为0，信号量未被锁定，直接退出 */
        rtos_task_critical_exit(status);
        
        return RTOS_OK;
    }

    if (p_mutex_sem->p_task_ower != p_current_task) {
        
        /* 不是拥有者释放，认为是非法  */
        rtos_task_critical_exit(status);
        
        return RTOS_ERROR_OWER;
    }

    if (--p_mutex_sem->loced_count > 0) {
        
        /* 减1后计数仍不为0, 直接退出，不需要唤醒等待的任务 */
        rtos_task_critical_exit(status);
        
        return RTOS_OK;
    }

    /* 是否有发生优先级继承 */
    if (p_mutex_sem->task_ower_prior != p_mutex_sem->p_task_ower->prio) {
        
        /* 有发生优先级继承，恢复拥有者的优先级 */
        if (p_mutex_sem->p_task_ower->task_state & RTOS_TASK_STATE_REDDY) {
            
            /* 任务处于就绪状态时，更改任务在就绪表中的位置 */
            rtos_task_sched_unready(p_mutex_sem->p_task_ower);  
            
            /* 恢复优先级 */
            p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;           
            rtos_task_sched_ready(p_mutex_sem->p_task_ower);
            
        } else{
            /* 其它状态，只需要修改优先级 */
            p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;
        }
    }

    /* 检查是否有任务等待 */
    if (rtos_event_wait_count(&p_mutex_sem->mutex_sem_event) > 0) {
        
        /* 如果有的话，则直接唤醒位于队列首部（最先等待）的任务 */
        rtos_task_t * task = rtos_task_first_event_wake_up(&p_mutex_sem->mutex_sem_event, (void *)0, RTOS_OK);
        
        /* 记录是唤醒的这个任务占有互斥信号量 */
        p_mutex_sem->p_task_ower = task;
        p_mutex_sem->task_ower_prior = task->prio;
        p_mutex_sem->loced_count++;

        /* 如果这个任务的优先级更高，就执行调度，切换过去 */
        if (task->prio < p_current_task->prio) {
            
             rtos_task_sched();
        }
    }
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
    return RTOS_OK;   
}


/**
 * \brief查询互斥信号量的状态信息
 */  
void rtos_mutex_sem_info_get (rtos_mutex_sem_t *p_mutex_sem, rtos_mutex_sem_info_t *p_info)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致p_current_task和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    p_info->task_count = rtos_event_wait_count(&p_mutex_sem->mutex_sem_event);
    p_info->owner_task_prio = p_mutex_sem->task_ower_prior;
    
    if (p_mutex_sem->p_task_ower != NULL) {
        
        p_info->inherited_prio = p_mutex_sem->p_task_ower->prio;
        
    } else {
        
        p_info->inherited_prio = RTOS_PRIO_COUNT; 
    }
    
    p_info->p_task_ower = p_mutex_sem->p_task_ower;
    
    p_info->locked_count = p_mutex_sem->loced_count;

    /* 退出临界区 */
    rtos_task_critical_exit(status); 

    
}

/**       
 * \brief 销毁互斥信号量
 */  
uint32_t rtos_mutex_sem_destroy (rtos_mutex_sem_t *p_mutex_sem)
{
    uint32_t count = 0;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致p_current_task和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();

    /* 信号量是否已经被锁定，未锁定时没有任务等待，不必处理 */
    if (p_mutex_sem->loced_count > 0) {
       
        /* 是否有发生优先级继承 */
        if (p_mutex_sem->task_ower_prior != p_mutex_sem->p_task_ower->prio) {
            
            /* 有发生优先级继承，恢复拥有者的优先级 */
            if (p_mutex_sem->p_task_ower->task_state & RTOS_TASK_STATE_REDDY) {
                
                /* 任务处于就绪状态时，更改任务在就绪表中的位置 */
                rtos_task_sched_unready(p_mutex_sem->p_task_ower);  
                
                /* 恢复优先级 */
                p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;           
                rtos_task_sched_ready(p_mutex_sem->p_task_ower);
                
            } else{
                /* 其它状态，只需要修改优先级 */
                p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;
            }
        }
        
        /* 然后，清空事件控制块中的任务 */
        count = rtos_task_event_all_remove(&p_mutex_sem->mutex_sem_event, NULL, -RTOS_DEL);

        /* 清空过程中可能有更高优先级任务就绪，执行一次调度 */    
        if (count > 0) {
            
           rtos_task_sched();         
            
        }        
        
    }

    /* 退出临界区 */
    rtos_task_critical_exit(status); 

    return count;    
    
}

#endif
 
 
 
 
 
 /* end of file */



