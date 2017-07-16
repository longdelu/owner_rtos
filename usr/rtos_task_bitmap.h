#ifndef __RTOS_TASK_CRITICAL_H
#define __RTOS_TASK_CRITICAL_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"

typedef struct rtos_task_bitmap {
    
    /** \brief λͼ���ݽṹ */
    uint32_t bitmap;
    
}rtos_task_bitmap_t;


/**
 * \brief λͼ���ݽṹ��ʼ���� ��ʼ��p_rtos_task_bitmap�����е�λȫ��0
 * \param[in] p_rtos_task_bitmap, λͼ���ݽṹ��ָ��
 * \return  ��
 */
void rtos_task_bitmap_init (rtos_task_bitmap_t *p_rtos_task_bitmap);


/**
 * \brief ����֧�ֵ�������ȼ���, �������ȼ�Ϊ 32 - 1
 * \param[in] ��
 * \return  ���֧�����ȼ�������
 */
uint32_t rtos_task_bitmap_prio_support (void);


/**
 * \brief ����λͼ���ݽṹ�е�ĳ��λ
 * \param[in] p_rtos_task_bitmap, λͼ���ݽṹ��ָ��
 * \param[in] bit_num : ��λͼ���ݽṹĳ1λ��1
 * \return ��
 */
void rtos_task_bitmap_set (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num);
                      

/**
 * \brief ���λͼ���ݽṹ�е�ĳ��λ
 * \param[in] p_rtos_task_bitmap, λͼ���ݽṹ��ָ��
 * \param[in] bit_num : ���λͼ���ݽṹ��ĳ1λ
 * \return ��
 */
void rtos_task_bitmap_clr (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num);


/**
 * \brief ��λͼ�е�0λ��ʼ���ң��ҵ���1�������õ�λ����ţ�
 * \param[in] p_rtos_task_bitmap, λͼ���ݽṹ��ָ��
 * \return  ���ص�ֵΪ������1��λ����λ����)
 */
uint32_t rtos_task_bitmap_first_set_get (rtos_task_bitmap_t *p_rtos_task_bitmap); 



#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_BITMAPL_H */
