#ifndef __RTOS_TASK_CRITICAL_H
#define __RTOS_TASK_CRITICAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "arm.h"
    
/*   
 * \par Ƕ��ʹ��ʾ����
 * \code 
 * uint32_t key0, key1;
 *
 * key0 = rtos_task_critical_entry();  // �ж����Ϲر�
 * key1 = rtos_task_critical_entry();  // �жϱ��ֹر�
 * 
 * rtos_task_critical_exit(key1);   // �жϱ��ֹر�
 * rtos_task_critical_exit(key0);   // �ж����ϴ�
 * \endcode
 */    

/** \brief ������״̬ */
#define TASKSCHED_UNLOCK           0    
#define TASKSCHED_LOCK             1  
        
    
/**
 * \brief  �����ٽ���
 * \param[in] ��
 * \return    ����֮ǰ���ٽ���CPU�ж�״ֵ̬
 */
uint32_t rtos_task_critical_entry (void);
    
    
/**
 * \brief  �˳��ٽ���
 * \param[in] status : �����ٽ���֮ǰ��CPU�ж�ʹ�ܻ����״̬
 * \return    ��
 */
void rtos_task_critical_exit (uint32_t status);


/**
 * \brief  ��ʼ��������
 * \param[in] ��
 * \return    ��
 */
void rtos_task_schedlock_init (void);


/**
 * \brief  �������򿪣���ֹ�������
 * \param[in] ��
 * \return    ��
 */
void rtos_task_sched_disable (void);


/**
 * \brief  �������رգ������������
 * \param[in] ��
 * \return    ��
 */
void rtos_task_sched_enable (void);


/**
 * \brief  ��ѯ������״̬
 * \param[in] ��
 * \return    TASKSCHED_UNLOCK: �����������; TASKSCHED_LOCK: ��ֹ�������
 */
uint8_t rtos_task_schedlock_status (void);

/**
 * \brief  �����ж�isr,����Ƕ�״���
 * \param[in] ��
 * \return    ��
 * \note �� rtos_interupt_exit ���ʹ��
 */
void  rtos_interupt_enter (void);



/**
 * \brief  �˳��ж�isr,�ݼ�Ƕ�״���
 * \param[in] ��
 * \return    ��
 * \note ��rtos_interupt_enter���ʹ��
 */
void  rtos_interupt_exit (void);





#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CRITICAL_H */
