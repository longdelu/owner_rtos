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





#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_CRITICAL_H */
