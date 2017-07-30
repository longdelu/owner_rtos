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
#include "arm.h"
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
#include "rtos_task_delay.h"
#include "rtos_task_switch.h"
#include "rtos_task_bitmap.h"
#include "rtos_task_list.h"
#include "rtos_task_event.h"
#include "rtos_sem.h"
#include "rtos_mbox.h"
#include "rtos_memblock.h"

#define   TASK_STACK_SIZE  1024

/** \brief 任务1标记 */
int g_task_flag1 = 0;

/** \brief 任务2标记 */
int g_task_flag2 = 0;

/** \brief 任务3标记 */
int g_task_flag3 = 0;

/** \brief 任务4标记 */
int g_task_flag4 = 0; 

/** \brief 任务优先级的标记位置结构全变量 */
rtos_task_bitmap_t task_priobitmap = {0};

/** \brief  任务延时队列 */
rtos_task_list_t rtos_task_delayedlist;

/** \brief 任务堆栈 */
taskstack_t first_task_stack_buf[TASK_STACK_SIZE];
taskstack_t second_task_stack_buf[TASK_STACK_SIZE];
taskstack_t third_task_stack_buf[TASK_STACK_SIZE];
taskstack_t forth_task_stack_buf[TASK_STACK_SIZE];

/** \brief 当前任务结构体 */
rtos_task_t first_task;

/** \brief 下一个任务结构体 */
rtos_task_t second_task;

/** \brief 第三个任务结构体 */
rtos_task_t third_task;

/** \brief 第四个任务结构体 */
rtos_task_t forth_task;

/** \brief 当前任务：记录当前是哪个任务正在运行 */
rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
rtos_task_t * p_next_task;

/** \brief 空闲任务结构体 */
rtos_task_t idle_task;

/** \brief 空闲任务堆栈 */
taskstack_t idle_task_stack_buf[TASK_STACK_SIZE];

/** \brief 同一个优先级任务的链表头结点 */
rtos_task_list_t task_table[TASK_COUNT];



/**
 * \brief 空闲任务入口函数
 */
void idle_task_entry (void *p_arg)
{
    for (; ;) {
        /* 空闲任务暂时什么都不做，它可以被用户任务抢占 */
        
    }
}

/**
 * \brief 存储控制块分布在data数据段，定义了20个100个字节大小的存储控制块
 */
uint8_t mem1[20][100];
rtos_memblock_t memblock1;
typedef uint8_t (*block)[100];     

uint8_t *p8[10];
uint32_t *p32[10];
void *p_void[10];

block memblock3[20];
    


/**
 * \brief 当前任务入口函数
 * \note  系统节拍初始化函数要等任务初始化函数完成后才能调用
 */
void first_task_entry (void *p_arg)
{  
    block memblock[20];
    uint8_t i = 0;
    
    /* 确保任务被调度起来后，再初始化系统节拍周期为10ms，否则会出现问题 */
    rtos_systick_init(10); 
    
    rtos_memblock_init(&memblock1, (uint8_t *)mem1,  sizeof(mem1[0]), sizeof(mem1) / sizeof(mem1[0]));
    
    for (i = 0; i < sizeof(mem1) / sizeof(mem1[0]); i++) {
        
         rtos_memblock_wait(&memblock1, (uint8_t **)&memblock[i], 0);
        
    }

    rtos_sched_mdelay(2); 

    for (i = 0; i < sizeof(mem1) / sizeof(mem1[0]); i++) {
        
         memset(memblock[i], i, 100);
        
         rtos_memblock_notify(&memblock1, (uint8_t *)memblock[i]);
         rtos_sched_mdelay(2);         
    }
    
   
    for (; ;) {      
         
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(1); 
        *((uint32_t*) p_arg) = 0;
        rtos_sched_mdelay(1);              
    }
}

/**
 * \brief 第二个任务入口函数
 */
void second_task_entry (void *p_arg)
{   
    int error = 0;
    
    block memblock;   /*　(uint8_t **)&memblock　取得其地址，通过指针修改该变量的内容　*/
        
    for (; ;) {
        
        error = rtos_memblock_wait(&memblock1, (uint8_t **)&memblock, 0);
        if (error == RTOS_OK) 
        {
            uint32_t value = *(uint8_t *)memblock;
            *((uint32_t*) p_arg) = value;
            rtos_sched_mdelay(1);
        }                
     
    }
}

/**
 * \brief 第三个任务入口函数
 */
void third_task_entry (void *p_arg)
{     
       
    for (; ;) {
        
             
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(1); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(1);      
    }
}

/**
 * \brief 第四个任务入口函数
 */
void forth_task_entry (void *p_arg)
{        
    
    for (; ;) {
        
                
        *((uint32_t*) p_arg) = 1;
        rtos_sched_mdelay(1); 
        *((uint32_t*) p_arg) = 0   ;
        rtos_sched_mdelay(1); 
                      
    }
}

/**
 * \brief 入口函数
 */
int main (void)
{    
      
    /* 组优先级有4位，次优先级也有4位 */
    NVIC_SetPriorityGrouping(0x03);
    NVIC_SetPriority(PendSV_IRQn, NVIC_EncodePriority(0x03,0x0F,0x0F));
    
    /* 初始化调度锁 */
    rtos_task_schedlock_init();
    
    /* 初始化任务任务就绪表位图数据 */
    rtos_task_bitmap_init(&task_priobitmap);
    
    /* 初始化优先级任务调度链表 */
    rtos_task_sched_init(task_table);
    
    /* 初始化任务延时队列 */    
    rtos_task_delayed_init(&rtos_task_delayedlist);
    
    /* 任务初始化函数 */
    rtos_task_init(&first_task,   first_task_entry,  &g_task_flag1, 0,  first_task_stack_buf,   sizeof(first_task_stack_buf)); 
    rtos_task_init(&second_task,  second_task_entry, &g_task_flag2, 1,  second_task_stack_buf,  sizeof(second_task_stack_buf));
    rtos_task_init(&third_task,   third_task_entry,  &g_task_flag3, 1,  third_task_stack_buf, sizeof(third_task_stack_buf));
    rtos_task_init(&forth_task,   forth_task_entry,  &g_task_flag4, 1,  forth_task_stack_buf, sizeof(forth_task_stack_buf));
        
    /* 空闲任务初始化 */
    rtos_task_init(&idle_task, idle_task_entry, NULL, RTOS_PRIO_COUNT - 1,  idle_task_stack_buf, sizeof(idle_task_stack_buf));

    
    /* 自动查找最高优先级的任务运行 */
    p_next_task =  rtos_task_highest_ready();
    
    /*  切换到p_next_task 指向的任务，这个函数永远不会返回 */
    rtos_task_run_first();
      
    return 0;    
}




/* end of file */

