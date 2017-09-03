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
 #include "rtos_hook.h"
 
/**
 * \name RTOS 相关全局变量声明
 * @{
 */

/** \brief 任务优先级的就绪表定义变量 */
rtos_task_bitmap_t task_priobitmap = {0};

/** \brief  任务延时队列 */
rtos_task_list_t rtos_task_delayedlist;

/** \brief 当前任务：记录当前是哪个任务正在运行 */
rtos_task_t * p_current_task;

/** \brief 下一个将即运行的任务：在进行任务切换前，先设置好该值，然后任务切换过程中会从中读取下一任务信息 */
rtos_task_t * p_next_task;

/** \brief 空闲任务结构体 */
rtos_task_t idle_task;

/** \brief 空闲任务堆栈 */
taskstack_t idle_task_stack_buf[RTOS_IDLE_TASK_STACK_SIZE];

/** \brief 同一个优先级任务的链表头结点 */
rtos_task_list_t task_table[TASK_COUNT];

taskstack_t idle_task_stack_buf[RTOS_IDLE_TASK_STACK_SIZE];

/* CPU占有率测量的时候,任务初始化要放在空闲任务时时面执行 */    
/** \brief 指向用户应用的函数指针 */
rtos_pfn_no_arg_t pfn_app_task_init;  

/** \brief rtos运行状态标记  */
static uint8_t __rtos_running_status = 0;
 


/** @} */

/**
 * \brief 空闲任务入口函数
 */
void idle_task_entry (void *p_arg)
{
  
#if RTOS_ENABLE_CPU_USE_CHECK == 1
    
    /* CPU占有率测量的时候， 任务调度禁止 */   
    rtos_task_sched_disable();
    
#if RTOS_ENABLE_TIMER == 1        
   /* CPU占有率测量的时候,任务初始化要放在空闲任务里面执行 */    
    rtos_timer_task_init();
#endif    
    
    /* 应用任务初始化 */
    pfn_app_task_init();       

    /* 确保任务被调度起来后，再初始化系统节拍周期为10ms，否则会出现问题 */
    rtos_systick_init(RTOS_SYSTICK_PERIOD); 
    
    /* 等待时钟同步 */
    rtos_cpu_use_sync_with_systick();
    
#endif    
    
    for (; ;) {
        
    /* 空闲任务可以添加一些钩子函数 */
        
#if RTOS_ENABLE_CPU_USE_CHECK == 1
        /* 进入临界区，以保护在整个任务调度与切换期间，不会因为发生中断导致currentTask和nextTask可能更改 */    
        uint32_t status = rtos_task_critical_entry(); 
        
        /*　统计运行空闲任务的计数 */
        rtos_cpu_idle_count_inc();
        
        /* 退出临界区 */
        rtos_task_critical_exit(status); 
                 
#endif
        
#if RTOS_ENABLE_HOOK == 1

   /* 空闲钩子函数 */
   rtos_hook_cpu_idle();
   
#endif        

        
    }
}



/**
 * \brief rtos初始化函数
 */
int rtos_init (void)
{
    /* 初始化调度锁 */
    rtos_task_schedlock_init();
    
    /* 初始化任务任务就绪表位图数据 */
    rtos_task_bitmap_init(&task_priobitmap);
    
    /* 初始化优先级任务调度链表 */
    rtos_task_sched_init(task_table);
    
    /* 初始化任务延时队列 */    
    rtos_task_delayed_init(&rtos_task_delayedlist);
    
#if RTOS_ENABLE_TIMER == 1    
    /* 初始化定时器模块 */  
    rtos_timer_moudule_init(); 

    /* 初始化定时器任务 */    
#if RTOS_ENABLE_CPU_USE_CHECK == 0
    rtos_timer_task_init();
    
#endif

#endif

    /* 空闲任务初始化 */
    rtos_task_init(&idle_task, idle_task_entry, NULL, RTOS_PRIO_COUNT - 1,  idle_task_stack_buf, sizeof(idle_task_stack_buf),RTOS_TASK_OPT_SAVE_FP);



    return RTOS_OK;
}


/**
 * \brief rtos cpu占有率测试
 */
void rtos_cpu_use_check_test (rtos_pfn_no_arg_t p_app_task_init)
{
     pfn_app_task_init  =  p_app_task_init;      
}

/**
 * \brief rtos运行状态检查
 */
uint8_t rtos_running_check (void) 
{  
    return __rtos_running_status ;          
}



/**
 * \brief rtos操作系统启动函数
 */
void rtos_start (void)
{
    /* 表明RTOS已经启动了 */
    __rtos_running_status = 1;
    
    
    /* 自动查找最高优先级的任务运行 */
    p_next_task =  rtos_task_highest_ready();
    
    
    
    /*  切换到p_next_task 指向的任务，这个函数永远不会返回 */
    rtos_task_run_first();
}










/* end of file */

