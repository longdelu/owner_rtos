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




/**
 * \brief 任务初始化
 */
void rtos_task_init(rtos_task_t * task, 
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

    p_task_table[task_prio] = task;                                // 以优先级为顺序，填入任务优先级表，方便通过优先级查找到对应的任务
    
    rtos_task_bitmap_set(&task_priobitmap, task_prio);             // 标记优先级位置中的相应位
}    


/**
 * \brief 获取当前最高优先级且可运行的任务                                     
 */
rtos_task_t *rtos_task_highest_ready(void) 
{
    uint32_t highestPrio = rtos_task_bitmap_first_set_get(&task_priobitmap);
    
    return p_task_table[highestPrio];
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

#if 1
/**
 * \brief 任务调度
 */
void rtos_task_sched(void)
{
    rtos_task_t *  p_temp_task = NULL;
    
    /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* 调度锁打开时，表时不允许进行任务调度，直接返回 */
    if (rtos_task_schedlock_status()) {
        
        rtos_task_critical_exit(status);
        return;
        
    }
    
    
   /* 找到优先级最高的任务，如果其优先级比当前任务的还高，那么就切换到这个任务 */
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
#endif

#if 0
/**
 * \brief 任务调度
 */
void rtos_task_sched(void)
{
    /* 
     * 进入临界区，以保护在整个任务调度与切换期间，
     * 不会因为发生中断导致p_current_task和p_next_task可能更改
     */    
    uint32_t status = rtos_task_critical_entry(); 
    
    if (rtos_task_schedlock_status()) {
        
        rtos_task_critical_exit(status);
        return;
        
    }
    
    /* 
     * 空闲任务只有在所有其它任务都不是延时状态时才执行
     * 所以，我们先检查下当前任务是否是空闲任务
     */
    if (p_current_task == p_idle_task) 
    {
        // 如果是的话，那么去执行task1或者task2中的任意一个
        // 当然，如果某个任务还在延时状态，那么就不应该切换到他。
        // 如果所有任务都在延时，那么就继续运行空闲任务，不进行任何切换了
        if (p_task_table[0]->delay_ticks == 0) 
        {
            p_next_task = p_task_table[0];
        }           
        else if (p_task_table[1]->delay_ticks == 0) 
        {
            p_next_task = p_task_table[1];
        } else 
        {
            rtos_task_critical_exit(status);
            return;
        }
    } 
    else 
    {
        // 如果是task1或者task2的话，检查下另外一个任务
        // 如果另外的任务不在延时中，就切换到该任务
        // 否则，判断下当前任务是否应该进入延时状态，如果是的话，就切换到空闲任务。否则就不进行任何切换
        if (p_current_task == p_task_table[0]) 
        {
            if (p_task_table[1]->delay_ticks == 0) 
            {
                p_next_task = p_task_table[1];
            }
            else if (p_current_task->delay_ticks != 0) 
            {
                p_next_task = p_idle_task;
            } 
            else 
            {
                rtos_task_critical_exit(status);
                return;
            }
        }
        else if (p_current_task == p_task_table[1]) 
        {
            if (p_task_table[0]->delay_ticks == 0) 
            {
                p_next_task = p_task_table[0];
            }
            else if (p_current_task->delay_ticks != 0) 
            {
                p_next_task = p_idle_task;
            }
            else 
            {
                rtos_task_critical_exit(status);
                return;
            }
        }
    }
    
    /* 触发PendSVC异常，进行任务切换 */
    rtos_task_switch();
    
    /* 退出临界区 */
    rtos_task_critical_exit(status); 
    
}

#endif



/* end of file */

