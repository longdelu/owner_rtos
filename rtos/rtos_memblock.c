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
 * \brief RTOS 存储块实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_memblock.h"
 
#if RTOS_ENABLE_MEMBLOCK == 1 
/**
 * \brief 初始化存储控制块
 */ 
void rtos_memblock_init (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr_start, uint32_t block_size, uint32_t block_cnt)
{
    uint8_t *p_memblock_addr_end = p_mem_addr_start + block_size * block_cnt;
    
    if (p_memblock == NULL || p_mem_addr_start == NULL) {
        return;
    } 
    
    /*
     * 每个存储块需要来放置链接指针，所以空间至少要比tNode大
     * 即便如此，实际用户可用的空间并没有少
     */
    if (block_size < sizeof(dlist_node_t)){
        return;
    }

    /* 初始化存储块事件控制块 */
    rtos_task_event_init(&p_memblock->memblock_event, RTOS_EVENT_TYPE_MEMBLOCK);

    p_memblock->mem_addr_start = p_mem_addr_start;
    p_memblock->block_size = block_size;
    p_memblock->max_count = block_cnt;

    /* 存储块链表初始化 */
    rtos_task_list_init(&p_memblock->block_list);
    
    while (p_mem_addr_start < p_memblock_addr_end) {
        rtos_task_list_add_tail(&p_memblock->block_list, (dlist_node_t *)p_mem_addr_start);

        p_mem_addr_start += block_size;
    }

} 


/**
 * \brief 等待存储块
 */
int32_t rtos_memblock_wait (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr, uint32_t wait_ticks)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /*　首先检查是否有空闲的存储块 */
    if (rtos_task_list_count(&p_memblock->block_list) > 0) {
        
        /* 如果有的话，取出一个空闲的存储块，修改了上层应用传进来的一维指针的指向，即从空指针转而指向了有用的指针 */
        *pp_mem_addr = (uint8_t *)rtos_task_list_remove_first(&p_memblock->block_list);
        /* 退出临界区 */
        rtos_task_critical_exit(status); 
        
        return RTOS_OK;

    } else {
           /*　将任务插入事件链表 */
        rtos_task_event_wait(p_current_task, &p_memblock->memblock_event, NULL, RTOS_EVENT_TYPE_MEMBLOCK, wait_ticks);
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 
        
        /* 最后再执行一次事件调度，以便于切换到其它任务 */
        rtos_task_sched();   

       /* 当切换回来时，从tTask中取出获得的消息,此时这个控制块有可能是int (*)[4]的地址 */
       *pp_mem_addr = (uint8_t *)p_current_task->p_event_msg;
       
       /* 取出等待结果 */        
       return  p_current_task->event_wait_result;

    }        
}

/**
 * \brief  获取存储块，如果没有存储块，则立即退回
 */
int32_t rtos_memblock_get (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /*　首先检查是否有空闲的存储块 */
    if (rtos_task_list_count(&p_memblock->block_list) > 0) {
        
        /* 如果有的话，取出一个空闲的存储块 */
        *pp_mem_addr = (uint8_t *)rtos_task_list_remove_first(&p_memblock->block_list);
        
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
 * \brief  通知存储块可用，唤醒等待队列中的一个任务，或者将存储块加入存储块队列链表中
 */
void rtos_memblock_notify (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
    
    /* 检查是否有任务等待 */
    if (rtos_event_wait_count(&p_memblock->memblock_event) > 0){
        /* 如果有的话，则直接唤醒位于队列首部（最先等待）的任务　*/
        rtos_task_t * task = rtos_task_first_event_wake_up(&p_memblock->memblock_event, (void *)p_mem_addr, RTOS_OK );

        /* 如果这个任务的优先级更高，就执行调度，切换过去 */
        if (task->prio < p_current_task->prio) {
             rtos_task_sched(); 
        }
    } else {
        
        /* 如果没有任务等待的话，将存储块插入到队列中 */
        rtos_task_list_add_tail(&p_memblock->block_list, (dlist_node_t *)p_mem_addr);        
    }  

    /* 退出临界区 */
    rtos_task_critical_exit(status);     
}
    
/**
 * \brief查询存储控制块的状态信息
 */  
void rtos_memblock_info_get (rtos_memblock_t * p_memblock, rtos_memblock_info_t *p_info)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    p_info->cur_count  =  rtos_task_list_count(&p_memblock->block_list);
    p_info->max_count  =  p_memblock->max_count;  
    p_info->block_size =  p_memblock->block_size;
    /* 待待存储块的任务数量 */    
    p_info->task_count =  rtos_event_wait_count(&p_memblock->memblock_event);
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
}

/**       
 * \brief 销毁存储控制块
 */  
uint32_t rtos_memblock_destroy (rtos_memblock_t *p_memblock)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 

    /* 清空事件控制块中的任务 */
    uint32_t count  = rtos_task_event_all_remove(&p_memblock->memblock_event, NULL, -RTOS_DEL);  
   
    
    /* 退出临界区 */
    rtos_task_critical_exit(status);  
    
    /* 清空过程中可能有更高优先级任务就绪，执行一次调度 */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
    
    return count;   
} 

#endif
 
 
/* end of file */




