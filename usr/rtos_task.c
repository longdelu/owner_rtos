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


/** \brief 当前任务：记录当前是哪个任务正在运行 */
extern rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
extern rtos_task_t * p_next_task;


/** \brief 所有任务的指针数组：简单起见，只使用两个任务 */
extern rtos_task_t * p_task_table[TASK_COUNT];

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
    
    
    task->task_stack_top = p_task_stack_top;                                // 保存最终的值 
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
    
    if (p_current_task ==  p_task_table[0]) {
                
        p_next_task =  p_task_table[1];
        
    } else {
        
        p_next_task =  p_task_table[0];
        
    }
    
    rtos_task_switch();
    
}





/* end of file */

