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
 * \brief RTOS ��Ϣ����ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_mbox.h"
 
 /**
 * \brief ��ʼ����Ϣ����
 */  
void rtos_mbox_init(rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t msg_max_count)
{
    rtos_task_event_init(&p_mbox->mbox_event, RTOS_EVENT_TYPE_MBOX);
    p_mbox->p_msg_buf   = p_msg_buf;
    p_mbox->write_index = 0;
    p_mbox->read_index  = 0;
    p_mbox->msg_cur_count  = 0;
    p_mbox->msg_max_count =  msg_max_count;       
                 
}


/**
 * \brief �ȴ�����, ��ȡһ����Ϣ
 */ 
int32_t rtos_mbox_wait (rtos_mbox_t *p_mbox, void **pp_msg_buf, uint32_t wait_ticks)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* ���ȼ����Ϣ�����Ƿ����0 */ 
    if (p_mbox->msg_cur_count > 0) {
        
        /* ����0�Ļ���ȡ��һ����Ϣ */
        --p_mbox->msg_cur_count;
        *pp_msg_buf = p_mbox->p_msg_buf[p_mbox->read_index++];
        
        /* ͬʱ��ȡ����ǰ�ƣ���������߽������ */
        if (p_mbox->read_index >= p_mbox->msg_max_count) {
            p_mbox->read_index = 0;
        }
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status);   
        
        return RTOS_OK;
        
    } else {
        
        /*������������¼����� */
        rtos_task_event_wait(p_current_task, &p_mbox->mbox_event, NULL, RTOS_EVENT_TYPE_MBOX, wait_ticks);
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 
        
        /* �����ִ��һ���¼����ȣ��Ա����л����������� */
        rtos_task_sched();   

       /* ���л�����ʱ����tTask��ȡ����õ���Ϣ */
       *pp_msg_buf = p_current_task->p_event_msg;
       
       /* ȡ���ȴ���� */        
       return  p_current_task->event_wait_result;
               
    }
    
}

/**
 * \brief  ��ȡһ����Ϣ�����û����Ϣ���������˻�
 */ 
int32_t rtos_mbox_nowait_get (rtos_mbox_t *p_mbox, void **pp_msg_buf)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /* ���ȼ����Ϣ�����Ƿ����0 */ 
    if (p_mbox->msg_cur_count > 0) {
        
        /* ����0�Ļ���ȡ��һ����Ϣ */
        --p_mbox->msg_cur_count;
        *pp_msg_buf = p_mbox->p_msg_buf[p_mbox->read_index++];
        
        /* ͬʱ��ȡ����ǰ�ƣ���������߽������ */
        if (p_mbox->read_index >= p_mbox->msg_max_count) {
            p_mbox->read_index = 0;
        }
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status);   
        
        return RTOS_OK;
        
    } else {
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status);  
       
       /* ��Դ������ */        
       return  -RTOS_UNUSED;
               
    }
}


/**
 * \brief  ֪ͨ��Ϣ���ã����ѵȴ������е�һ�����񣬻��߽���Ϣ���뵽������
 */ 
int32_t  rtos_mbox_notify (rtos_mbox_t *p_mbox, void *p_msg, uint32_t notify_option)
{
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
    
    /* ����Ƿ�������ȴ� */
    if (rtos_event_wait_count(&p_mbox->mbox_event) > 0){
        /* ����еĻ�����ֱ�ӻ���λ�ڶ����ײ������ȵȴ���������*/
        rtos_task_t * task = rtos_task_event_wake_up(&p_mbox->mbox_event, (void *)p_msg, RTOS_OK );

        /* ��������������ȼ����ߣ���ִ�е��ȣ��л���ȥ */
        if (task->prio < p_current_task->prio) {
             rtos_task_sched(); 
        }
    } else {
        
        /* ���û������ȴ��Ļ�������Ϣ���뵽�������� */
        if (p_mbox->msg_cur_count >= p_mbox->msg_max_count) {
            
            /* �˳��ٽ��� */
            rtos_task_critical_exit(status); 
            
            /* ��Ϣ�������Ѿ�����*/
            return -RTOS_FULL;
            
        }
        
        /* ����ѡ����Ϣ���뵽ͷ���������������ȡ����Ϣ��ʱ�����Ȼ�ȡ����Ϣ */ 
        if (notify_option == RTOS_MBOX_SENDFRONT) {
            
            if (p_mbox->read_index <=0) {
                p_mbox->read_index = p_mbox->msg_max_count - 1;
            } else {
                 --p_mbox->read_index;
            }
            
            p_mbox->p_msg_buf[p_mbox->read_index] = p_msg;    
        } else {
            p_mbox->p_msg_buf[p_mbox->write_index++] = p_msg;
            
            if (p_mbox->write_index >= p_mbox->msg_max_count) {
                p_mbox->write_index = 0;
            }                
        }
        
        /* ��Ϣ������1 */           
         ++p_mbox->msg_cur_count ;
   
                  
    }
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
    
    return RTOS_OK;   
}


/**
 * \brief   ���������������Ϣ
 */ 
void rtos_mbox_flush (rtos_mbox_t *p_mbox)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
    
    /* ����¼�������������ȴ���˵�������Ѿ��ǿյ��ˣ�����Ҫ����� */
    if (rtos_event_wait_count(&p_mbox->mbox_event) == 0){
        p_mbox->read_index  = 0;
        p_mbox->write_index = 0;
        p_mbox->msg_cur_count = 0;      
    }
        
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);     
}

/**
 * \brief   ��������
 */ 
uint32_t rtos_mbox_destroy (rtos_mbox_t *p_mbox)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    /* ����¼����ƿ��е����� */
    uint32_t count  = rtos_task_event_all_remove(&p_mbox->mbox_event, NULL, -RTOS_DEL);  
    
    p_mbox->read_index  = 0;
    p_mbox->write_index = 0;
    p_mbox->msg_cur_count = 0;    
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);  
    
    /* ��չ����п����и������ȼ����������ִ��һ�ε��� */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
    
    return count;    
}

/**********************************************************************************************************
** Function name        :   tMboxGetInfo
** Descriptions         :   ��ѯ״̬��Ϣ
** parameters           :   mbox ��ѯ������
** parameters           :   info ״̬��ѯ�洢��λ��
** Returned value       :   ��
***********************************************************************************************************/
void rtos_mbox_info_get (rtos_mbox_t *p_mbox, rtos_mbox_info_t *p_info)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    /* ������Ҫ����Ϣ */
    p_info->msg_cur_count = p_mbox->msg_cur_count;
    p_info->max_msg_count = p_mbox->msg_max_count;
    p_info->task_count = rtos_event_wait_count(&p_mbox->mbox_event);

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);  
}

 
 
 
  /* end of file */


