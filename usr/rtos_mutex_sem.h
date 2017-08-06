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
    
    /** \brief ��ǰ�¼���� */
    uint32_t loced_count;
    
    /** \brief �����ź���ӵ���� */ 
    rtos_task_t *p_task_ower;
    
    /** \brief �����ź���ӵ���ߵ�ԭ�����ȼ� */  
    uint32_t task_ower_prior;
    
}rtos_mutex_sem_t;


/**
 * \brief ��ʼ�������ź���
 * \param[in] p_mutex_sem: ָ�򻥳��ź����ṹ���ָ��
 * \return ��
 */  
void rtos_mutex_sem_init (rtos_mutex_sem_t *p_mutex_sem);
    

    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */


