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
 * \brief RTOS �����ź���ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_mutex_sem.h"
 
 /**
 * \brief ��ʼ�������ź���
 */  
void rtos_mutex_sem_init (rtos_mutex_sem_t *p_mutex_sem)
{
    rtos_task_event_init(&p_mutex_sem->mutex_sem_event, RTOS_EVENT_TYPE_MUTEX_SEM);
    
    p_mutex_sem->p_task_ower = NULL;   

    p_mutex_sem->loced_count = 0;
    
    p_mutex_sem->task_ower_prior = RTOS_PRIO_COUNT;
}


/**
 * \brief �ȴ������ź���
 */  
int32_t rtos_mutex_sem_wait (rtos_mutex_sem_t *p_mutex_sem, uint32_t wait_ticks)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���p_current_task��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    if (p_mutex_sem->loced_count <= 0) {
        
        /* ���û����������ʹ�õ�ǰ�������� */
        p_mutex_sem->p_task_ower = p_current_task;
        p_mutex_sem->task_ower_prior =  p_current_task->prio;
        p_mutex_sem->loced_count++ ;
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 

        return RTOS_OK;  
        
    } else {
        
        /* �ź����Ѿ�������,�ź���ӵ����������һ��wait */
        if (p_mutex_sem->p_task_ower == p_current_task) {
            
            /* ������ź�����ӵ�����ٴ�wait�������Ӽ��� */
            p_mutex_sem->loced_count++; 
            
            /* �˳��ٽ��� */
            rtos_task_critical_exit(status); 

            return RTOS_OK; 
            
        } else {
            
            /*
             * ������ź���ӵ����֮�������wait����Ҫ������Ƿ���Ҫʹ��
             * ���ȼ��̳з�ʽ����
             */
            if (p_current_task->prio < p_mutex_sem->p_task_ower->prio) {
                
                /* ��Ҫ�������ȼ���ת���ѵ�ǰ��������ȼ���ߵ�����ĸ����ȼ���������ȼ�һ�� */
                
                rtos_task_t *p_tmp_task_ower = p_mutex_sem->p_task_ower;
                
                /*
                 * �����ǰ��������ȼ���ӵ�������ȼ����ߣ���ʹ�����ȼ��̳�
                 * ����ԭӵ���ߵ�����
                 */
                if (p_tmp_task_ower->task_state & RTOS_TASK_STATE_REDDY) {
                    
                    /* �����ھ���״̬ʱ�����������ھ������е�λ�� */
                    rtos_task_sched_unready(p_tmp_task_ower);
                    
                    /* �ı���ӵ����(��������ȼ� */
                    p_tmp_task_ower->prio =  p_current_task->prio;
                    
                    rtos_task_sched_ready(p_tmp_task_ower);
    
                    
                } else {
                
                    /* ����״̬��ֻ��Ҫ�޸����ȼ� */
                    p_tmp_task_ower->prio =  p_current_task->prio;
                      
                }
                
                        
            }
            
            /* ����ǰ��������¼��ȴ������� */
            rtos_task_event_wait(p_current_task, &p_mutex_sem->mutex_sem_event, NULL, RTOS_EVENT_TYPE_MUTEX_SEM, wait_ticks); 
            
            /* �˳��ٽ��� */
            rtos_task_critical_exit(status);

           /* ��ִ��һ���¼����ȣ��Ա����л����������� */
            rtos_task_sched();  
            
            
           /* �����ڵȴ���ʱ���߼����ź���ִ��rtos_mutex_sem_notify����ʱ��ִ�л᷵�ص����Ȼ��ȡ���ȴ���� */      
           return  p_current_task->event_wait_result;
      
        }            
    }
}

/**
 * \brief ��ȡ�����ź�������������ź��������ã��������˻�
 */  
int32_t rtos_mutex_sem_get (rtos_mutex_sem_t *p_mutex_sem)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���p_current_task��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    if (p_mutex_sem->loced_count <= 0) {
        
        /* ���û����������ʹ�õ�ǰ�������� */
        p_mutex_sem->p_task_ower = p_current_task;
        p_mutex_sem->task_ower_prior =  p_current_task->prio;
        p_mutex_sem->loced_count++;
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 

        return RTOS_OK;  
        
    } else {
        
        /* �ź����Ѿ�������,�ź���ӵ����������һ��wait */
        if (p_mutex_sem->p_task_ower == p_current_task) {
            
            /* ������ź�����ӵ�����ٴ�wait�������Ӽ��� */
            p_mutex_sem->loced_count++; 
            
            /* �˳��ٽ��� */
            rtos_task_critical_exit(status); 

            return RTOS_OK; 
            
        } 
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 

        return -RTOS_UNUSED;   
    }        
}


/**
 * \brief ֪ͨ�����ź������ã����ѵȴ������е�һ������,
 *        ��������ӿڱ���������ռ��Щ�����ź�������������������Ի����������ź����Ļ���
 */  
int32_t rtos_mutex_sem_notify (rtos_mutex_sem_t *p_mutex_sem)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���p_current_task��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    if (p_mutex_sem->loced_count <= 0) {
        
        /* ��������Ϊ0���ź���δ��������ֱ���˳� */
        rtos_task_critical_exit(status);
        
        return RTOS_OK;
    }

    if (p_mutex_sem->p_task_ower != p_current_task) {
        
        /* ����ӵ�����ͷţ���Ϊ�ǷǷ�  */
        rtos_task_critical_exit(status);
        
        return RTOS_ERROR_OWER;
    }

    if (--p_mutex_sem->loced_count > 0) {
        
        /* ��1������Բ�Ϊ0, ֱ���˳�������Ҫ���ѵȴ������� */
        rtos_task_critical_exit(status);
        
        return RTOS_OK;
    }

    /* �Ƿ��з������ȼ��̳� */
    if (p_mutex_sem->task_ower_prior != p_mutex_sem->p_task_ower->prio) {
        
        /* �з������ȼ��̳У��ָ�ӵ���ߵ����ȼ� */
        if (p_mutex_sem->p_task_ower->task_state & RTOS_TASK_STATE_REDDY) {
            
            /* �����ھ���״̬ʱ�����������ھ������е�λ�� */
            rtos_task_sched_unready(p_mutex_sem->p_task_ower);  
            
            /* �ָ����ȼ� */
            p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;           
            rtos_task_sched_ready(p_mutex_sem->p_task_ower);
            
        } else{
            /* ����״̬��ֻ��Ҫ�޸����ȼ� */
            p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;
        }
    }

    /* ����Ƿ�������ȴ� */
    if (rtos_event_wait_count(&p_mutex_sem->mutex_sem_event) > 0) {
        
        /* ����еĻ�����ֱ�ӻ���λ�ڶ����ײ������ȵȴ��������� */
        rtos_task_t * task = rtos_task_first_event_wake_up(&p_mutex_sem->mutex_sem_event, (void *)0, RTOS_OK);
        
        /* ��¼�ǻ��ѵ��������ռ�л����ź��� */
        p_mutex_sem->p_task_ower = task;
        p_mutex_sem->task_ower_prior = task->prio;
        p_mutex_sem->loced_count++;

        /* ��������������ȼ����ߣ���ִ�е��ȣ��л���ȥ */
        if (task->prio < p_current_task->prio) {
            
             rtos_task_sched();
        }
    }
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
    
    return RTOS_OK;   
}


/**
 * \brief��ѯ�����ź�����״̬��Ϣ
 */  
void rtos_mutex_sem_info_get (rtos_mutex_sem_t *p_mutex_sem, rtos_mutex_sem_info_t *p_info)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���p_current_task��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    p_info->task_count = rtos_event_wait_count(&p_mutex_sem->mutex_sem_event);
    p_info->owner_task_prio = p_mutex_sem->task_ower_prior;
    
    if (p_mutex_sem->p_task_ower != NULL) {
        
        p_info->inherited_prio = p_mutex_sem->p_task_ower->prio;
        
    } else {
        
        p_info->inherited_prio = RTOS_PRIO_COUNT; 
    }
    
    p_info->p_task_ower = p_mutex_sem->p_task_ower;
    
    p_info->locked_count = p_mutex_sem->loced_count;

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 

    
}

/**       
 * \brief ���ٻ����ź���
 */  
uint32_t rtos_mutex_sem_destroy (rtos_mutex_sem_t *p_mutex_sem)
{
    uint32_t count = 0;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���p_current_task��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();

    /* �ź����Ƿ��Ѿ���������δ����ʱû������ȴ������ش��� */
    if (p_mutex_sem->loced_count > 0) {
       
        /* �Ƿ��з������ȼ��̳� */
        if (p_mutex_sem->task_ower_prior != p_mutex_sem->p_task_ower->prio) {
            
            /* �з������ȼ��̳У��ָ�ӵ���ߵ����ȼ� */
            if (p_mutex_sem->p_task_ower->task_state & RTOS_TASK_STATE_REDDY) {
                
                /* �����ھ���״̬ʱ�����������ھ������е�λ�� */
                rtos_task_sched_unready(p_mutex_sem->p_task_ower);  
                
                /* �ָ����ȼ� */
                p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;           
                rtos_task_sched_ready(p_mutex_sem->p_task_ower);
                
            } else{
                /* ����״̬��ֻ��Ҫ�޸����ȼ� */
                p_mutex_sem->p_task_ower->prio = p_mutex_sem->task_ower_prior;
            }
        }
        
        /* Ȼ������¼����ƿ��е����� */
        count = rtos_task_event_all_remove(&p_mutex_sem->mutex_sem_event, NULL, -RTOS_DEL);

        /* ��չ����п����и������ȼ����������ִ��һ�ε��� */    
        if (count > 0) {
            
           rtos_task_sched();         
            
        }        
        
    }

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 

    return count;    
    
}
 
 
 
 
 
 /* end of file */



