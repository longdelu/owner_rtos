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
 * \brief main函数入口
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "rtos_init.h"

#define   TASK_STACK_SIZE  1024  

/** \brief 任务1标记 */
int g_task_flag1 = 0;

/** \brief 任务2标记 */
int g_task_flag2 = 0;

/** \brief 任务3标记 */
int g_task_flag3 = 0;

/** \brief 任务4标记 */
int g_task_flag4 = 0; 

/** \brief 任务堆栈 */

 /* 按8字节对齐 */
__align(8) taskstack_t first_task_stack_buf[TASK_STACK_SIZE];
__align(8) taskstack_t second_task_stack_buf[TASK_STACK_SIZE];
__align(8) taskstack_t third_task_stack_buf[TASK_STACK_SIZE];
__align(8) taskstack_t forth_task_stack_buf[TASK_STACK_SIZE];

/** \brief 当前任务结构体 */
rtos_task_t first_task;

/** \brief 下一个任务结构体 */
rtos_task_t second_task;

/** \brief 第三个任务结构体 */
rtos_task_t third_task;

/** \brief 第四个任务结构体 */
rtos_task_t forth_task;

/**
 * \brief 任务状态信息
 */

/**
 * \brief 当前任务入口函数
 * \note  系统节拍初始化函数要等任务初始化函数完成后才能调用
 */
void first_task_entry (void *p_arg)
{  
/* CPU不测量占有率的时候 */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0    
    /* 确保任务被调度起来后，再初始化系统节拍周期为10ms，否则会出现问题 */
    rtos_systick_init(RTOS_SYSTICK_PERIOD); 
#endif
    
    for (; ;) {         
         
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0;
        rtos_sched_mdelay(10);      
             
    }
}

/**
 * \brief 第二个任务入口函数
 */
void second_task_entry (void *p_arg)
{          
    for (; ;) {
        
        /*
         * 当运行至此处时，由于互斥信号量的优先级继承机制
         * task2的优先级由1变成0
         */        
       
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(10);       
 
    }
}

/**
 * \brief 第三个任务入口函数
 */
void third_task_entry (void *p_arg)
{     
       
    for (; ;) {
                    
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(10);    
         
    }
}

/**
 * \brief 第四个任务入口函数
 */
void forth_task_entry (void *p_arg)
{        
    
    for (; ;) {
                        
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(10); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(10); 
                      
    }
}

/**
 * \brief RTOS 应用相关任务初始化
 */
void  rtos_task_app_init (void) 
{
    rtos_task_init(&first_task,   first_task_entry,  &g_task_flag1, 0,  first_task_stack_buf,   sizeof(first_task_stack_buf),RTOS_TASK_OPT_SAVE_FP); 
    rtos_task_init(&second_task,  second_task_entry, &g_task_flag2, 1,  second_task_stack_buf,  sizeof(second_task_stack_buf),RTOS_TASK_OPT_SAVE_FP);
    rtos_task_init(&third_task,   third_task_entry,  &g_task_flag3, 1,  third_task_stack_buf, sizeof(third_task_stack_buf),RTOS_TASK_OPT_SAVE_FP);
    rtos_task_init(&forth_task,   forth_task_entry,  &g_task_flag4, 1,  forth_task_stack_buf, sizeof(forth_task_stack_buf),RTOS_TASK_OPT_SAVE_FP);    
    
}    

/**
 * \brief 入口函数
 */
int main (void)
{       
    /* 组优先级有4位，次优先级也有4位 */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
   
    /* RTOS初始化 */
    rtos_init();

/* CPU不测量占有率的时候 */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0
  
    /* 任务初始化函数 */
    rtos_task_app_init();
  
#else
    
    /* 表明是在测试CPU占有率 */
    rtos_cpu_use_check_test(rtos_task_app_init);
    
    /* CPU占有率测试初始化 */
    rtos_cpu_use_init();
    
#endif    
    
    /* 启动操作系统, 自动查找最高优先级的任务运行,这个函数永远不会返回 */
    rtos_start();
    
    return RTOS_OK;    
}




/* end of file */

