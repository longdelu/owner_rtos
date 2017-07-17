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
 * \brief ����ϵͳ��������ʵ�� 
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#ifndef __RTOS_TASK_LIST_H
#define __RTOS_TASK_LIST_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_list.h"
    
/** \brief ������������ */   



/** \brief ˫���������ʵ�� */    
typedef struct __rtos_task_list {
 
    /** \brief ˫����������ͷ���  */ 
    dlist_node_t head_node ;
    
    /** \brief �������� */
    uint32_t count;
    
    
}rtos_task_list_t;   


/**
 * \brief RTOS���������ʼ�� 
 */  
void rtos_task_list_init (rtos_task_list_t *p_rtos_task_list);

/**
 * \brief ͳ�������н�������
 */
uint32_t rtos_task_list_count (rtos_task_list_t *p_rtos_task_list);

/**
 * \brief �Ƴ��������������еĽ��
 */
void rtos_task_list_remove_all (rtos_task_list_t *p_rtos_task_list);


/**
 * \brief ����������ͷ��������
 */
void rtos_task_list_add_head (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node);

/**
 * \brief ����������β��������
 */
void rtos_task_list_add_tail (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node);



/**
 * \brief ����������ָ��λ�ú��������
 */
void rtos_task_list_add (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_pos, dlist_node_t *p_node);

/**
 * \brief ������������ɾ����1���û����
 */
dlist_node_t * rtos_task_list_remove_first (rtos_task_list_t * p_rtos_task_list);

/**
 * \brief ������������ɾ�����1���û����
 */
dlist_node_t * rtos_task_list_remove_last (rtos_task_list_t * p_rtos_task_list);

/**
 * \brief ����������ɾ��ָ�����
 */
void rtos_task_list_remove (rtos_task_list_t * p_rtos_task_list, dlist_node_t *p_node);
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_LIST_H */ 



