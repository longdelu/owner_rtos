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
 * \brief RTOS �洢��ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_memblock.h"
 
#if RTOS_ENABLE_MEMBLOCK == 1 
/**
 * \brief ��ʼ���洢���ƿ�
 */ 
void rtos_memblock_init (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr_start, uint32_t block_size, uint32_t block_cnt)
{
    uint8_t *p_memblock_addr_end = p_mem_addr_start + block_size * block_cnt;
    
    if (p_memblock == NULL || p_mem_addr_start == NULL) {
        return;
    } 
    
    /*
     * ÿ���洢����Ҫ����������ָ�룬���Կռ�����Ҫ��tNode��
     * ������ˣ�ʵ���û����õĿռ䲢û����
     */
    if (block_size < sizeof(dlist_node_t)){
        return;
    }

    /* ��ʼ���洢���¼����ƿ� */
    rtos_task_event_init(&p_memblock->memblock_event, RTOS_EVENT_TYPE_MEMBLOCK);

    p_memblock->mem_addr_start = p_mem_addr_start;
    p_memblock->block_size = block_size;
    p_memblock->max_count = block_cnt;

    /* �洢�������ʼ�� */
    rtos_task_list_init(&p_memblock->block_list);
    
    while (p_mem_addr_start < p_memblock_addr_end) {
        rtos_task_list_add_tail(&p_memblock->block_list, (dlist_node_t *)p_mem_addr_start);

        p_mem_addr_start += block_size;
    }

} 


/**
 * \brief �ȴ��洢��
 */
int32_t rtos_memblock_wait (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr, uint32_t wait_ticks)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /*�����ȼ���Ƿ��п��еĴ洢�� */
    if (rtos_task_list_count(&p_memblock->block_list) > 0) {
        
        /* ����еĻ���ȡ��һ�����еĴ洢�飬�޸����ϲ�Ӧ�ô�������һάָ���ָ�򣬼��ӿ�ָ��ת��ָ�������õ�ָ�� */
        *pp_mem_addr = (uint8_t *)rtos_task_list_remove_first(&p_memblock->block_list);
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 
        
        return RTOS_OK;

    } else {
           /*������������¼����� */
        rtos_task_event_wait(p_current_task, &p_memblock->memblock_event, NULL, RTOS_EVENT_TYPE_MEMBLOCK, wait_ticks);
        
        /* �˳��ٽ��� */
        rtos_task_critical_exit(status); 
        
        /* �����ִ��һ���¼����ȣ��Ա����л����������� */
        rtos_task_sched();   

       /* ���л�����ʱ����tTask��ȡ����õ���Ϣ,��ʱ������ƿ��п�����int (*)[4]�ĵ�ַ */
       *pp_mem_addr = (uint8_t *)p_current_task->p_event_msg;
       
       /* ȡ���ȴ���� */        
       return  p_current_task->event_wait_result;

    }        
}

/**
 * \brief  ��ȡ�洢�飬���û�д洢�飬�������˻�
 */
int32_t rtos_memblock_get (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    /*�����ȼ���Ƿ��п��еĴ洢�� */
    if (rtos_task_list_count(&p_memblock->block_list) > 0) {
        
        /* ����еĻ���ȡ��һ�����еĴ洢�� */
        *pp_mem_addr = (uint8_t *)rtos_task_list_remove_first(&p_memblock->block_list);
        
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
 * \brief  ֪ͨ�洢����ã����ѵȴ������е�һ�����񣬻��߽��洢�����洢�����������
 */
void rtos_memblock_notify (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry();
    
    /* ����Ƿ�������ȴ� */
    if (rtos_event_wait_count(&p_memblock->memblock_event) > 0){
        /* ����еĻ�����ֱ�ӻ���λ�ڶ����ײ������ȵȴ���������*/
        rtos_task_t * task = rtos_task_first_event_wake_up(&p_memblock->memblock_event, (void *)p_mem_addr, RTOS_OK );

        /* ��������������ȼ����ߣ���ִ�е��ȣ��л���ȥ */
        if (task->prio < p_current_task->prio) {
             rtos_task_sched(); 
        }
    } else {
        
        /* ���û������ȴ��Ļ������洢����뵽������ */
        rtos_task_list_add_tail(&p_memblock->block_list, (dlist_node_t *)p_mem_addr);        
    }  

    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);     
}
    
/**
 * \brief��ѯ�洢���ƿ��״̬��Ϣ
 */  
void rtos_memblock_info_get (rtos_memblock_t * p_memblock, rtos_memblock_info_t *p_info)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 
    
    p_info->cur_count  =  rtos_task_list_count(&p_memblock->block_list);
    p_info->max_count  =  p_memblock->max_count;  
    p_info->block_size =  p_memblock->block_size;
    /* �����洢����������� */    
    p_info->task_count =  rtos_event_wait_count(&p_memblock->memblock_event);
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status); 
    
}

/**       
 * \brief ���ٴ洢���ƿ�
 */  
uint32_t rtos_memblock_destroy (rtos_memblock_t *p_memblock)
{
    /* �����ٽ������Ա�������������������л��ڼ䣬������Ϊ�����жϵ���currentTask��nextTask���ܸ��� */    
    uint32_t status = rtos_task_critical_entry(); 

    /* ����¼����ƿ��е����� */
    uint32_t count  = rtos_task_event_all_remove(&p_memblock->memblock_event, NULL, -RTOS_DEL);  
   
    
    /* �˳��ٽ��� */
    rtos_task_critical_exit(status);  
    
    /* ��չ����п����и������ȼ����������ִ��һ�ε��� */    
    if (count > 0) {
        
       rtos_task_sched();         
        
    }
    
    return count;   
} 

#endif
 
 
/* end of file */




