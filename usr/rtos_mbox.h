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

/** \brief ������Ϣ˳�� */
#define RTOS_MBOX_SENDNORMAL             0x00        // ��Ϣ������������β��
#define RTOS_MBOX_SENDFRONT              0x01        // ��Ϣ������������ͷ��    
    
/** \brief ��Ϣ�����¼����ƿ� */       
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
    
    /** 
     * \brief ��Ϣ�洢������, һ��p_msg_buf[i]��Ϣ��������ָ��һ��һά����Ϣ���飬������һ����άָ��
     * \note ��ס��ָ�뵱�� int _a[10]; int *p = _a; �� *(p + 0) = p[0] = a[0]; 
     *       eg:  int a[10];
                  int b[10]
                  int c[10];
                  void *g_msg_buf[3] = {a, b,c}
     *              
     *            p_msg_buf    = g_msg_buf;    
     *            p_msg_buf[0] = g_msg_buf[0]
     *            p_msg_buf[0][0] = g_msg_buf[0][0] = a[0]  
     * 
     */ 
    void **p_msg_buf;    
    
}rtos_mbox_t;  

/** \brief ��Ϣ����״̬��Ϣ�ṹ�� */  
typedef struct rtos_mbox_info {
    /** \brief ��ǰ����Ϣ���� */
    uint32_t msg_cur_count;

    /** \brief ����������ɵ���Ϣ���� */
    uint32_t max_msg_count;

    /** \brief ��ǰ�ȴ����������  */
    uint32_t task_count;
}rtos_mbox_info_t;


/**
 * \brief ��ʼ����Ϣ����
 * \param[in] p_mbox: ָ��ȴ���ʼ������Ϣ����ṹ���ָ��
 * \param[in] p_msg_buf:  ��Ϣ�洢������
 * \param[in] max_count: ��Ϣ���������, ͨ����˵����ж��ٸ�һά����
 * \return  ��
 */  
void rtos_mbox_init(rtos_mbox_t *p_mbox, void **p_msg_buf, uint32_t msg_max_count);


/**
 * \brief �ȴ�����, ��ȡһ����Ϣ
 * \param[in] p_mbox: ָ����Ϣ����ṹ���ָ��
 * \param[in] pp_msg_buf:  ��Ϣ�洢������
 * \param[in] wait_ticks: ���ȴ���ticks����Ϊ0��ʾ���޵ȴ�
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */ 
int32_t rtos_mbox_wait (rtos_mbox_t *p_mbox, void **pp_msg_buf, uint32_t wait_ticks);

/**
 * \brief  ��ȡһ����Ϣ�����û����Ϣ���������˻�
 * \param[in] p_mbox: ָ����Ϣ����ṹ���ָ��
 * \param[in] pp_msg_buf:  ��Ϣ�洢������
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK,
 */ 
int32_t rtos_mbox_nowait_get (rtos_mbox_t *p_mbox, void **pp_msg_buf);


/**
 * \brief  ֪ͨ��Ϣ���ã����ѵȴ������е�һ�����񣬻��߽���Ϣ���뵽������
 * \param[in] p_mbox: ָ����Ϣ����ṹ���ָ��
 * \param[in] p_msg:  ��������Ϣ
 * \param[in] notify_option:  ��������Ϣѡ�� RTOS_MBOX_SENDNORMAL �� RTOS_MBOX_SENDFRONT
 * \return  -RTOS_FULL RTOS_OK  
 */ 
int32_t  rtos_mbox_notify (rtos_mbox_t *p_mbox, void *p_msg, uint32_t notify_option);

/**
 * \brief   ���������������Ϣ
 * \param[in] p_mbox: ָ����Ϣ����ṹ���ָ��
 * \return  �� 
 */ 
void rtos_mbox_flush (rtos_mbox_t *p_mbox);

/**
 * \brief   ��������
 * \param[in] p_mbox: ָ����Ϣ����ṹ���ָ��
 * \return  �����ٸ��ź��������ѵ���������
 */ 
uint32_t rtos_mbox_destroy (rtos_mbox_t *p_mbox);

/**       
 * \brief ��ѯ��Ϣ����״̬��Ϣ
 * \param[in] p_sem:  ָ������ź����ṹ���ָ��
 * \param[in] p_info: ָ����Ϣ����״̬��Ϣ�ṹ���ָ��  
 * \return  ��
 */  
void rtos_mbox_info_get (rtos_mbox_t *p_mbox, rtos_mbox_info_t *p_info);   
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MBOX_H */

