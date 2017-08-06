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
 
 #include "rtos_flaggroup.h"
 
 
/**
 * \brief  ������������鲢���ĵ��¼���־
 * \param[in] p_flag_grp: ָ������¼������ṹ���ָ��
 * \param[in] wait_type :  �ȴ����¼����ͣ�ֵΪ RTOS_FLAG_GRP_ANY_* ��һ��� (# RTOS_FLAG_GRP_ANY_CLR)
 * \param     p_req_flag:    ������¼���־�洢��ַ�ͼ�����洢λ��,��������Ҳ�����
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK
 */   
static int32_t __rtos_rtos_flag_grp_check_and_consum( rtos_flag_grp_t *p_flag_grp, uint32_t wait_type, uint32_t *p_req_flag)
{
    uint32_t src_flags = *p_req_flag;
    uint32_t is_set = wait_type & RTOS_FALG_GRP_SET;
    uint32_t is_all = wait_type & RTOS_FALG_GRP_ALL;
    uint32_t is_consume = wait_type & RTOS_FLAG_GRP_CONSUME;

    /*
     *  ����Щ���͵ı�־λ����
     *  flagGroup->flags & flags��������¼����������Щλ�Ѿ�Ϊ1
     *  flagGroup->flags & flags: ������¼����������Щλ�Ѿ�Ϊ0
     */
    uint32_t calc_flag = is_set ? (p_flag_grp->flag & src_flags) : (~p_flag_grp->flag & src_flags);

    /* ���б�־λ����, �����������־λ���֣��������� */
    if (((is_all != 0) && (calc_flag == src_flags)) || ((is_all == 0) && (calc_flag != 0))) {
        
        /* �Ƿ����ĵ���־λ */
        if (is_consume) {
            
            if (is_set) {
                /* ���Ϊ1�ı�־λ�����0 */
                p_flag_grp->flag &= ~src_flags;
            } else {
                /* ���Ϊ0�ı�־λ�����1 */
                p_flag_grp->flag |= src_flags;
            }
        }
        
        *p_req_flag = calc_flag;
        
        return RTOS_OK;
    }

    *p_req_flag = calc_flag;
    
    return -RTOS_UNUSED;    
    
 
}
 
 
/**
 * \brief ��ʼ���¼������
 */  
void rtos_flag_grp_init(rtos_flag_grp_t *p_flag_grp, uint32_t flag)
{
    rtos_task_event_init(&p_flag_grp->flag_grp_event, RTOS_EVENT_TYPE_FLAG_GRP);
    
    p_flag_grp->flag = flag;
                 
}


/**
 * \brief �ȴ������¼������
 */  
int32_t rtos_flag_grp_wait (rtos_flag_grp_t *p_flag_grp, 
                            uint32_t wait_type, 
                            uint32_t req_flag, 
                            uint32_t *p_req_result, 
                            uint32_t wait_ticks)
{
    uint32_t result = 0;
    uint32_t flags = req_flag;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    result = __rtos_rtos_flag_grp_check_and_consum(p_flag_grp, wait_type, &flags); 
    
    if (result != RTOS_OK) {
        
        /* ����������Ҫ�ȴ����¼���������� */
        p_current_task->wait_flags_grp_type = wait_type;
        p_current_task->event_flags_grp    = req_flag;
        
        rtos_task_event_wait(p_current_task, &p_flag_grp->flag_grp_event, NULL, RTOS_EVENT_TYPE_FLAG_GRP, wait_ticks); 
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 
        
        /* �����ִ��һ���¼����ȣ��Ա����л����������� */
        rtos_task_sched(); 
        
        /* �����ڵȴ���ʱ�����¼����������ؿ���ʱ��ִ�л᷵�ص����Ȼ��ȡ���ȴ����������ȴ�����Щ�¼����λ */
        *p_req_result = p_current_task->event_flags_grp;
        
        return  p_current_task->event_wait_result;
        
    } else {
        /* �����ڵȴ���ʱ�����¼����������ؿ���ʱ��ִ�л᷵�ص����Ȼ��ȡ���ȴ����������ȴ�����Щ�¼����λ */
        *p_req_result = flags;
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 
    }
    
    
    return result;
    
}




/**
 * \brief ��ȡ�¼�����飬����¼��������������ã��������˻�
 */  
int32_t rtos_flag_grp_get (rtos_flag_grp_t *p_flag_grp, uint32_t wait_type, uint32_t req_flag, uint32_t *p_req_result)
{
    uint32_t result = 0;
    uint32_t flags = req_flag;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    result = __rtos_rtos_flag_grp_check_and_consum(p_flag_grp, wait_type, &flags); 
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
    
   *p_req_result =  flags;

    return result;    
}


/**
 * \brief �����¼�������е�ĳһλ�����ѵȴ������е�һ������
 */  
void rtos_flag_grp_notify (rtos_flag_grp_t *p_flag_grp, uint32_t set_bit, uint32_t flag)
{
    dlist_node_t *p_nxet_node = NULL;
    
    dlist_node_t *p_current_node = NULL;
     
    rtos_task_list_t *p_flag_grp_wait_list = NULL;  
    
    rtos_task_t *p_task = NULL;
    
    uint32_t flag_grp = 0;
    
    uint32_t sched = 0;
    
    uint32_t result = 0;
    
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();    
    
    if (set_bit) {
        p_flag_grp->flag |= flag;
    } else {
        p_flag_grp->flag &= ~flag;        
    }      
    
    p_flag_grp_wait_list = &p_flag_grp->flag_grp_event.event_wait_list;
   

    /* �������еĵȴ�����, ��ȡ�������������񣬼��뵽���Ƴ��б��� */    
    for (p_current_node = p_flag_grp_wait_list->head_node.p_next; 
         p_current_node != &(p_flag_grp_wait_list->head_node);
         p_current_node = p_nxet_node) {
             
         p_task =  RTOS_CONTAINER_OF(p_current_node, rtos_task_t, event_node);
             
         flag_grp = p_task->event_flags_grp;
             
             
         p_nxet_node =  p_current_node->p_next;
             
             
         result = __rtos_rtos_flag_grp_check_and_consum(p_flag_grp, p_task->wait_flags_grp_type, &flag_grp);

         if (result == RTOS_OK) {
             
             /* �������� */
             p_task->event_flags_grp = flag_grp;
             rtos_task_event_wake_up(&p_flag_grp->flag_grp_event, p_task, (void *)0,  RTOS_OK);
             sched = 1;
             

         }             

     }
     
     /* ����������������ִ��һ�ε��� */
     if (sched) {
         rtos_task_sched();          
     }
             
                            
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);     
    
}

/**
 * \brief  ��ѯ�¼���־���״̬��Ϣ
 */  
void rtos_flag_grp_info_get (rtos_flag_grp_t *p_flag_grp, rtos_flag_grp_info_get_t *p_info)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();  
    
    p_info->flag = p_flag_grp->flag;
    
    /* �������ź������������� */    
    p_info->task_count =  rtos_event_wait_count(&p_flag_grp->flag_grp_event);
 
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);         
}
 
 
/**
 * \brief  �����¼���־��
 */  
uint32_t rtos_flag_grp_destroy (rtos_flag_grp_t *p_flag_grp)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();  
       
    /* ����¼����ƿ��е����� */
    uint32_t count  = rtos_task_event_all_remove(&p_flag_grp->flag_grp_event, NULL, -RTOS_DEL);  
    p_flag_grp->flag = 0; 
    
    /* ��չ����п����и������ȼ����������ִ��һ�ε��� */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
 
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);  
    
    return count;           
} 
 
 
 
 
 
 
 
 
 
 
 
/* end of file */

