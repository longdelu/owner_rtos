#ifndef __RTOS_TASK_H
#define __RTOS_TASK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "c_lib.h"  
    
/* Cortex-M�Ķ�ջ��Ԫ���ͣ���ջ��Ԫ�Ĵ�СΪ32λ������ʹ��uint32_t */
typedef uint32_t taskstack_t;
    
typedef struct rtos_task {
   
    /** \brief ��ջջ����ַ */    
    uint32_t *task_stack_top;
    
    /** \brief ��ջ���𼴵�ַ */
    uint32_t * stack_base;

    /** \brief ��ջ�������� */
    uint32_t stack_size;
    
}rtos_task_t;

/**
 * \brief  �����ʼ������
 *
 * \param[in] task: ����ṹ��ָ��
 * \param[in] task_entry: ������ں���
 * \param[in] p_arg: ������ں�������
 * \param[in] task_prio: ��������ȼ�
 * \param[in] task_stack: �����ջ��ָ��
 * \param[in] task_stack_size: �����ջ�Ĵ�С,���ֽ�Ϊ��λ
 *
 * \return    ��
 */
void rtos_task_init(rtos_task_t * task, 
                    void (*task_entry) (void *p_arg), 
                    void *p_arg, 
                    uint32_t task_prio, 
                    uint32_t *task_stack,
                    uint32_t task_stack_size);
                    
                    
/**
 * \brief �л�����һ���������У���������ϵͳ��һ������
 *                    
 * \param[in] ��
 *
 * \return    ��                                      
 */
void rtos_task_run_first (void);                    
                    
                    
/**
 * \brief ������Ⱥ��� 
 *                    
 * \param[in] ��
 *
 * \return    ��
 */
void rtos_task_sched(void);                    


#ifdef __cplusplus
}
#endif

#endif /* __RTOS_TASK_H */

/* end of file */

