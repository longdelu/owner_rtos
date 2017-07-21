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
    
    dlist_init(&task->delay_node);                                 // ��ʼ����ʱ����
    dlist_init(&task->prio_node);                                  // ��ʼ��ͬһ���ȼ��������
    
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
    
    rtos_task_list_add_tail(&task_table[task->prio], &(task->prio_node)); 
    
    rtos_task_bitmap_set(&task_priobitmap,task->prio); 
} 


/**
 * \brief ����������б����Ƴ�                                    
 */
void rtos_task_sched_unready(rtos_task_t *task)
{
    /* ͬһ���ȼ������񲻿���ͬʱ����������ʱ���м��������ȼ����е��� */
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
           
}


/**
 * \brief ���������ʱ�����л���
 */
void rtos_task_del_delayed_list (rtos_task_t *p_task)
{
    rtos_task_list_remove(&rtos_task_delayedlist, &p_task->delay_node);

    p_task->task_state &= ~RTOS_TASK_STATE_REDDY;    
           
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
 * \brief ��������
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



    




/* end of file */

