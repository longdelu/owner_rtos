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
 * \brief RTOS �¼����ƿ�ָ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_TASK_EVENT_H
#define __RTOS_TASK_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif
  
#include "c_lib.h"
#include "rtos_task_list.h"
#include "rtos_config.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
    
    
typedef enum rtos_task_event_type {
    
    /** \brief δ֪���͵��¼����ƿ� */
    RTOS_EVENT_TYPE_UNKNOW   = (1UL << 16) ,
    
    /** \brief �����ź����¼����ƿ� */
    RTOS_EVENT_TYPE_SEM      = (1UL << 17),

    /** \brief ��Ϣ�����¼����ƿ� */
    RTOS_EVENT_TYPE_MBOX     = (1UL << 18), 
    
    /** \brief �洢����ƿ� */
    RTOS_EVENT_TYPE_MEMBLOCK = (1UL << 19),       

    /** \brief �¼������ */
    RTOS_EVENT_TYPE_FLAG_GRP = (1UL << 20),         
  
}rtos_task_event_type_t;
    
    
    
typedef struct rtos_task_event {
   
   /** \brief event�¼������� */    
   rtos_task_event_type_t event_type;  
    
   /** \brief ����ȴ��¼������� */    
   rtos_task_list_t event_wait_list;  
    

}rtos_task_event_t;

/**
 * \brief ��ʼ��RTOS�����¼����ƿ�
 * \param[in] p_event: RTOS�����¼����ƿ��ָ��
 * \param[in] event_type: �����Ļص�����
 * \return  ��
 */  
void rtos_task_event_init (rtos_task_event_t *p_event, rtos_task_event_type_t event_type);


/**
 * \brief ��RTOS����ָ�����¼����ƿ��ϵȴ��¼�����
 * \param[in] p_task: ����ṹ��ָ��  
 * \param[in] p_event: RTOS�����¼����ƿ��ָ��
 * \param[in] p_event_msg: �¼���Ϣ�洢�ľ���λ��
 * \param[in] time_out: �ȴ��೤ʱ��,����ʱ�ĵȴ�����ʱֵʱ��delay_ticks��Ϊ0��
 * \return  ��
 */  
void rtos_task_event_wait (rtos_task_t *p_task, 
                           rtos_task_event_t *p_event, 
                           void *p_event_msg, 
                           uint32_t state, 
                           uint32_t time_out);   
    
/**
 * \brief ��ָ���¼����ƿ��ϻ����׸��ȴ���RTOS����(�����ǰ������¼��뵽���������У���������������¼��ĺ�������task->delay_ticks����Ϊ0��
 * \param[in] p_event: RTOS�����¼����ƿ��ָ��
 * \param[in] p_event_msg: �¼���Ϣ�洢�ľ���λ��
 * \param[in] event_result: �¼��ȴ��Ľ��
 * \param[in] ��ָ���¼����ƿ��ϻ����׸��ȴ���RTOS���������ṹ��ָ��       
 */  
rtos_task_t  *rtos_task_event_wake_up (rtos_task_event_t *p_event, void *p_event_msg, int32_t event_result); 

/**
 * \brief ��ָ���¼����ƿ���ɾ���ȴ���RTOS����
 * \param[in] p_task: ����ṹ��ָ��  
 * \param[in] p_event_msg: �¼���Ϣ�洢�ľ���λ��
 * \param[in] event_result: �¼��ȴ��Ľ��
 * \return  ��                         
 */  
void rtos_task_event_del (rtos_task_t  *p_task, void *p_event_msg, int32_t event_result);
                           
                           
                           
/**
 * \brief �¼����ƿ��еȴ�����������
 * \param[in] p_event: RTOS�����¼����ƿ��ָ��
 * \return  �����¼����ƿ��еȴ�����������    
 */
uint32_t rtos_event_wait_count (rtos_task_event_t *p_event);


/**
 * \brief ������еȴ��е����񣬽��¼����͸���������
 * \param[in] p_event: RTOS�����¼����ƿ��ָ��
 * \param[in] p_event_msg: �¼���Ϣ�洢�ľ���λ��
 * \param[in] event_result: �¼��ȴ��Ľ��
 * \return  ���ѵ���������
 */ 
uint32_t  rtos_task_event_all_remove (rtos_task_event_t *p_event, void *p_event_msg, int32_t event_result);

    


 
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_EVENT_H */





