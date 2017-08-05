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
 
 #include "rtos_flaggroup.h"
 
 
/**
 * \brief  辅助函数。检查并消耗掉事件标志
 * \param[in] p_flag_grp: 指向计数事件标记组结构体的指针
 * \param[in] wait_type :  等待的事件类型，值为 RTOS_FLAG_GRP_ANY_* 这一类宏 (# RTOS_FLAG_GRP_ANY_CLR)
 * \param     p_req_flag:    待检查事件标志存储地址和检查结果存储位置,即是输入也是输出
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK
 */   
static int32_t __rtos_rtos_flag_grp_check_and_consum( rtos_flag_grp_t *p_flag_grp, uint32_t wait_type, uint32_t *p_req_flag)
{
    uint32_t src_flags = *p_req_flag;
    uint32_t is_set = wait_type & RTOS_FALG_GRP_SET;
    uint32_t is_all = wait_type & RTOS_FALG_GRP_ALL;
    uint32_t is_consume = wait_type & RTOS_FLAG_GRP_CONSUME;

    /*
     *  有哪些类型的标志位出现
     *  flagGroup->flags & flags：计算出事件标记组中哪些位已经为1
     *  flagGroup->flags & flags: 计算出事件标记组中哪些位已经为0
     */
    uint32_t calc_flag = is_set ? (p_flag_grp->flag & src_flags) : (~p_flag_grp->flag & src_flags);

    /* 所有标志位出现, 或者做任意标志位出现，满足条件 */
    if (((is_all != 0) && (calc_flag == src_flags)) || ((is_all == 0) && (calc_flag != 0))) {
        
        /* 是否消耗掉标志位 */
        if (is_consume) {
            
            if (is_set) {
                /* 清除为1的标志位，变成0 */
                p_flag_grp->flag &= ~src_flags;
            } else {
                /* 清除为0的标志位，变成1 */
                p_flag_grp->flag |= src_flags;
            }
        }
        
        *p_req_flag = calc_flag;
        
        return RTOS_OK;
    }

    *p_req_flag = src_flags;
    
    return -RTOS_UNUSED;    
    
 
}
 
 
/**
 * \brief 初始化事件标记组
 */  
void rtos_flag_grp_init(rtos_flag_grp_t *p_flag_grp, uint32_t flag)
{
    rtos_task_event_init(&p_flag_grp->flag_grp_event, RTOS_EVENT_TYPE_FLAG_GRP);
    
    p_flag_grp->flag = flag;
                 
}


/**
 * \brief 等待计数事件标记组
 */  
int32_t rtos_flag_grp_wait (rtos_flag_grp_t *p_flag_grp, 
                            uint32_t wait_type, 
                            uint32_t req_flag, 
                            uint32_t *p_req_result, 
                            uint32_t wait_ticks)
{
    uint32_t result = 0;
    uint32_t flags = req_flag;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    result = __rtos_rtos_flag_grp_check_and_consum(p_flag_grp, wait_type, &flags); 
    
    if (result != RTOS_OK) {
        
        /* 设置任务需要等待的事件标记组类型 */
        p_current_task->wait_flags_grp_type = wait_type;
        p_current_task->event_flags_grp    = req_flag;
        
        rtos_task_event_wait(p_current_task, &p_flag_grp->flag_grp_event, NULL, RTOS_EVENT_TYPE_FLAG_GRP, wait_ticks); 
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 
        
        /* 最后再执行一次事件调度，以便于切换到其它任务 */
        rtos_task_sched(); 
        
        /* 当由于等待超时或者事件标记组标记相关可用时，执行会返回到这里，然后取出等待结果与任务等待到哪些事件标记位 */
        *p_req_result = p_current_task->event_flags_grp;
        
        return  p_current_task->event_wait_result;
        
    } else {
        /* 当由于等待超时或者事件标记组标记相关可用时，执行会返回到这里，然后取出等待结果与任务等待到哪些事件标记位 */
        *p_req_result = flags;
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 
    }
    
    
    return result;
    
}




/**
 * \brief 获取事件标记组，如果事件标记组计数不可用，则立即退回
 */  
int32_t rtos_flag_grp_get (rtos_flag_grp_t *p_flag_grp, uint32_t wait_type, uint32_t req_flag, uint32_t *p_req_result)
{
    uint32_t result = 0;
    uint32_t flags = req_flag;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    result = __rtos_rtos_flag_grp_check_and_consum(p_flag_grp, wait_type, &flags); 
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
   *p_req_result =  flags;

    return result;    
}


/**
 * \brief 设置事件标记组中的某一位，唤醒等待队列中的一个任务
 */  
void rtos_flag_grp_notify (rtos_flag_grp_t *p_flag_grp, uint32_t set_bit, uint32_t flag)
{
    dlist_node_t *p_nxet_node = NULL;
    
    dlist_node_t *p_current_node = NULL;
     
    rtos_task_list_t *p_flag_grp_wait_list = NULL;  
    
    rtos_task_t *p_task = NULL;
    
    uint32_t flag_grp = 0;
    
    uint32_t sched = 0;
    
    uint32_t result = 0;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();    
    
    if (set_bit) {
        p_flag_grp->flag |= flag;
    } else {
        p_flag_grp->flag &= ~flag;        
    }      
    
    p_flag_grp_wait_list = &p_flag_grp->flag_grp_event.event_wait_list;
   

    /* 遍历所有的等待任务, 获取满足条件的任务，加入到待移除列表中 */    
    for (p_current_node = p_flag_grp_wait_list->head_node.p_next; 
         p_current_node != &(p_flag_grp_wait_list->head_node);
         p_current_node = p_nxet_node) {
             
         p_task =  RTOS_CONTAINER_OF(p_current_node, rtos_task_t, event_node);
             
         flag_grp = p_task->event_flags_grp;
             
             
         p_nxet_node =  p_current_node->p_next;
             
             
         result = __rtos_rtos_flag_grp_check_and_consum(p_flag_grp, p_task->wait_flags_grp_type, &flag_grp);

         if (result == RTOS_OK) {
             
             /* 唤醒任务 */
             p_task->event_flags_grp = flag_grp;
             rtos_task_event_wake_up(&p_flag_grp->flag_grp_event, p_task, (void *)0,  RTOS_OK);
             sched = 1;
             

         }             

     }
     
     /* 如果有任务就绪，则执行一次调度 */
     if (sched) {
         rtos_task_sched();          
     }
             
                            
    /* 退出临界区 */
    rtos_task_critical_exit(status);     
    
}

/**
 * \brief  查询事件标志组的状态信息
 */  
void rtos_flag_grp_info_get (rtos_flag_grp_t *p_flag_grp, rtos_flag_grp_info_get_t *p_info)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();  
    
    p_info->flag = p_flag_grp->flag;
    
    /* 待待该信号量的任务数量 */    
    p_info->task_count =  rtos_event_wait_count(&p_flag_grp->flag_grp_event);
 
    /* 退出临界区 */
    rtos_task_critical_exit(status);         
}
 
 
/**
 * \brief  销毁事件标志组
 */  
uint32_t rtos_flag_grp_destroy (rtos_flag_grp_t *p_flag_grp)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();  
       
    /* 清空事件控制块中的任务 */
    uint32_t count  = rtos_task_event_all_remove(&p_flag_grp->flag_grp_event, NULL, -RTOS_DEL);  
    p_flag_grp->flag = 0; 
    
    /* 清空过程中可能有更高优先级任务就绪，执行一次调度 */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
 
    /* 退出临界区 */
    rtos_task_critical_exit(status);  
    
    return count;           
} 
 
 
 
 
 
 
 
 
 
 
 
/* end of file */

