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
    /** \brief  ������ */
    dlist_node_t timer_node;

    /** \brief  ���������Ӻ��ticks�� */
    uint32_t start_delay_ticks;

    /** \brief  ���ڶ�ʱʱ������tick�� */
    uint32_t duration_ticks;

    /** \brief ��ǰ��ʱ�ݼ�����ֵ,����ָʾ start_delay_ticks ��  duration_ticks ������� */
    uint32_t delay_ticks;

    /** \brief ��ʱ�ص����� */
    void (*pfn_timer_func) (void *p_arg);

    /** \brief ���ݸ��ص������Ĳ��� */
    void *p_arg;

    /** \brief ��ʱ�����ò��� */
    uint32_t config;

    /** \brief ��ʱ��״̬ */
    rtos_timer_sate_t state;
}rtos_timer_t;


/** \brief ��ʱ��״̬��Ϣ�ṹ������ */
typedef struct rtos_timer_info_get {
    
    /** \brief ���������Ӻ��ticks��   */
    uint32_t start_delay_ticks;
    
    /** \brief ���ڶ�ʱʱ������tick��   */
    uint32_t duration_ticks;
    
    /** \brief ��ʱ�ص����� */
    void (*pfn_timer_func) (void *p_arg);

    /** \brief ���ݸ��ص������Ĳ��� */
    void *p_arg;
    
    /** \brief ��ʱ�����ò��� */
    uint32_t config;

    /** \brief ��ʱ��״̬ */
    rtos_timer_sate_t state;
    
}rtos_timer_info_get_t;


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
                      
/**
 * \brief ������ʱ��
 * \param[in] p_timer: ָ��ʱ���ṹ���ָ��
 * \return ��
 */
void rtos_timer_start (rtos_timer_t * p_timer);  


/**
 * \brief ֹͣ��ʱ��
 * \param[in] p_timer: ָ��ʱ���ṹ���ָ��
 * \return ��
 */
void rtos_timer_stop (rtos_timer_t * p_timer);  



/**
 * \brief ��ѯ��ʱ��״̬��Ϣ
 * \param[in] p_timer: ָ��ʱ���ṹ���ָ��
 * \param[in] p_info : ��ʱ��״̬��Ϣ�ṹ������
 * \return ��
 */
void rtos_timer_info_get (rtos_timer_t * p_timer, rtos_timer_info_get_t *p_info);


/**
 * \brief ����u��ʱ��
 * \param[in] p_timer: ָ��ʱ���ṹ���ָ��
 * \return ��
 */
void rtso_timer_destroy (rtos_timer_t * p_timer) ;



/**
 * \brief ֪ͨϵͳ�δ�ʱ���Ѿ������ˣ���Ϊ������Ӳ���жϴ������е��õģ�Ҳ�������������Ӳ����ʱ���Ĵ���
 * \param[in] ��
 * \return ��
 */
void rtos_timer_moudule_tick_notify (void); 


/**
 * \brief ��ʱ��ģ���ʼ������
 * \param[in] ��
 * \return ��
 */
void  rtos_timer_moudule_init (void);    


/**
 * \brief ��ʼ����ʱ������
 * \param[in] ��
 * \return ��
 */
void rtos_timer_task_init (void);
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_SOFT_TIMER_H */



