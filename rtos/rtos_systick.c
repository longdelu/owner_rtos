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


/** \brief  ������ʱ���� */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief ͬһ�����ȼ����������ͷ��� */
extern rtos_task_list_t task_table[TASK_COUNT];


/** \brief ϵͳ�δ����ֵ */
uint32_t rtos_systick = 0;

  
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
        
        if (--p_task->delay_ticks == 0) {
            
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
void rtos_systick_init (uint32_t ms)
{
    SysTick->LOAD  = ms * SystemCoreClock / 1000 - 1; 
    NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);
    SysTick->VAL   = 0;                           
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                     SysTick_CTRL_TICKINT_Msk   |
                     SysTick_CTRL_ENABLE_Msk; 
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
    /* �ٽ������� */
    uint32_t status = rtos_task_critical_entry();
          
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

