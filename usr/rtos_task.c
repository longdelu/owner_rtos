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
 * \brief 操作系统任务相关函数实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "arm.h"
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_switch.h"
#include "rtos_task_delay.h"
#include "rtos_task_critical.h"
#include "rtos_task_bitmap.h"
#include "rtos_config.h"
#include "rtos_task_event.h"

/** \brief 任务优先级的标记位置结构全变量 */
extern rtos_task_bitmap_t task_priobitmap;

/** \brief  任务延时队列 */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief 同一个优先级任务的链表头结点 */
extern rtos_task_list_t task_table[TASK_COUNT];


/**
 * \brief 任务初始化
 */
void rtos_task_init(rtos_task_t *task, 
                    void (*task_entry) (void *p_arg), 
                    void *p_arg, 
                    uint32_t task_prio, 
                    uint32_t *task_stack,
                    uint32_t task_stack_size)
{
    uint32_t * p_task_stack_top = NULL;
    
    
    task->stack_base = task_stack;
    task->stack_size = task_stack_size;
    
    memset(task_stack, 0, task_stack_size);
    
    
    p_task_stack_top =  task_stack +  task_stack_size / sizeof(taskstack_t);
    
    
    /*
     * 注意以下两点：
     * 1、不需要用到的寄存器，直接填了寄存器号，方便在IDE调试时查看效果；
     * 2、顺序不能变，要结合PendSV_Handler以及CPU对异常的处理流程来理解
     *
     */
    *(--p_task_stack_top) = (unsigned long)(1<<24);                // XPSR, 设置了Thumb模式，恢复到Thumb状态而非ARM状态运行
    *(--p_task_stack_top) = (unsigned long)task_entry;             // 程序的入口地址PC(R15)
    *(--p_task_stack_top) = (unsigned long)0x14;                   // R14(LR), 任务不会通过return xxx结束自己，所以未用
    *(--p_task_stack_top) = (unsigned long)0x12;                   // R12, 未用
    *(--p_task_stack_top) = (unsigned long)0x3;                    // R3, 未用
    *(--p_task_stack_top) = (unsigned long)0x2;                    // R2, 未用
    *(--p_task_stack_top) = (unsigned long)0x1;                    // R1, 未用
    *(--p_task_stack_top) = (unsigned long)p_arg;                  // R0 = param, 传给任务的入口函数
    *(--p_task_stack_top) = (unsigned long)0x11;                   // R11, 未用
    *(--p_task_stack_top) = (unsigned long)0x10;                   // R10, 未用
    *(--p_task_stack_top) = (unsigned long)0x9;                    // R9, 未用
    *(--p_task_stack_top) = (unsigned long)0x8;                    // R8, 未用
    *(--p_task_stack_top) = (unsigned long)0x7;                    // R7, 未用
    *(--p_task_stack_top) = (unsigned long)0x6;                    // R6, 未用
    *(--p_task_stack_top) = (unsigned long)0x5;                    // R5, 未用
    *(--p_task_stack_top) = (unsigned long)0x4;                    // R4, 未用
    
    
    task->task_stack_top = p_task_stack_top;                       // 保存最终的值,为栈顶的地址

    task->delay_ticks    = 0;                                      // 任务延时间片
       
    task->prio           = task_prio;                              // 设置任务的优先级 
    
    task->slice          = RTOS_SLICE_MAX;                         // 初始化任务的时间片计数
    
    task->task_state     = RTOS_TASK_STATE_REDDY;                  // 设置任务为就绪状态 
    
    task->suspend_cnt    = 0;                                      // 初始挂起次数为0
    
    
    task->clean_param    = NULL;                                   // 设置传递给清理函数的参数
    task->pfn_clean      = NULL;                                   // 设置清理函数
    task->req_delete_flag = 0;                                     // 请求删除标记
    
    dlist_init(&task->delay_node);                                 // 初始化延时队列结点
    dlist_init(&task->prio_node);                                  // 初始化同一优先级任务队列结点，用于就绪表
    dlist_init(&task->event_node);                                 // 初始化等待事件延时结点  
    
    task->p_event = NULL;                                          // 没有等待事件       
    task->p_event_msg = NULL;                                      // 没有存储等待事件的消息
    task->event_wait_result  =  RTOS_EVENT_TYPE_UNKNOW;            // 没有等待事件错误
    
    
    /*
     * \note 调用该函数，一定要保证该优先级的任务头结点已经有正确的指向
     */
    rtos_task_list_add_tail(&task_table[task_prio], &(task->prio_node));  // 插入对应的优先级队列中
    
    rtos_task_bitmap_set(&task_priobitmap, task_prio);             // 标记优先级位置中的相应位
} 

/**
 * \brief 将任务设置为就绪状态                                     
 */
void rtos_task_sched_ready(rtos_task_t *task)
{
    /* 仅仅在同一优先级列表尾部插入任务 */
    rtos_task_list_add_tail(&task_table[task->prio], &(task->prio_node)); 
    
    rtos_task_bitmap_set(&task_priobitmap,task->prio); 
} 


/**
 * \brief 将任务从绪列表中移除                                    
 */
void rtos_task_sched_unready(rtos_task_t *task)
{
    /* 同一优先级的任务不可能同时处于任务延时队列及任务优先级队列当中 */
    rtos_task_list_remove(&task_table[task->prio], &(task->prio_node));
    
    if (rtos_task_list_count(&task_table[task->prio]) == 0) {
    
        /* 只能该优先级的任务链表上没有任何任务时，才清除就绪表 */
        rtos_task_bitmap_clr(&task_priobitmap,task->prio);
    }        
}

/**
 * \brief 初始化优先级任务调度链表                                    
 */
void rtos_task_sched_init(rtos_task_list_t *p_task_table)
{
    
    volatile uint32_t i = 0;
    
    for (i = 0; i < RTOS_PRIO_COUNT; i++) {
         rtos_task_list_init(&p_task_table[i]);
    }    
}  


/**
 * \brief 获取当前最高优先级且可运行的任务                                     
 */
rtos_task_t *rtos_task_highest_ready(void) 
{
    uint32_t highestPrio = rtos_task_bitmap_first_set_get(&task_priobitmap);
    
    dlist_node_t *p_node = rtos_task_list_begin_get(&task_table[highestPrio]);
    
    /* 近回优先级任务链表的最前面的那个任务 */
    return RTOS_CONTAINER_OF(p_node, rtos_task_t, prio_node);;
}    



/**
 * \brief 切换到第一个任务运行，启动操作系统第一个任务
 *
 * \note  中判断当前切换是tinyOS启动时切换至第1个任务，还是多任务已经跑起来后执行的切换
 *
 */
void rtos_task_run_first (void)
{
    /* 正常情况下使用的主堆栈MSP(特权级）*/  
    __set_PSP(0);
    
    PendSVC_Trigger();
    
}

/**
 * \brief 任务调度
 */
void rtos_task_sched(void)
{
    rtos_task_t *p_temp_task = NULL;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* 调度锁打开时，表时不允许进行任务调度，直接返回 */
    if (rtos_task_schedlock_status()) {
        
        rtos_task_critical_exit(status);
        return;
        
    }
    
    
   /* 找到此时可运行优先级最高的任务，那么就切换到这个任务 */
    p_temp_task = rtos_task_highest_ready();    
        
    /* p_current_task 与 p_next_task 这两个者会在PendSVC异常处理函数中修正其值 */
    if (p_current_task !=  p_temp_task) {
                
        p_next_task =  p_temp_task;    

        /* 触发PendSVC异常，进行任务切换 */
        rtos_task_switch();
        
    }
        
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
}


/**
 * \brief 初始化任务延时队列
 */
void rtos_task_delayed_init (rtos_task_list_t *p_rtos_task_delayed_list)
{
     rtos_task_list_init(p_rtos_task_delayed_list);  
}


/**
 * \brief 将任务加入到延时队列中
 */
void rtos_task_add_delayed_list (rtos_task_t *p_task, uint32_t delay_ticks)
{
    p_task->delay_ticks = delay_ticks;
    
    rtos_task_list_add_tail(&rtos_task_delayedlist, &p_task->delay_node);

    p_task->task_state |= RTOS_TASK_STATE_DELAYED;

    
   /*　表示被删除的任务重新加入到延时队列中,重置任务所拥有的状态 */
   if ( p_task->task_state & RTOS_TASK_STATE_RED_DEL) {
       
       p_task->task_state &= ~RTOS_TASK_STATE_RED_DEL;  
   }
          
}


/**
 * \brief 将任务从延时队列中唤醒
 */
void rtos_task_wake_up_delayed_list (rtos_task_t *p_task)
{
    rtos_task_list_remove(&rtos_task_delayedlist, &p_task->delay_node);

    p_task->task_state &= ~RTOS_TASK_STATE_REDDY;    
           
}


/**
 * \brief 将延时的任务从延时队列中删除,
 */
void rtos_task_del_delayed_list (rtos_task_t *p_task)
{
    
    rtos_task_list_remove(&rtos_task_delayedlist, &p_task->delay_node);

    p_task->task_state |= RTOS_TASK_STATE_RED_DEL;       
}


/**
 * \brief 挂起任务
 */
void rtos_task_suspend (rtos_task_t *p_task)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /** 
     * 
     *\note 这个是设计策略的问题，延时的任务也是可以允许挂起的,简单思路是
     *       先设个标记，等延时结束期望加到就绪队列时检查这个标记，有标记
     *       就在此时挂起而不是加入就绪队列。
     */ 
    
    /* 不允许对已经进入延时状态的任务挂起 */
    if (!(p_task->task_state & RTOS_TASK_STATE_DELAYED)) 
    {
        /* 
         *  增加挂起计数，仅当该任务被执行第一次挂起操作时，才考虑是否
         *  要执行任务切换操作
         */
        if (p_task->suspend_cnt == 0)
        {
           
            /* 挂起的最大次数为4294967295UL */
            if (p_task->suspend_cnt <= 4294967295UL) {
                p_task->suspend_cnt++;                
            }
            /*  设置挂起标志 */
            p_task->task_state |= RTOS_TASK_STATE_SUSPEND;

            /* 
             * 挂起方式很简单，就是将其从就绪队列中移除，这样调度器就不会发现他
             * 也就没法切换到该任务运行
             */
            rtos_task_sched_unready(p_task);

            /* 当然，这个挂起任务可能是自己，那么就切换到其它任务 */
            if (p_task == p_current_task)
            {
                rtos_task_sched();
            }
        }
    }
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
           
}


/**
 * \brief 唤醒挂起任务，只是把任务加入到同一优先级的任务链表的尾部，并不保证插入到以前的对应的位置
 */
void rtos_task_wakeup (rtos_task_t *p_task)
{
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
    
    /* 检查任务是否处于挂起状态 */
    if (p_task->task_state & RTOS_TASK_STATE_SUSPEND) {
        
        /* 递减挂起计数，如果为0了，则清除挂起标志，同时设置进入就绪状态 */
        if (--p_task->suspend_cnt == 0) {
            
            /*  清楚挂起标志 */
            p_task->task_state &= ~RTOS_TASK_STATE_SUSPEND;
            
            /* 
             * 同时将任务放回就绪队列中
             */
            rtos_task_sched_ready(p_task);
            
            /* 取消挂起的过程中，可能有更高优先级的任务就绪，执行一次任务调度 */
            rtos_task_sched();
            
        }
        
    }

        

    /* 退出临界区 */
    rtos_task_critical_exit(status);     
}

/**
 * \brief 设置任务被删除时调用的清理函数
 */
void rtos_task_set_clean_call_fuc (rtos_task_t *p_task, void (*pfn_clean)(void * p_par), void * p_par)
{
    p_task->clean_param =  p_par;
    p_task->pfn_clean = pfn_clean;
        
}


/**
 * \brief 强制删除指定的任务（只能删除一次该任务，不能在调用该函数后再调用删除函数）
 */
void rtos_task_force_del (rtos_task_t *p_task)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
    
    /*  如果任务处于延时状态，则从延时队列中删除 */
    if (p_task->task_state & RTOS_TASK_STATE_DELAYED) {
        rtos_task_del_delayed_list(p_task);
    }
    
    /* 如果任务不处于挂起状态，那么就是就绪态，从就绪表中删除 */
    if (p_task->task_state & RTOS_TASK_STATE_SUSPEND) {
        
         rtos_task_sched_unready(p_task);
    }
    
    /* 删除时，如果有设置清理函数，则调用清理函数 */
    if (p_task->pfn_clean) {
        p_task->pfn_clean(p_task->clean_param);
    }
    
    /* 如果删除的是自己，那么需要切换至另一个任务，所以执行一次任务调度 */
    if (p_current_task == p_task) {
        /* 取消挂起的过程中，可能有更高优先级的任务就绪，执行一次任务调度 */
        rtos_task_sched();
    }
        
           
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
        
}


/**
 * \brief 请求删除某个任务，由任务自己决定是否删除自己
 */
void rtos_task_req_del (rtos_task_t *p_task)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
 
    p_task->req_delete_flag = 1;

    /* 退出临界区 */
    rtos_task_critical_exit(status); 
        
}


/**
 * \brief 查询是否已经被请求删除自已
 */
uint32_t rtos_task_req_del_flag_check (void)
{
    uint32_t req_del_flag = 0;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
 
    /* 获取请求删除的标记 */
    req_del_flag = p_current_task->req_delete_flag;

    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
    
    return req_del_flag;
        
}

/**
 * \brief 任务删除自身(只能删除一次任务）
 */
void rtos_task_del_self (void)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
 
    /*
     * 任务在调用该函数时，必须是处于就绪状态，不可能处于延时或挂起等其它状态
     * 所以，只需要从就绪队列中移除即可   
     */    
    rtos_task_sched_unready(p_current_task);
    
    
    /* 删除时，如果有设置清理函数，则调用清理函数 */
    if (p_current_task->pfn_clean) {
        p_current_task->pfn_clean(p_current_task->clean_param);
    }
    
    /*　接下来肯定时切换到其它任务去运行 */
    rtos_task_sched(); 

    /* 退出临界区 */
    rtos_task_critical_exit(status); 
           
}


/**
 * \brief 获取任务相关信息
 */
void rtos_task_info_get (rtos_task_t *p_task, rtos_task_info_t *p_task_info)
{
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry();
      
    p_task_info->delay_ticks = p_task->delay_ticks;         // 延时信息
    p_task_info->task_prio   = p_task->prio;                // 任务优先级
    p_task_info->task_state  = p_task->task_state;          // 任务状态
    p_task_info->task_slice  = p_task->slice;               // 剩余时间片
    p_task_info->suspend_cnt = p_task->suspend_cnt;         // 被挂起的次数

    /* 退出临界区 */
    rtos_task_critical_exit(status); 
           
}

     
    










    




/* end of file */

