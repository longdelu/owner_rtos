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
 * \brief RTOS �����¼����ƿ����ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_task_event.h"
 #include "rtos_task.h"
 #include "rtos_task_critical.h"
 
 /**
 * \brief ��ʼ��RTOS�����¼����ƿ�
 */  
void rtos_task_event_init (rtos_task_event_t *p_event, rtos_task_event_type_t event_type)
{
    p_event->event_type = event_type;

    rtos_task_list_init(&p_event->event_wait_list);   
    
}


/**
 * \brief ��RTOS����ָ�����¼����ƿ��ϵȴ��¼�����
 */  
void rtos_task_event_wait (rtos_task_t *p_task, 
                           rtos_task_event_t *p_event, 
                           void *p_event_msg, 
                           uint32_t state, 
                           uint32_t time_out) 
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    
    p_task->task_state       |= state;         /* ��������ڵȴ�ĳ���¼���״̬ */ 
    p_task->p_event           = p_event;       /* ��������ȴ����¼��ṹ         */
    p_task->p_event_msg       = p_event_msg;   /* ��������ȴ��¼�����Ϣ�洢λ�� */   
    p_task->event_wait_result = RTOS_OK;       /* ����¼��ĵȴ����             */  
    
    /* ������Ӿ����������Ƴ� */
    rtos_task_sched_unready(p_task);
    
    /* ��������뵽�ȴ��¼������� */
    rtos_task_list_add_tail(&p_event->event_wait_list, &p_task->event_node);

    /*
     * ������������ó�ʱ����ͬʱ���뵽��ʱ������
     * ��ʱ�䵽��ʱ������ʱ������Ƹ����������ʱ�б����Ƴ���ͬʱ���¼��б����Ƴ�
     */
    if (time_out > 0) {
        
        /* ��������뵽��ʱ���� */
        rtos_task_add_delayed_list(p_task, time_out);
    }

    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
}    
    
/**
 * \brief ��ָ���¼����ƿ��ϻ����׸��ȴ���RTOS����
 */  
rtos_task_t  *rtos_task_event_wake_up (rtos_task_event_t *p_event, void *p_event_msg, int32_t event_result) 
{   
    dlist_node_t *p_event_node = NULL;
    
    rtos_task_t  *p_task = NULL;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();


    /* ȡ�����¼����ƿ�ȴ������еĵ�һ����� */
    if ((p_event_node = rtos_task_list_remove_first(&p_event->event_wait_list)) != NULL) {
        
         /* ת���ɶ�Ӧ������ṹ */
         p_task = RTOS_CONTAINER_OF(p_event_node, rtos_task_t, event_node);
        
         p_task->task_state       &= ~RTOS_TASK_EVENT_WAIT_MASK;         /* �������δ���ڵȴ�ĳ���¼���״̬ */ 
         p_task->p_event           = NULL;                               /* ��������ȴ����¼��ṹ         */
         p_task->p_event_msg       = p_event_msg;                        /* ��������ȴ��¼�����Ϣ�洢λ�� */   
         p_task->event_wait_result = event_result;                       /* �����¼��ĵȴ����             */  
        
        /* ���������˳�ʱ�ȴ����������£��������ʱ�������Ƴ� */
        if (p_task->delay_ticks != 0) {
            
            rtos_task_wake_up_delayed_list(p_task);
        }
        
        /* ���������������� */
        rtos_task_sched_ready(p_task) ;

    }        


    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);    

   return p_task;    
    

}

/**
 * \brief ��ָ���¼����ƿ���ɾ���ȴ���RTOS����
 */  
void rtos_task_event_del (rtos_task_t  *p_task, void *p_event_msg, int32_t event_result) 
{   
   
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
    
    /*
     * ����������ڵĵȴ��������Ƴ���ע�⣬����û�м��waitEvent�Ƿ�Ϊ�ա���Ȼ�Ǵ��¼����Ƴ�����ô��Ϊ�Ͳ�����Ϊ�� 
     */
    rtos_task_list_remove(&p_task->p_event->event_wait_list, &p_task->event_node); 


    {
        
         p_task->task_state       &= ~RTOS_TASK_EVENT_WAIT_MASK;         /* �������δ���ڵȴ�ĳ���¼���״̬ */ 
         p_task->p_event           = NULL;                               /* ��������ȴ����¼��ṹ         */
         p_task->p_event_msg       = p_event_msg;                        /* ��������ȴ��¼�����Ϣ�洢λ�� */   
         p_task->event_wait_result = event_result;                       /* �����¼��ĵȴ����             */  
        
        
        /* ���������������� */
        rtos_task_sched_ready(p_task) ;

    }        


    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);    
    

}  
    
    
 
 
 
 /* end of file */


