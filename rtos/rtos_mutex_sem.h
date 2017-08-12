/**
 * \file
 * \brief RTOS �洢��ͷ�ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_MUTEX_SEM_H
#define __RTOS_MUTEX_SEM_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
    
    
    
/** \brief �����ź����ṹ������   */    
typedef struct rtos_mutex_sem {
    /** \brief �¼����ƿ�   */
    rtos_task_event_t mutex_sem_event;
    
    /** \brief �����ź���Ƕ�׼��������Խ��Ƕ�׵��� */
    uint32_t loced_count;
    
    /** \brief �����ź���ӵ���ߣ���ֹ��������ִ���ͷ��ź���ʱ��������������ִ���ͷŲ�����Ч */ 
    rtos_task_t *p_task_ower;
    
    /** \brief �����ź���ӵ���ߵ�ԭ�����ȼ� */  
    uint32_t task_ower_prior;
    
}rtos_mutex_sem_t;


/** \brief �����ź�����ѯ�ṹ */ 
typedef struct  rtos_mutex_sem_info
{
    /** \brief �ȴ����������� */
    uint32_t task_count;

    /** \brief ӵ������������ȼ� */
    uint32_t owner_task_prio;

    /** \brief �̳����ȼ� */
    uint32_t inherited_prio;

    /** \brief  ��ǰ�ź�����ӵ���� */
    rtos_task_t *p_task_ower;

    /** \brief �������� */
    uint32_t locked_count;
    
}rtos_mutex_sem_info_t;



/**
 * \brief ��ʼ�������ź���
 * \param[in] p_mutex_sem: ָ�򻥳��ź����ṹ���ָ��
 * \return ��
 */  
void rtos_mutex_sem_init (rtos_mutex_sem_t *p_mutex_sem);


/**
 * \brief �ȴ������ź���
 * \param[in] p_mutex_sem: ָ�򻥳��ź����ṹ���ָ��
 * \param[in] wait_ticks:  ���ź�������Ϊ0ʱ���ȴ���ticks����Ϊ0ʱ��ʾ��Զ�ȴ�
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */  
int32_t rtos_mutex_sem_wait (rtos_mutex_sem_t *p_mutex_sem, uint32_t wait_ticks);

/**
 * \brief ��ȡ�����ź�������������ź��������ã��������˻�
 * \param[in] p_mutex_sem: ָ�򻥳��ź����ṹ���ָ��
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK
 */  
int32_t rtos_mutex_sem_get (rtos_mutex_sem_t *p_mutex_sem);


/**
 * \brief ֪ͨ�����ź������ã����ѵȴ������е�һ�����񣬻��߽�����+1
 * \param[in] p_mutex_sem: ָ�򻥳��ź����ṹ���ָ��
 * \return  ��
 */  
int32_t rtos_mutex_sem_notify (rtos_mutex_sem_t *p_mutex_sem);


/**
 * \brief��ѯ�����ź�����״̬��Ϣ
 * \param[in] p_mutex_sem:  ָ�򻥳��ź����ṹ���ָ��
 * \param[in] p_info: ָ�򻥳��ź����ṹ���ָ��
 * \return  ��
 */  
void rtos_mutex_sem_info_get (rtos_mutex_sem_t * p_mutex_sem, rtos_mutex_sem_info_t *p_info);

/**       
 * \brief ���ٻ����ź���
 * \param[in] p_mutex_sem:  ָ�򻥳��ź����ṹ���ָ��
 * \return  �����ٸû����ź��������ѵ���������
 */  
uint32_t rtos_mutex_sem_destroy (rtos_mutex_sem_t *p_mutex_sem);

 
    

    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */


