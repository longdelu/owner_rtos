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
 * \brief RTOS �����ʱ��ԭ��ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#include "rtos_soft_timer.h"
#include "rtos_sem.h"

static  rtos_task_list_t __rtos_timer_hard_list;
static  rtos_task_list_t __rtos_timer_soft_list;

/** \brief ���ڷ�����ʱ���б���ź��� */
static rtos_sem_t __rtos_softtimer_sem;

/** \brief ���ڷ�����ʱ���б���ź��� */
static rtos_sem_t __rtos_timer_tick_sem;


/**
 * \brief ��ʼ����ʱ��
 */
void rtos_timer_init (rtos_timer_t * p_timer, 
                      uint32_t start_delay_ticks, 
                      uint32_t duration_ticks,
                      void (*pfn_timer_func)(void *p_arg), 
                      void *p_arg, 
                      uint32_t config)
{
    
    dlist_init(&p_timer->timer_node);
    p_timer->start_delay_ticks = start_delay_ticks;
    p_timer->duration_ticks =  duration_ticks;
    
    p_timer->pfn_timer_func = pfn_timer_func;
    p_timer->p_arg          = p_arg;
    p_timer->config         = config;
    
    if (start_delay_ticks == 0) {
        p_timer->delay_ticks = duration_ticks;
             
    } else {
        p_timer->delay_ticks = start_delay_ticks;
    }
    
    
    p_timer->state = RTOS_TIMER_CREATED;
     
}

/**
 * \brief ������ʱ��
 */
void rtos_timer_start (rtos_timer_t * p_timer)
{
    uint32_t status  = 0;
    
    switch ((int32_t)p_timer->state) {
    case RTOS_TIMER_CREATED:
    case RTOS_TIMER_STOPPED:
        p_timer->delay_ticks = (p_timer->start_delay_ticks > 0) ? p_timer->start_delay_ticks :p_timer->duration_ticks;
        p_timer->state = RTOS_TIMER_STARTED;
    
        /* ���ݶ�ʱ�����ͼ�����Ӧ�Ķ�ʱ���б� */
        if (p_timer->config & TIMER_CONFIG_TYPE_HARD) {
           
           /* Ӳ��ʱ������ʱ�ӽ����ж��д�������ʹ��critical������*/    
           status = rtos_task_critical_entry();

           /* ���뵽Ӳ����ʱ���б� */
           rtos_task_list_add_tail(&__rtos_timer_hard_list, &p_timer->timer_node); 

           /* �˳��ٽ��� */
           rtos_task_critical_exit(status);           
   
        } else {
            
            /* 
             * ��ȡ��ʱ���б�ķ���Ȩ��
             * ��ʱ�����Ȼ�ȡ�ź������Դ����ʱ��ʱ�������ʱͬʱ�ڷ�����ʱ���б��µĳ�ͻ����
             */ 
            rtos_sem_wait(&__rtos_softtimer_sem, 0);
            
            /* ������ʱ���б� */ 
            rtos_task_list_add_tail(&__rtos_timer_soft_list, &p_timer->timer_node);
            
            /* �ͷŶ�ʱ���б����Ȩ�� */ 
            rtos_sem_notify(&__rtos_softtimer_sem);            
           
        }
       
        break;
        
    default:
        break;    
        
    }
}


/**
 * \brief ������ʱ��
 */
void rtos_timer_stop (rtos_timer_t * p_timer)
{
    uint32_t status  = 0;
    
    switch ((int32_t)p_timer->state) {
    case RTOS_TIMER_RUNNING:
    case RTOS_TIMER_STARTED:
    
        /* ����Ѿ��������ж϶�ʱ�����ͣ�Ȼ�����Ӧ����ʱ�б����Ƴ� */
        if (p_timer->config & TIMER_CONFIG_TYPE_HARD) {
           
           /* Ӳ��ʱ������ʱ�ӽ����ж��д�������ʹ��critical������*/    
           status = rtos_task_critical_entry();

           /* ��Ӳ��ʱ���б����Ƴ� */
           rtos_task_list_remove(&__rtos_timer_hard_list, &p_timer->timer_node); 

           /* �˳��ٽ��� */
           rtos_task_critical_exit(status);           
   
        } else {
            
            /* 
             * ��ȡ��ʱ���б�ķ���Ȩ��
             * ��ʱ�����Ȼ�ȡ�ź������Դ����ʱ��ʱ�������ʱͬʱ�ڷ�����ʱ���б��µĳ�ͻ����
             */ 
            rtos_sem_wait(&__rtos_softtimer_sem, 0);
            
            /* ������ʱ���б� */ 
            rtos_task_list_remove(&__rtos_timer_soft_list, &p_timer->timer_node);
            
            /* �ͷŶ�ʱ���б����Ȩ�� */ 
            rtos_sem_notify(&__rtos_softtimer_sem);            
           
        }
        
        p_timer->state = RTOS_TIMER_STOPPED;
       
        break;
        
    default:
        break;    
        
    }
}

/**
 * \brief ���ٶ�ʱ��
 */
void rtso_timer_destroy (rtos_timer_t * p_timer)    
{   
    /* ��ֹͣ��ʱ�� */
    rtos_timer_stop(p_timer);
   
    p_timer->state = RTOS_TIMER_DESTROYED;
 
}

/**
 * \brief ��ѯ��ʱ��״̬��Ϣ
 */
void rtos_timer_info_get (rtos_timer_t *p_timer, rtos_timer_info_get_t *p_info)    
{
    uint32_t status = 0; 
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
     status = rtos_task_critical_entry(); 
    
     p_info->config  =  p_timer->config;
     p_info->start_delay_ticks = p_timer->start_delay_ticks;
     p_info->duration_ticks =  p_timer->duration_ticks;
     p_info->p_arg = p_timer->p_arg;
     p_info->pfn_timer_func = p_timer->pfn_timer_func;
     p_info->state = p_timer->state;
    
    
     /* �˳��ٽ��� */
     rtos_task_critical_exit(status); 
 
}




/** \brief �����ջ */
taskstack_t __rtos_timer_task_stack_buf[TIMER_TASK_STACK_SIZE];

/** \brief ��ǰ����ṹ�� */
rtos_task_t __rtos_timer_task_task;


/**
 * \brief ��ʱ����������
 */
static void rtos_timersoft_list_call_fuc (rtos_task_list_t *p_softtimer_list) 
{
    dlist_node_t *p_timer_node = NULL;
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 
    
    rtos_timer_t * p_softtimer = NULL;
    
        /* �����ʱ���е�һ���û���� */
    p_next  = dlist_begin_get(&p_softtimer_list->head_node);
    
    /*  �����ʱ���еĽ���λ��: Ϊͷ��㱾�� */
    p_end  = dlist_end_get(&p_softtimer_list->head_node);
    
    

    /* ������ж�ʱ����delay_ticks���������0�Ļ�����1��*/
    while(p_next != p_end) {   

        /* �ȼ�¼�µ�ǰ��� */
        p_timer_node =  p_next;
           
        /* �ȼ�¼��һ��������Ϣ */
        p_next = dlist_next_get(&p_softtimer_list->head_node, p_next);
        
           
        p_softtimer = RTOS_CONTAINER_OF(p_timer_node, rtos_timer_t, timer_node);
        
        /* �����ʱ�ѵ�������ö�ʱ�������� */
        if ((p_softtimer->delay_ticks == 0) || (--p_softtimer->delay_ticks == 0)) {
            
            /* �л�Ϊ��������״̬ */
            p_softtimer->state = RTOS_TIMER_RUNNING;
            
            /* ���ö�ʱ�������� */
            p_softtimer->pfn_timer_func(p_softtimer->p_arg);
            
            /* �л�Ϊ�Ѿ�����״̬ */
            p_softtimer->state = RTOS_TIMER_STARTED;
            
            
            if (p_softtimer->duration_ticks > 0) {
                
                /* ����������Եģ����ظ���ʱ����ֵ */
                p_softtimer->delay_ticks = p_softtimer->duration_ticks;
            } else {
                /* ������һ���Լ���������ֹ��ʱ�� */
                rtos_task_list_remove(p_softtimer_list, &p_softtimer->timer_node);
                p_softtimer->state = RTOS_TIMER_STOPPED;
            }
            
        }

        
    }
    
}

/**
 * \brief �����ʱ��������ں���
 */
static void rtos_timersoft_task (void *p_parg) 
{
    
    for (; ;) {
        
        /* �ȴ�ϵͳ���ķ��͵��ж��¼��ź� */  
        rtos_sem_wait(&__rtos_timer_tick_sem, 0);
        
        /* ��ȡ��ʱ���б�ķ���Ȩ�� */ 
        rtos_sem_wait(&__rtos_softtimer_sem, 0);
        
        /* ������ʱ���б� */ 
        rtos_timersoft_list_call_fuc(&__rtos_timer_soft_list);
        
        /* �ͷŶ�ʱ���б����Ȩ�� */ 
        rtos_sem_notify(&__rtos_softtimer_sem);
        
            
    }
    
}

/**
 * \brief ֪ͨϵͳ�δ�ʱ���Ѿ������ˣ���Ϊ������Ӳ���жϴ������е��õģ�Ҳ�������������Ӳ����ʱ���Ĵ���
 */
void rtos_timer_moudule_tick_notify (void)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    rtos_timersoft_list_call_fuc(&__rtos_timer_hard_list);
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);   

    /* ֪ͨ�����ʱ�����ı仯 */ 
    rtos_sem_notify(&__rtos_timer_tick_sem);
     
}

/**
 * \brief ��ʱ��ģ���ʼ������
 */
void  rtos_timer_moudule_init (void) 
{
    /* ��ʱ���б��ʼ�� */
    rtos_task_list_init(&__rtos_timer_soft_list);
    rtos_task_list_init(&__rtos_timer_hard_list);  

    
    /* ����ź�����ʼ�� */    
    rtos_sem_init(&__rtos_timer_tick_sem, 0, 0);
    rtos_sem_init(&__rtos_softtimer_sem, 1, 1);    
    
#if RTOS_TIMER_TASK_PRIO >= (RTOS_PRIO_COUNT - 1)
    #error "The proprity of timer task must be greater then (RTOS_PRIO_COUNT - 1)"
#endif
    
    
    /* ��ʱ�������ʼ������ */
    rtos_task_init(&__rtos_timer_task_task, rtos_timersoft_task, NULL, RTOS_TIMER_TASK_PRIO, __rtos_timer_task_stack_buf, sizeof(__rtos_timer_task_stack_buf)); 
       
}




/* end of file */




