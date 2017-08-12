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
 * \brief RTOS �����ź������ʵ��
 * \note �����ź�����֧��Ƕ�׵���
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_sem.h"
 #include "rtos_task.h"
 
 /**
 * \brief ��ʼ�������ź�����start_count�������ź���һ��ʼ���м��������ź�������
 */  
void rtos_sem_init(rtos_sem_t *p_sem, uint32_t start_count, uint32_t max_count)
{
    rtos_task_event_init(&p_sem->sem_event, RTOS_EVENT_TYPE_SEM);
    
    if (max_count == 0) {
        
        p_sem->sem_count =  start_count;
        
    } else {
        
        p_sem->sem_count =  (start_count > max_count) ? max_count : start_count;       
    }
                 
}


/**
 * \brief �ȴ������ź���
 */  
int32_t rtos_sem_wait (rtos_sem_t *p_sem, uint32_t wait_ticks)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    if (p_sem->sem_count > 0) {
        
        --p_sem->sem_count;
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 

        return RTOS_OK;     
        
    } else {
        
        rtos_task_event_wait(p_current_task, &p_sem->sem_event, NULL, RTOS_EVENT_TYPE_SEM, wait_ticks); 
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 
        
        /* �����ִ��һ���¼����ȣ��Ա����л����������� */
        rtos_task_sched(); 
        
        /* �����ڵȴ���ʱ���߼����ź���ִ��rtos_sem_notify����ʱ��ִ�л᷵�ص����Ȼ��ȡ���ȴ���� */      
        return  p_current_task->event_wait_result;
    } 
}

/**
 * \brief ��ȡ�ź���������ź������������ã��������˻�
 */  
int32_t rtos_sem_get (rtos_sem_t *p_sem)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    
    if (p_sem->sem_count > 0) {
        
        --p_sem->sem_count;
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 

        return RTOS_OK; 
        
    }  else {
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 

        return -RTOS_UNUSED;           
    }  
}


/**
 * \brief ֪ͨ�ź������ã����ѵȴ������е�һ�����񣬻��߽�����+1
 */  
void rtos_sem_notify (rtos_sem_t *p_sem)
{
     /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* ����Ƿ�������ȴ� */
    if (rtos_event_wait_count(&p_sem->sem_event) > 0){
        /* ����еĻ�����ֱ�ӻ���λ�ڶ����ײ������ȵȴ���������*/
        rtos_task_t * task = rtos_task_first_event_wake_up(&p_sem->sem_event, (void *)0, RTOS_OK );

        /* ��������������ȼ����ߣ���ִ�е��ȣ��л���ȥ */
        if (task->prio < p_current_task->prio) {
             rtos_task_sched(); 
        }
    } else {
        /* ���û������ȴ��Ļ������Ӽ��� */
        if ((uint64_t)p_sem->sem_count < 0xFFFFFFFF) {
            
            ++p_sem->sem_count;
        } else {
            p_sem->sem_count = 0xFFFFFFFF;
        }

        // �����������������������ļ�������ݼ�
        if ((p_sem->max_count != 0) && (p_sem->sem_count > p_sem->max_count)) {    
            p_sem->sem_count = p_sem->max_count;
        }
                  
    }
        
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);    
}

/**
 * \brief��ѯ�ź�����״̬��Ϣ
 */  
void rtos_sem_info_get (rtos_sem_t * p_sem, rtos_sem_info_t *p_info)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    p_info->count      =  p_sem->sem_count;
    p_info->max_count  =  p_sem->max_count;  

    /* �������ź������������� */    
    p_info->task_count =  rtos_event_wait_count(&p_sem->sem_event);
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
    
}

/**       
 * \brief �����ź���
 */  
uint32_t rtos_sem_destroy (rtos_sem_t *p_sem)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    /* ����¼����ƿ��е����� */
    uint32_t count  = rtos_task_event_all_remove(&p_sem->sem_event, NULL, -RTOS_DEL);  

    p_sem->sem_count = 0;    
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);  
    
    /* ��չ����п����и������ȼ����������ִ��һ�ε��� */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
    
    return count;   
}
 
 
 
 
 
 
 
 /* end of file */

