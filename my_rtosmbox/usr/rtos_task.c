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
 * \brief ����ϵͳ������غ���ʵ��
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

/** \brief �������ȼ��ı��λ�ýṹȫ���� */
extern rtos_task_bitmap_t task_priobitmap;

/** \brief  ������ʱ���� */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief ͬһ�����ȼ����������ͷ��� */
extern rtos_task_list_t task_table[TASK_COUNT];


/**
 * \brief �����ʼ��
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
     * ע���������㣺
     * 1������Ҫ�õ��ļĴ�����ֱ�����˼Ĵ����ţ�������IDE����ʱ�鿴Ч����
     * 2��˳���ܱ䣬Ҫ���PendSV_Handler�Լ�CPU���쳣�Ĵ������������
     *
     */
    *(--p_task_stack_top) = (unsigned long)(1<<24);                // XPSR, ������Thumbģʽ���ָ���Thumb״̬����ARM״̬����
    *(--p_task_stack_top) = (unsigned long)task_entry;             // �������ڵ�ַPC(R15)
    *(--p_task_stack_top) = (unsigned long)0x14;                   // R14(LR), ���񲻻�ͨ��return xxx�����Լ�������δ��
    *(--p_task_stack_top) = (unsigned long)0x12;                   // R12, δ��
    *(--p_task_stack_top) = (unsigned long)0x3;                    // R3, δ��
    *(--p_task_stack_top) = (unsigned long)0x2;                    // R2, δ��
    *(--p_task_stack_top) = (unsigned long)0x1;                    // R1, δ��
    *(--p_task_stack_top) = (unsigned long)p_arg;                  // R0 = param, �����������ں���
    *(--p_task_stack_top) = (unsigned long)0x11;                   // R11, δ��
    *(--p_task_stack_top) = (unsigned long)0x10;                   // R10, δ��
    *(--p_task_stack_top) = (unsigned long)0x9;                    // R9, δ��
    *(--p_task_stack_top) = (unsigned long)0x8;                    // R8, δ��
    *(--p_task_stack_top) = (unsigned long)0x7;                    // R7, δ��
    *(--p_task_stack_top) = (unsigned long)0x6;                    // R6, δ��
    *(--p_task_stack_top) = (unsigned long)0x5;                    // R5, δ��
    *(--p_task_stack_top) = (unsigned long)0x4;                    // R4, δ��
    
    
    task->task_stack_top = p_task_stack_top;                       // �������յ�ֵ,Ϊջ���ĵ�ַ

    task->delay_ticks    = 0;                                      // ������ʱ��Ƭ
       
    task->prio           = task_prio;                              // ������������ȼ� 
    
    task->slice          = RTOS_SLICE_MAX;                         // ��ʼ�������ʱ��Ƭ����
    
    task->task_state     = RTOS_TASK_STATE_REDDY;                  // ��������Ϊ����״̬ 
    
    task->suspend_cnt    = 0;                                      // ��ʼ�������Ϊ0
    
    
    task->clean_param    = NULL;                                   // ���ô��ݸ��������Ĳ���
    task->pfn_clean      = NULL;                                   // ����������
    task->req_delete_flag = 0;                                     // ����ɾ�����
    
    dlist_init(&task->delay_node);                                 // ��ʼ����ʱ���н��
    dlist_init(&task->prio_node);                                  // ��ʼ��ͬһ���ȼ�������н�㣬���ھ�����
    dlist_init(&task->event_node);                                 // ��ʼ���ȴ��¼���ʱ���  
    
    task->p_event = NULL;                                          // û�еȴ��¼�       
    task->p_event_msg = NULL;                                      // û�д洢�ȴ��¼�����Ϣ
    task->event_wait_result  =  RTOS_EVENT_TYPE_UNKNOW;            // û�еȴ��¼�����
    
    
    /*
     * \note ���øú�����һ��Ҫ��֤�����ȼ�������ͷ����Ѿ�����ȷ��ָ��
     */
    rtos_task_list_add_tail(&task_table[task_prio], &(task->prio_node));  // �����Ӧ�����ȼ�������
    
    rtos_task_bitmap_set(&task_priobitmap, task_prio);             // ������ȼ�λ���е���Ӧλ
} 

/**
 * \brief ����������Ϊ����״̬                                     
 */
void rtos_task_sched_ready(rtos_task_t *task)
{
    /* ������ͬһ���ȼ��б�β���������� */
    rtos_task_list_add_tail(&task_table[task->prio], &(task->prio_node)); 
    
    rtos_task_bitmap_set(&task_priobitmap,task->prio); 
} 


/**
 * \brief ����������б����Ƴ�                                    
 */
void rtos_task_sched_unready(rtos_task_t *task)
{
    /* ͬһ���ȼ������񲻿���ͬʱ����������ʱ���м��������ȼ�����(�������У����� */
    rtos_task_list_remove(&task_table[task->prio], &(task->prio_node));
    
    if (rtos_task_list_count(&task_table[task->prio]) == 0) {
    
        /* ֻ�ܸ����ȼ�������������û���κ�����ʱ������������� */
        rtos_task_bitmap_clr(&task_priobitmap,task->prio);
    }        
}

/**
 * \brief ��ʼ�����ȼ������������                                    
 */
void rtos_task_sched_init(rtos_task_list_t *p_task_table)
{
    
    volatile uint32_t i = 0;
    
    for (i = 0; i < RTOS_PRIO_COUNT; i++) {
         rtos_task_list_init(&p_task_table[i]);
    }    
}  


/**
 * \brief ��ȡ��ǰ������ȼ��ҿ����е�����                                     
 */
rtos_task_t *rtos_task_highest_ready(void) 
{
    uint32_t highestPrio = rtos_task_bitmap_first_set_get(&task_priobitmap);
    
    dlist_node_t *p_node = rtos_task_list_begin_get(&task_table[highestPrio]);
    
    /* �������ȼ������������ǰ����Ǹ����� */
    return RTOS_CONTAINER_OF(p_node, rtos_task_t, prio_node);;
}    



/**
 * \brief �л�����һ���������У���������ϵͳ��һ������
 *
 * \note  ���жϵ�ǰ�л���tinyOS����ʱ�л�����1�����񣬻��Ƕ������Ѿ���������ִ�е��л�
 *
 */
void rtos_task_run_first (void)
{
    /* ���������ʹ�õ�����ջMSP(��Ȩ����*/  
    __set_PSP(0);
    
    PendSVC_Trigger();
    
}

/**
 * \brief �������
 */
void rtos_task_sched(void)
{
    rtos_task_t *p_temp_task = NULL;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* ��������ʱ����ʱ���������������ȣ�ֱ�ӷ��� */
    if (rtos_task_schedlock_status()) {
        
        rtos_task_critical_exit(status);
        return;
        
    }
    
    
   /* �ҵ���ʱ���������ȼ���ߵ�������ô���л���������� */
    p_temp_task = rtos_task_highest_ready();    
        
    /* p_current_task �� p_next_task �������߻���PendSVC�쳣��������������ֵ */
    if (p_current_task !=  p_temp_task) {
                
        p_next_task =  p_temp_task;    

        /* ����PendSVC�쳣�����������л� */
        rtos_task_switch();
        
    }
        
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
    
}


/**
 * \brief ��ʼ��������ʱ����
 */
void rtos_task_delayed_init (rtos_task_list_t *p_rtos_task_delayed_list)
{
     rtos_task_list_init(p_rtos_task_delayed_list);  
}


/**
 * \brief ��������뵽��ʱ������
 */
void rtos_task_add_delayed_list (rtos_task_t *p_task, uint32_t delay_ticks)
{
    p_task->delay_ticks = delay_ticks;
    
    rtos_task_list_add_tail(&rtos_task_delayedlist, &p_task->delay_node);

    p_task->task_state |= RTOS_TASK_STATE_DELAYED;

    
   /*����ʾ��ɾ�����������¼��뵽��ʱ������,����������ӵ�е�״̬ */
   if ( p_task->task_state & RTOS_TASK_STATE_RED_DEL) {
       
       p_task->task_state &= ~RTOS_TASK_STATE_RED_DEL;  
   }
          
}


/**
 * \brief ���������ʱ�����л���
 */
void rtos_task_wake_up_delayed_list (rtos_task_t *p_task)
{
    rtos_task_list_remove(&rtos_task_delayedlist, &p_task->delay_node);

    p_task->task_state &= ~RTOS_TASK_STATE_REDDY;    
           
}


/**
 * \brief ����ʱ���������ʱ������ɾ��,
 */
void rtos_task_del_delayed_list (rtos_task_t *p_task)
{
    
    rtos_task_list_remove(&rtos_task_delayedlist, &p_task->delay_node);

    p_task->task_state |= RTOS_TASK_STATE_RED_DEL;       
}


/**
 * \brief ��������
 */
void rtos_task_suspend (rtos_task_t *p_task)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /** 
     * 
     *\note �������Ʋ��Ե����⣬��ʱ������Ҳ�ǿ�����������,��˼·��
     *       �������ǣ�����ʱ���������ӵ���������ʱ��������ǣ��б��
     *       ���ڴ�ʱ��������Ǽ���������С�
     */ 
    
    /* ��������Ѿ�������ʱ״̬��������� */
    if (!(p_task->task_state & RTOS_TASK_STATE_DELAYED)) 
    {
        /* 
         *  ���ӹ������������������ִ�е�һ�ι������ʱ���ſ����Ƿ�
         *  Ҫִ�������л�����
         */
        if (p_task->suspend_cnt == 0)
        {
           
            /* �����������Ϊ4294967295UL */
            if (p_task->suspend_cnt <= 4294967295UL) {
                p_task->suspend_cnt++;                
            }
            /*  ���ù����־ */
            p_task->task_state |= RTOS_TASK_STATE_SUSPEND;

            /* 
             * ����ʽ�ܼ򵥣����ǽ���Ӿ����������Ƴ��������������Ͳ��ᷢ����
             * Ҳ��û���л�������������
             */
            rtos_task_sched_unready(p_task);

            /* ��Ȼ�������������������Լ�����ô���л����������� */
            if (p_task == p_current_task)
            {
                rtos_task_sched();
            }
        }
    }
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
           
}


/**
 * \brief ���ѹ�������ֻ�ǰ�������뵽ͬһ���ȼ������������β����������֤���뵽��ǰ�Ķ�Ӧ��λ��
 */
void rtos_task_wakeup (rtos_task_t *p_task)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
    
    /* ��������Ƿ��ڹ���״̬ */
    if (p_task->task_state & RTOS_TASK_STATE_SUSPEND) {
        
        /* �ݼ�������������Ϊ0�ˣ�����������־��ͬʱ���ý������״̬ */
        if (--p_task->suspend_cnt == 0) {
            
            /*  ��������־ */
            p_task->task_state &= ~RTOS_TASK_STATE_SUSPEND;
            
            /* 
             * ͬʱ������Żؾ���������
             */
            rtos_task_sched_ready(p_task);
            
            /* ȡ������Ĺ����У������и������ȼ������������ִ��һ��������� */
            rtos_task_sched();
            
        }
        
    }

        

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);     
}

/**
 * \brief ��������ɾ��ʱ���õ�������
 */
void rtos_task_set_clean_call_fuc (rtos_task_t *p_task, void (*pfn_clean)(void * p_par), void * p_par)
{
    p_task->clean_param =  p_par;
    p_task->pfn_clean = pfn_clean;
        
}


/**
 * \brief ǿ��ɾ��ָ��������ֻ��ɾ��һ�θ����񣬲����ڵ��øú������ٵ���ɾ��������
 */
void rtos_task_force_del (rtos_task_t *p_task)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
    
    /*  �����������ʱ״̬�������ʱ������ɾ�� */
    if (p_task->task_state & RTOS_TASK_STATE_DELAYED) {
        rtos_task_del_delayed_list(p_task);
    }
    
    /* ������񲻴��ڹ���״̬����ô���Ǿ���̬���Ӿ�������ɾ�� */
    if (p_task->task_state & RTOS_TASK_STATE_SUSPEND) {
        
         rtos_task_sched_unready(p_task);
    }
    
    /* ɾ��ʱ������������������������������ */
    if (p_task->pfn_clean) {
        p_task->pfn_clean(p_task->clean_param);
    }
    
    /* ���ɾ�������Լ�����ô��Ҫ�л�����һ����������ִ��һ��������� */
    if (p_current_task == p_task) {
        /* ȡ������Ĺ����У������и������ȼ������������ִ��һ��������� */
        rtos_task_sched();
    }
        
           
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
        
}


/**
 * \brief ����ɾ��ĳ�������������Լ������Ƿ�ɾ���Լ�
 */
void rtos_task_req_del (rtos_task_t *p_task)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
 
    p_task->req_delete_flag = 1;

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
        
}


/**
 * \brief ��ѯ�Ƿ��Ѿ�������ɾ������
 */
uint32_t rtos_task_req_del_flag_check (void)
{
    uint32_t req_del_flag = 0;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
 
    /* ��ȡ����ɾ���ı�� */
    req_del_flag = p_current_task->req_delete_flag;

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
    
    
    return req_del_flag;
        
}

/**
 * \brief ����ɾ������(ֻ��ɾ��һ������
 */
void rtos_task_del_self (void)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
 
    /*
     * �����ڵ��øú���ʱ�������Ǵ��ھ���״̬�������ܴ�����ʱ����������״̬
     * ���ԣ�ֻ��Ҫ�Ӿ����������Ƴ�����   
     */    
    rtos_task_sched_unready(p_current_task);
    
    
    /* ɾ��ʱ������������������������������ */
    if (p_current_task->pfn_clean) {
        p_current_task->pfn_clean(p_current_task->clean_param);
    }
    
    /*���������϶�ʱ�л�����������ȥ���� */
    rtos_task_sched(); 

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
           
}


/**
 * \brief ��ȡ���������Ϣ
 */
void rtos_task_info_get (rtos_task_t *p_task, rtos_task_info_t *p_task_info)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
      
    p_task_info->delay_ticks = p_task->delay_ticks;         // ��ʱ��Ϣ
    p_task_info->task_prio   = p_task->prio;                // �������ȼ�
    p_task_info->task_state  = p_task->task_state;          // ����״̬
    p_task_info->task_slice  = p_task->slice;               // ʣ��ʱ��Ƭ
    p_task_info->suspend_cnt = p_task->suspend_cnt;         // ������Ĵ���

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
           
}

     
    










    




/* end of file */

