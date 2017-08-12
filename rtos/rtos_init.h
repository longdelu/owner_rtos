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
 * \brief RTOS�����ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_INIT_H
#define __RTOS_INIT_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"

    
/**
 * \name RTOS ���ͷ�ļ�
 * @{
 */
#include "arm.h"
#include "c_lib.h"
#include "rtos_task.h"
#include "rtos_task_critical.h"
#include "rtos_task_delay.h"
#include "rtos_task_switch.h"
#include "rtos_task_bitmap.h"
#include "rtos_task_list.h"
#include "rtos_task_event.h"
#include "rtos_sem.h"
#include "rtos_mbox.h"
#include "rtos_memblock.h"
#include "rtos_flaggroup.h"
#include "rtos_mutex_sem.h"
#include "rtos_soft_timer.h" 


/** @} */

/**
 * \name RTOS ���ȫ�ֱ�������
 * @{
 */

/** \brief �������ȼ��ľ���������� */
extern rtos_task_bitmap_t task_priobitmap;

/** \brief  ������ʱ���� */
extern rtos_task_list_t rtos_task_delayedlist;

/** \brief ��ǰ���񣺼�¼��ǰ���ĸ������������� */
extern rtos_task_t * p_current_task;

/** \brief ��һ���������е������ڽ��������л�ǰ�������úø�ֵ��Ȼ�������л������л���ж�ȡ��һ������Ϣ */
extern rtos_task_t * p_next_task;

/** \brief ��������ṹ�� */
extern rtos_task_t idle_task;

/** \brief ���������ջ */
extern taskstack_t idle_task_stack_buf[RTOS_IDLE_TASK_STACK_SIZE];

/** \brief ͬһ�����ȼ����������ͷ��� */
extern rtos_task_list_t task_table[TASK_COUNT];

/** @} */


/**
 * \brief ����������ں���
 * \param[in]:��
 * \return ��
 */
int rtos_init (void);


/**
 * \brief rtos����ϵͳ��������
 * \param[in]:��
 * \return ��
 */
void rtos_start (void);

    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_INIT_H */





