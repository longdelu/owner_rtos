/**
 * \file
 * \brief RTOS ��Ϣ����ͷ�ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_MBOX_H
#define __RTOS_MBOX_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
    
typedef struct rtos_mbox {
    /** \brief �¼����ƿ� */
    rtos_task_event_t mbox_event;

    /** \brief ��ǰ����Ϣ���� */
    uint32_t msg_cur_count;  

    /** \brief ����Ϣ������ */
    uint32_t read_index;   

    /** \brief д��Ϣ������ */
    uint32_t write_index;  

    /** \brief  ����������ɵ���Ϣ���� */
    uint32_t msg_max_count;
    
    /** \brief  ��Ϣ�洢������ */
    void **p_msg_buf;    
    
}rtos_mbox_t;  


/**
 * \brief ��ʼ����Ϣ����
 * \param[in] p_mbox: ָ��ȴ���ʼ��������ṹ���ָ��
 * \param[in] p_msg_buf:  ��Ϣ�洢������
 * \param[in] max_count: ��Ϣ���������
 * \return  ��
 */  
void rtos_mbox_init(rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t max_count);
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MBOX_H */

