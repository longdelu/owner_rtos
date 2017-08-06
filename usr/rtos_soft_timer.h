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
 * \brief RTOS �����ʱ��ʵ�ֽӿ��ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 
#ifndef __RTOS_SOFT_TIMER_H
#define __RTOS_SOFT_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"


/** \brief ��ʱ����״̬ */    
typedef  enum rtos_timer_sate {
    RTOS_TIMER_CREATED,     /** < \brief  ��ʱ���Ѿ����� */
    RTOS_TIMER_STARTED,     /** < \brief  ��ʱ���Ѿ����� */
    RTOS_TIMER_RUNNING,     /** < \brief  ��ʱ������ִ�лص����� */
    RTOS_TIMER_STOPPED,     /** < \brief  ��ʱ���Ѿ�ֹͣ */
    RTOS_TIMER_DESTROYED    /** < \brief  ��ʱ���Ѿ����� */   
}rtos_timer_sate_t;

/** \brief  ��ʱ���ṹ������ */
typedef struct rtos_timer
{
    // ������
    dlist_node_t timer_node;

    // ���������Ӻ��ticks��
    uint32_t start_delay_ticks;

    // ���ڶ�ʱʱ������tick��
    uint32_t duration_ticks;

    // ��ǰ��ʱ�ݼ�����ֵ,����ָʾ start_delay_ticks ��  duration_ticks �������
    uint32_t delay_ticks;

    // ��ʱ�ص�����
    void (*pfn_timer_func) (void *p_arg);

    // ���ݸ��ص������Ĳ���
    void *p_arg;

    // ��ʱ�����ò���
    uint32_t config;

    // ��ʱ��״̬
    rtos_timer_sate_t state;
}rtos_timer_t;


/** \brief ��Ӳ��ʱ������ */
#define TIMER_CONFIG_TYPE_HARD          (1 << 0)   /** \brief ��Ӳ���ж��д��� */
#define TIMER_CONFIG_TYPE_SOFT          (0 << 0)   /** \brief �������ж��д��� */ 


/**
 * \brief ��ʼ����ʱ��
 * \param[in] p_timer: ָ��ʱ���ṹ���ָ��
 * \param[in] delay_ticks:  ��ʱ����ʼ��������ʱticks��
 * \param[in] duration_ticks:  �������Զ�ʱ���õ�����tick����һ���Զ�ʱ����Ч
 * \param[in] pfn_timer_func: ��ʱ���ص�����
 * \param[in] p_arg:  ���ݸ���ʱ���ص������Ĳ���
 * \param[in] config: ��ʱ���ĳ�ʼ���ã� ֵΪ  TIMER_CONFIG_TYPE_HARD �� TIMER_CONFIG_TYPE_SOFT
 * \return ��
 */
void rtos_timer_init (rtos_timer_t * p_timer, 
                      uint32_t start_delay_ticks, 
                      uint32_t duration_ticks,
                      void (*pfn_timer_func)(void *p_arg), 
                      void *p_arg, 
                      uint32_t config);
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_SOFT_TIMER_H */



