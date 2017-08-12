#ifndef __RTOS_TASK_DELAY_H
#define __RTOS_TASK_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"    


/**
 * \brief  ���ص�ǰ����ϵͳ�δ�ʱ�Ӽ���
 *
 * \param[in] ��
 *
 * \return    ��ǰ����ϵͳ�δ�ʱ�Ӽ���
 */    
uint32_t rtos_get_systick (void); 
    

/**
 * \brief  ��������ȵ���ʱ
 *
 * \param[in] ��ʱ���ڣ���λ���ڣ���С��ʱΪ10ms
 *   
 * \note      Ҫ��������Ӧ��tick����
 *
 * \return    ��
 */      
void rtos_mdelay (int32_t ms);   


/**
 * \brief  ����ϵͳ�δ�ʱ�ӳ�ʼ������
 *
 * \param[in] ms: �δ�ʱ�ص����ڣ���λms
 *
 * \return    ��
 */
void rtos_systick_init (uint32_t ms);


/**
 * \brief  ��������ȵ���ʱ
 *
 * \param[in] ��ʱ���ڣ���λ���ڣ���С��ʱΪ10ms
 *   
 * \note      Ҫ��������Ӧ��tick����
 *
 * \return    ��
 */      
void rtos_sched_mdelay (uint32_t ms);   



#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_DELAY_H */
