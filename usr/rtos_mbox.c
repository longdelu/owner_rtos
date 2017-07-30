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
 * \brief RTOS 消息邮箱实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_mbox.h"
 
 /**
 * \brief 初始化消息邮箱
 */  
void rtos_mbox_init(rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t msg_max_count)
{
    rtos_task_event_init(&p_mbox->mbox_event, RTOS_EVENT_TYPE_MBOX);
    p_mbox->p_msg_buf   = p_msg_buf;
    p_mbox->write_index = 0;
    p_mbox->read_index  = 0;
    p_mbox->msg_cur_count  = 0;
    p_mbox->msg_max_count =  msg_max_count;       
                 
}


/**
 * \brief 等待邮箱, 获取一则消息
 */ 
int32_t rtos_mbox_wait (rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t wait_ticks)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* 首先检查消息计数是否大于0 */ 
    if (p_mbox->msg_cur_count > 0) {
        
        /* 大于0的话，取出一个消息 */
        --p_mbox->msg_cur_count;
        *p_msg_buf = p_mbox->p_msg_buf[p_mbox->read_index++];
        
        /* 同时读取索引前移，如果超出边界则回绕 */
        if (p_mbox->read_index >= p_mbox->msg_max_count) {
            p_mbox->read_index = 0;
        }
        
        /* 退出临界区 */
        rtos_task_critical_exit(status);   
        
        return RTOS_OK;
        
    } else {
        
        /*　将任务插入事件链表 */
        rtos_task_event_wait(p_current_task, &p_mbox->mbox_event, NULL, RTOS_EVENT_TYPE_MBOX, wait_ticks);
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 
        
        /* 最后再执行一次事件调度，以便于切换到其它任务 */
        rtos_task_sched();   

       /* 当切换回来时，从tTask中取出获得的消息 */
       *p_msg_buf = p_current_task->p_event_msg;
       
       /* 取出等待结果 */        
       return  p_current_task->event_wait_result;
               
    }
    
}

/**
 * \brief  获取一则消息，如果没有消息，则立即退回
 */ 
int32_t rtos_mbox_nowait_get (rtos_mbox_t *p_mbox, void **p_msg_buf)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* 首先检查消息计数是否大于0 */ 
    if (p_mbox->msg_cur_count > 0) {
        
        /* 大于0的话，取出一个消息 */
        --p_mbox->msg_cur_count;
        *p_msg_buf = p_mbox->p_msg_buf[p_mbox->read_index++];
        
        /* 同时读取索引前移，如果超出边界则回绕 */
        if (p_mbox->read_index >= p_mbox->msg_max_count) {
            p_mbox->read_index = 0;
        }
        
        /* 退出临界区 */
        rtos_task_critical_exit(status);   
        
        return RTOS_OK;
        
    } else {
        
        /* 退出临界区 */
        rtos_task_critical_exit(status);  
       
       /* 资源不可用 */        
       return  -RTOS_UNUSED;
               
    }
}


/**
 * \brief  通知消息可用，唤醒等待队列中的一个任务，或者将消息插入到邮箱中
 */ 
int32_t  rtos_mbox_notify (rtos_mbox_t *p_mbox, void *p_msg, uint32_t notify_option)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
    
    /* 检查是否有任务等待 */
    if (rtos_event_wait_count(&p_mbox->mbox_event) > 0){
        /* 如果有的话，则直接唤醒位于队列首部（最先等待）的任务　*/
        rtos_task_t * task = rtos_task_event_wake_up(&p_mbox->mbox_event, (void *)p_msg, RTOS_OK );

        /* 如果这个任务的优先级更高，就执行调度，切换过去 */
        if (task->prio < p_current_task->prio) {
             rtos_task_sched(); 
        }
    } else {
        
        /* 如果没有任务等待的话，将消息插入到缓冲区中 */
        if (p_mbox->msg_cur_count >= p_mbox->msg_max_count) {
            
            /* 退出临界区 */
            rtos_task_critical_exit(status); 
            
            /* 消息缓冲区已经满了*/
            return -RTOS_FULL;
            
        }
        
        /* 可以选择将消息插入到头，这样后面任务获取的消息的时候，优先获取该消息 */ 
        if (notify_option == RTOS_MBOX_SENDFRONT) {
            
            if (p_mbox->read_index <=0) {
                p_mbox->read_index = p_mbox->msg_max_count - 1;
            } else {
                 --p_mbox->read_index;
            }
            
            p_mbox->p_msg_buf[p_mbox->read_index] = p_msg;    
        } else {
            p_mbox->p_msg_buf[p_mbox->write_index++] = p_msg;
            
            if (p_mbox->write_index >= p_mbox->msg_max_count) {
                p_mbox->write_index = 0;
            }                
        }
        
        /* 消息计数加1 */           
         ++p_mbox->msg_cur_count ;
   
                  
    }
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
    return RTOS_OK;   
}


/**
 * \brief   清空邮箱中所有消息
 */ 
void rtos_mbox_flush (rtos_mbox_t *p_mbox)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
    
    /* 如果事件队列中有任务等待，说明邮箱已经是空的了，不需要再清空 */
    if (rtos_event_wait_count(&p_mbox->mbox_event) == 0){
        p_mbox->read_index  = 0;
        p_mbox->write_index = 0;
        p_mbox->msg_cur_count = 0;      
    }
        
    /* 退出临界区 */
    rtos_task_critical_exit(status);     
}

/**
 * \brief   销毁邮箱
 */ 
uint32_t rtos_mbox_destroy (rtos_mbox_t *p_mbox)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    /* 清空事件控制块中的任务 */
    uint32_t count  = rtos_task_event_all_remove(&p_mbox->mbox_event, NULL, -RTOS_DEL);  

    p_mbox->msg_cur_count = 0;    
    
    /* 退出临界区 */
    rtos_task_critical_exit(status);  
    
    /* 清空过程中可能有更高优先级任务就绪，执行一次调度 */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
    
    return count;    
}

/**********************************************************************************************************
** Function name        :   tMboxGetInfo
** Descriptions         :   查询状态信息
** parameters           :   mbox 查询的邮箱
** parameters           :   info 状态查询存储的位置
** Returned value       :   无
***********************************************************************************************************/
void rtos_mbox_info_get (rtos_mbox_t *p_mbox, rtos_mbox_info_t *p_info)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    /* 拷贝需要的信息 */
    p_info->msg_cur_count = p_mbox->msg_cur_count;
    p_info->max_msg_count = p_mbox->msg_max_count;
    p_info->task_count = rtos_event_wait_count(&p_mbox->mbox_event);

    /* 退出临界区 */
    rtos_task_critical_exit(status);  
}

 
 
 
  /* end of file */


