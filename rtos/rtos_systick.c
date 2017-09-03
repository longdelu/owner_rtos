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
 * \brief 操作系统滴答时钟
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "arm.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
#include "rtos_task_bitmap.h"
#include "rtos_config.h"
#include "rtos_task_event.h"
#include "rtos_soft_timer.h"
#include "rtos_cpu_use.h"
#include "rtos_hook.h"
#include "rtos_task_delay.h"
#include "rtos_init.h"


/** \brief  任务延时队列 */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief 同一个优先级任务的链表头结点 */
extern rtos_task_list_t task_table[TASK_COUNT];

uint32_t rtos_systick = 0;      /**< \brief 系统滴答计数值 */

uint32_t __g_us_ticks = 0;      /**< \brief 当延时1us的计数值   */
uint32_t __g_ms_ticks = 0;      /**< \brief 当延时1ms的计数值   */

uint32_t __g_reload = 0;        /**< \brief systick重载计数值   */  

uint32_t __g_us_max = 0;        /**< \brief 延时的最大us数      */
uint32_t __g_ms_max = 0;        /**< \brief 延时的最大ms数      */


  
/**
 * \brief  任务延时滴答处理
 */
static void __rtos_task_delay_tick_handler (void)
{
    volatile uint32_t i = 0;
    
    volatile uint8_t slice_flag = 0;
    
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 
    
    rtos_task_t * p_task  = NULL;
       
    /*　临界区保护　*/
    uint32_t status = rtos_task_critical_entry();
       
    /* 获得延时队列第一个用户结点 */
    p_next  = dlist_begin_get(&rtos_task_delayedlist.head_node);
    
    /*  获得延时队列的结束位置: 为头结点本身 */
    p_end  = dlist_end_get(&rtos_task_delayedlist.head_node);
    
    
     /* 检查所有任务延时列表时面所有任务的delayTicks数，如果不0的话，减1 */
    while(p_next != p_end) {   

        /* 先纪录下当前结点 */
        p_tmp =  p_next;
           
        /* 先记录下一个结点的信息 */
        p_next = dlist_next_get(&rtos_task_delayedlist.head_node, p_next);
        
        
        p_task = RTOS_CONTAINER_OF(p_tmp, rtos_task_t, delay_node);
        
        if (--p_task->delay_ticks <= 0) {
            
             /* 如果任务还处于等待事件的状态，则将其从事件等待队列中唤醒 */
            if (p_task->p_event) {
                
                /* 此时，消息为空，等待结果为超时 */
                rtos_task_event_del(p_task, NULL, -RTOS_TIMEOUT);
                
            }
            
            /* 将任务从延时队列中移除 */
            rtos_task_wake_up_delayed_list(p_task);
            
            /* 将任务登记到就绪列表中 */
            rtos_task_sched_ready(p_task);
        }
        
              
    }
    
    /* 减去当前的时间片计数 */
    --p_current_task->slice;
    
    /*　上面那一段代码需要有可能唤醒延时队列当中的一些任务，但有可能该任务的任务并不当前的高，也不会立即切换过去 */
    
    /* 获得同一优先级就绪队列第一个用户结点 */
    p_next  = dlist_begin_get(&task_table[p_current_task->prio].head_node);
    
    /* 获得同一优先级就绪队列结束位置,为头结点本身 */
    p_end  = dlist_end_get(&task_table[p_current_task->prio].head_node);
    
     /* 检查所有同一优先级就绪队列包含的任务，判断当前任务是否还在里面 */
    while(p_next != p_end) {  
        
        /* 先纪录下当前结点 */
        p_tmp =  p_next;
           
        /* 先记录下一个结点的信息 */
        p_next = dlist_next_get(&task_table[p_current_task->prio].head_node, p_next);
        
        
        p_task = RTOS_CONTAINER_OF(p_tmp, rtos_task_t, prio_node);
        
        
        /* 该任务还在里面 */
        if (p_task == p_current_task){
            
            slice_flag = 1;

            
            break;

        }            
        
    }

    /* 检查下当前任务的时间片是否已经到了,并且确定该任务必须在该优先级就绪队列里面 */
    if (slice_flag == 1) {  
    
        /* 检查下当前任务的时间片是否已经到了,并且确定该任务必须在就绪队列里面 */
        if (p_current_task->slice == 0) {
            /*
             * 如果当前任务中还有其它任务的话，那么切换到下一个任务
             * 方法是将当前任务从队列的头部移除，插入到尾部
             * 这样后面执rtos_task_sched()时就会从头部取出新的任务取出新的任务作为当前任务运行
             */
            if (rtos_task_list_count(&task_table[p_current_task->prio]) > 0)
            {
                rtos_task_list_remove_first(&task_table[p_current_task->prio]);
                rtos_task_list_add_tail(&task_table[p_current_task->prio], &(p_current_task->prio_node));

                /*  重置时间片计数器 */
                p_current_task->slice = RTOS_SLICE_MAX;
            }
        }
    }
   
    /* 退出临界区保护 */
    rtos_task_critical_exit(status);    
}

 
/**
 * \brief  操作系统滴答时钟初始化函数
 */
void rtos_systick_init (void)
{
    uint32_t sysclk = SystemCoreClock;
    
    __g_us_ticks =  sysclk / 1000000;
    __g_ms_ticks =  sysclk / 1000;

    __g_us_max   = (uint64_t)0xFFFFFF * (uint64_t)1000000 / sysclk;

    __g_ms_max   = __g_us_max / 1000; 
    
    __g_reload   = RTOS_SYSTICK_PERIOD * __g_ms_ticks;      
    
#ifdef ARMCM3 

    SysTick->LOAD  = __g_reload 
#endif

#ifdef STM32F429xx 
    /* SysTick频率为HCLK */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
    if (RTOS_SYSTICK_PERIOD >  __g_ms_max) {
        while(1) {
            ;
        }            
    } else {
        
        SysTick->LOAD  = __g_reload;
    }
     
#endif
    
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    SysTick->VAL   = 0;                           
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk; 
}

/** 
 * \brief 微秒级别延时函数
 */
void rtos_udelay (uint32_t nus)
{
    uint32_t ticks;
    uint32_t told = 0, tnow = 0,tcnt=0;
    
    ticks = nus * __g_us_ticks;     /* 需要的节拍数 */ 
    
    if (rtos_running_check()) {
       /* 调度锁关闭，禁止任务调度, 因为此时发生调度的话就跑去其它任务去了 */
       rtos_task_sched_disable();       
    }    
    
    /* 刚进入时的计数器值 */
    told = SysTick->VAL;
    
    while (1) {
        /* 当前的计数器值 */
        tnow = SysTick->VAL;
        
        if (tnow != told) {
            
            if (tnow < told) {
                tcnt+=told-tnow;    /* 这里注意一下SYSTICK是一个递减的计数器就可以了.*/
            } else {
                /* 当前滴答已经从0回到了重载计数值重新开始计数 */
                tcnt += __g_reload - tnow + told;
            }
            
            told=tnow;
            
            if(tcnt >= ticks) { 
                break; /* 时间超过/等于要延迟的时间,则退出. */
            }                
                   
        }
                   
    }

    if (rtos_running_check()) {
        /* 调度锁打开，允许任务调度 */
        rtos_task_sched_enable();      
    }    
}

/**
 * \brief  操作系统ms延时函数实现
 */
void rtos_mdelay (int32_t ms)
{ 
    uint32_t i = 0;
    uint32_t nms = 0;
    
    if (rtos_running_check()) {
        /* 延时的时间大于OS的最少时间周期 */
        if (ms > RTOS_SYSTICK_PERIOD) {            
            rtos_sched_mdelay(ms / RTOS_SYSTICK_PERIOD);            
        }      
        /* OS 已经无法提供这么小的延时了,采用普通方式延时 */
        nms %= RTOS_SYSTICK_PERIOD; 
        
        for (i = 0; i < nms; i++) {           
             rtos_udelay(1000);
        }          
             
    } else {
        
        for (i = 0; i < ms; i++) {           
             rtos_udelay(1000);
        }            
        
    } 
}



/**
 * \brief  操作系统滴答中断处理函数
 *
 * \param[in] 无 
 *
 * \return    无
 */
void SysTick_Handler (void) 
{  
    /* 当操作系统运行起来时才执行调度 */
    if (rtos_running_check() == 0) {
        
        return;
        
    }
    /* 临界区保户 */
    uint32_t status = rtos_task_critical_entry();
    
#ifdef STM32F429xx    
    /* stm32f4xx tick */
    HAL_IncTick();
#endif
     
    __rtos_task_delay_tick_handler();    
    
    /* 滴答计数 */
    rtos_systick++;   
    
#if RTOS_ENABLE_CPU_USE_CHECK == 1
   
    /* 检查cpu使用率 */
     rtos_cpu_use_check();   
    
#endif 
    
    /* 退出临界区保护 */
    rtos_task_critical_exit(status);
    
#if RTOS_ENABLE_TIMER == 1    
    /* 通知定时器模块节拍事件 */
    rtos_timer_moudule_tick_notify();
#endif

#if RTOS_ENABLE_HOOK == 1

    /* 系统滴答调用钩子函数 */
    rtos_hook_systick();

#endif
            
    /* 这个过程中可能有任务延时完毕(delayTicks = 0)，进行一次调度 */
    rtos_task_sched(); 
}


/**
 * \brief  滴答计数初始化
 */
void rtos_tick_count_init (void)
{
    
    rtos_systick = 0;   
}


 
 
 /* end of file */

