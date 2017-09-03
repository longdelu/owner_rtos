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
 * \brief ����ϵͳ�δ�ʱ��
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


/** \brief  ������ʱ���� */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief ͬһ�����ȼ����������ͷ��� */
extern rtos_task_list_t task_table[TASK_COUNT];

uint32_t rtos_systick = 0;      /**< \brief ϵͳ�δ����ֵ */

uint32_t __g_us_ticks = 0;      /**< \brief ����ʱ1us�ļ���ֵ   */
uint32_t __g_ms_ticks = 0;      /**< \brief ����ʱ1ms�ļ���ֵ   */

uint32_t __g_reload = 0;        /**< \brief systick���ؼ���ֵ   */  

uint32_t __g_us_max = 0;        /**< \brief ��ʱ�����us��      */
uint32_t __g_ms_max = 0;        /**< \brief ��ʱ�����ms��      */


  
/**
 * \brief  ������ʱ�δ���
 */
static void __rtos_task_delay_tick_handler (void)
{
    volatile uint32_t i = 0;
    
    volatile uint8_t slice_flag = 0;
    
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 
    
    rtos_task_t * p_task  = NULL;
       
    /*���ٽ���������*/
    uint32_t status = rtos_task_critical_entry();
       
    /* �����ʱ���е�һ���û���� */
    p_next  = dlist_begin_get(&rtos_task_delayedlist.head_node);
    
    /*  �����ʱ���еĽ���λ��: Ϊͷ��㱾�� */
    p_end  = dlist_end_get(&rtos_task_delayedlist.head_node);
    
    
     /* �������������ʱ�б�ʱ�����������delayTicks���������0�Ļ�����1 */
    while(p_next != p_end) {   

        /* �ȼ�¼�µ�ǰ��� */
        p_tmp =  p_next;
           
        /* �ȼ�¼��һ��������Ϣ */
        p_next = dlist_next_get(&rtos_task_delayedlist.head_node, p_next);
        
        
        p_task = RTOS_CONTAINER_OF(p_tmp, rtos_task_t, delay_node);
        
        if (--p_task->delay_ticks <= 0) {
            
             /* ������񻹴��ڵȴ��¼���״̬��������¼��ȴ������л��� */
            if (p_task->p_event) {
                
                /* ��ʱ����ϢΪ�գ��ȴ����Ϊ��ʱ */
                rtos_task_event_del(p_task, NULL, -RTOS_TIMEOUT);
                
            }
            
            /* ���������ʱ�������Ƴ� */
            rtos_task_wake_up_delayed_list(p_task);
            
            /* ������Ǽǵ������б��� */
            rtos_task_sched_ready(p_task);
        }
        
              
    }
    
    /* ��ȥ��ǰ��ʱ��Ƭ���� */
    --p_current_task->slice;
    
    /*��������һ�δ�����Ҫ�п��ܻ�����ʱ���е��е�һЩ���񣬵��п��ܸ���������񲢲���ǰ�ĸߣ�Ҳ���������л���ȥ */
    
    /* ���ͬһ���ȼ��������е�һ���û���� */
    p_next  = dlist_begin_get(&task_table[p_current_task->prio].head_node);
    
    /* ���ͬһ���ȼ��������н���λ��,Ϊͷ��㱾�� */
    p_end  = dlist_end_get(&task_table[p_current_task->prio].head_node);
    
     /* �������ͬһ���ȼ��������а����������жϵ�ǰ�����Ƿ������� */
    while(p_next != p_end) {  
        
        /* �ȼ�¼�µ�ǰ��� */
        p_tmp =  p_next;
           
        /* �ȼ�¼��һ��������Ϣ */
        p_next = dlist_next_get(&task_table[p_current_task->prio].head_node, p_next);
        
        
        p_task = RTOS_CONTAINER_OF(p_tmp, rtos_task_t, prio_node);
        
        
        /* ������������ */
        if (p_task == p_current_task){
            
            slice_flag = 1;

            
            break;

        }            
        
    }

    /* ����µ�ǰ�����ʱ��Ƭ�Ƿ��Ѿ�����,����ȷ������������ڸ����ȼ������������� */
    if (slice_flag == 1) {  
    
        /* ����µ�ǰ�����ʱ��Ƭ�Ƿ��Ѿ�����,����ȷ������������ھ����������� */
        if (p_current_task->slice == 0) {
            /*
             * �����ǰ�����л�����������Ļ�����ô�л�����һ������
             * �����ǽ���ǰ����Ӷ��е�ͷ���Ƴ������뵽β��
             * ��������ִrtos_task_sched()ʱ�ͻ��ͷ��ȡ���µ�����ȡ���µ�������Ϊ��ǰ��������
             */
            if (rtos_task_list_count(&task_table[p_current_task->prio]) > 0)
            {
                rtos_task_list_remove_first(&task_table[p_current_task->prio]);
                rtos_task_list_add_tail(&task_table[p_current_task->prio], &(p_current_task->prio_node));

                /*  ����ʱ��Ƭ������ */
                p_current_task->slice = RTOS_SLICE_MAX;
            }
        }
    }
   
    /* �˳��ٽ������� */
    rtos_task_critical_exit(status);    
}

 
/**
 * \brief  ����ϵͳ�δ�ʱ�ӳ�ʼ������
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
    /* SysTickƵ��ΪHCLK */
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
 * \brief ΢�뼶����ʱ����
 */
void rtos_udelay (uint32_t nus)
{
    uint32_t ticks;
    uint32_t told = 0, tnow = 0,tcnt=0;
    
    ticks = nus * __g_us_ticks;     /* ��Ҫ�Ľ����� */ 
    
    if (rtos_running_check()) {
       /* �������رգ���ֹ�������, ��Ϊ��ʱ�������ȵĻ�����ȥ��������ȥ�� */
       rtos_task_sched_disable();       
    }    
    
    /* �ս���ʱ�ļ�����ֵ */
    told = SysTick->VAL;
    
    while (1) {
        /* ��ǰ�ļ�����ֵ */
        tnow = SysTick->VAL;
        
        if (tnow != told) {
            
            if (tnow < told) {
                tcnt+=told-tnow;    /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.*/
            } else {
                /* ��ǰ�δ��Ѿ���0�ص������ؼ���ֵ���¿�ʼ���� */
                tcnt += __g_reload - tnow + told;
            }
            
            told=tnow;
            
            if(tcnt >= ticks) { 
                break; /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�. */
            }                
                   
        }
                   
    }

    if (rtos_running_check()) {
        /* �������򿪣������������ */
        rtos_task_sched_enable();      
    }    
}

/**
 * \brief  ����ϵͳms��ʱ����ʵ��
 */
void rtos_mdelay (int32_t ms)
{ 
    uint32_t i = 0;
    uint32_t nms = 0;
    
    if (rtos_running_check()) {
        /* ��ʱ��ʱ�����OS������ʱ������ */
        if (ms > RTOS_SYSTICK_PERIOD) {            
            rtos_sched_mdelay(ms / RTOS_SYSTICK_PERIOD);            
        }      
        /* OS �Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ */
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
 * \brief  ����ϵͳ�δ��жϴ�����
 *
 * \param[in] �� 
 *
 * \return    ��
 */
void SysTick_Handler (void) 
{  
    /* ������ϵͳ��������ʱ��ִ�е��� */
    if (rtos_running_check() == 0) {
        
        return;
        
    }
    /* �ٽ������� */
    uint32_t status = rtos_task_critical_entry();
    
#ifdef STM32F429xx    
    /* stm32f4xx tick */
    HAL_IncTick();
#endif
     
    __rtos_task_delay_tick_handler();    
    
    /* �δ���� */
    rtos_systick++;   
    
#if RTOS_ENABLE_CPU_USE_CHECK == 1
   
    /* ���cpuʹ���� */
     rtos_cpu_use_check();   
    
#endif 
    
    /* �˳��ٽ������� */
    rtos_task_critical_exit(status);
    
#if RTOS_ENABLE_TIMER == 1    
    /* ֪ͨ��ʱ��ģ������¼� */
    rtos_timer_moudule_tick_notify();
#endif

#if RTOS_ENABLE_HOOK == 1

    /* ϵͳ�δ���ù��Ӻ��� */
    rtos_hook_systick();

#endif
            
    /* ��������п�����������ʱ���(delayTicks = 0)������һ�ε��� */
    rtos_task_sched(); 
}


/**
 * \brief  �δ������ʼ��
 */
void rtos_tick_count_init (void)
{
    
    rtos_systick = 0;   
}


 
 
 /* end of file */

