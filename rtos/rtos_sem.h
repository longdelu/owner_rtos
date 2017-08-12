/**
 * \file
 * \brief RTOS �����ź���ͷ�ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_SEM_H
#define __RTOS_SEM_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"

/** \brief �ź����ṹ�� */    
typedef struct rtos_sem {
    
    /** \brief �¼����ƿ� */
    rtos_task_event_t sem_event;

    /** \brief �ź������� */
    uint32_t sem_count;

    /** \brief �ź��������� */
    uint32_t max_count;    
       
}rtos_sem_t;


/** \brief �ź�������Ϣ���� */
typedef struct rtos_sem_info {
    /** \brief  ��ǰ�ź����ļ��� */
    uint32_t count;

    /** \brief  �ź�������������� */
    uint32_t max_count;

    /** \brief  ��ǰ�ȴ���������� */
    uint32_t task_count;
}rtos_sem_info_t;




/**
 * \brief ��ʼ�������ź���
 * \param[in] p_sem: ָ������ź����ṹ���ָ��
 * \param[in] start_count:  �ź����ĳ�ʼ����
 * \param[in] max_count: �ź����������������Ϊ0�����ź������������ϲ�������
 * \return  ��
 */  
void rtos_sem_init(rtos_sem_t *p_sem, uint32_t start_count, uint32_t max_count);


/**
 * \brief �ȴ������ź���
 * \param[in] p_sem: ָ������ź����ṹ���ָ��
 * \param[in] wait_ticks:  ���ź�������Ϊ0ʱ���ȴ���ticks����Ϊ0ʱ��ʾ��Զ�ȴ�
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */  
int32_t rtos_sem_wait (rtos_sem_t *p_sem, uint32_t wait_ticks);

/**
 * \brief ��ȡ�ź���������ź������������ã��������˻�
 * \param[in] p_sem: ָ������ź����ṹ���ָ��
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK
 */  
int32_t rtos_sem_get (rtos_sem_t *p_sem);


/**
 * \brief ֪ͨ�ź������ã����ѵȴ������е�һ�����񣬻��߽�����+1
 * \param[in] p_sem: ָ������ź����ṹ���ָ��
 * \return  ��
 */  
void rtos_sem_notify (rtos_sem_t *p_sem);


/**
 * \brief��ѯ�ź�����״̬��Ϣ
 * \param[in] p_sem:  ָ������ź����ṹ���ָ��
 * \param[in] p_info: ָ������ź����ṹ���ָ��
 * \return  ��
 */  
void rtos_sem_info_get (rtos_sem_t * p_sem, rtos_sem_info_t *p_info);

/**       
 * \brief �����ź���
 * \param[in] p_sem:  ָ������ź����ṹ���ָ��
 * \return  �����ٸ��ź��������ѵ���������
 */  
uint32_t rtos_sem_destroy (rtos_sem_t *p_sem);

    
   
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_SEM_H */

