/*******************************************************************************
*                                 Embedded_rtos
*                       ----------------------------
*                       innovating embedded platform
*
* Copyright (c) 2001-2017 Free soft.
* All rights reserved.
*
* Contact information:
* web site:    
* e-mail:      
*******************************************************************************/

/**
 * \file
 * \brief RTOS �¼����ƿ�ָ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#ifndef __RTOS_FLAGGROUP_H
#define __RTOS_FLAGGROUP_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"
    
    
#define RTOS_FALG_GRP_CLR        (0UL << 0)         /** \brief �¼������λ��0 */
#define RTOS_FALG_GRP_SET        (1UL << 0)         /** \brief �¼������λ��1 */
#define RTOS_FALG_GRP_ANY        (0UL << 1)         /** \brief �¼����������λ�仯 */
#define RTOS_FALG_GRP_ALL        (1UL << 1)         /** \brief �¼����������λ�仯 */

/** \brief �¼����������λ��1 */    
#define RTOS_FLAG_GRP_ALL_SET    (RTOS_FALG_GRP_SET | RTOS_FALG_GRP_ALL) 
    
/** \brief�¼�������κ�һλ��1*/    
#define RTOS_FLAG_GRP_ANY_SET    (RTOS_FALG_GRP_SET | RTOS_FALG_GRP_ANY) 
    
/** \brief �¼����������λ��0 */
#define RTOS_FLAG_GRP_ALL_CLR    (RTOS_FALG_GRP_CLR | RTOS_FALG_GRP_ALL) 
    
/** \brief �¼�������κ�һλ��0 */
#define RTOS_FLAG_GRP_ANY_CLR    (RTOS_FALG_GRP_CLR | RTOS_FALG_GRP_ANY)   

/** \brief �¼�����鱻����     */
#define RTOS_FLAG_GRP_CONSUME    (1UL << 7)

/** \brief �¼������ṹ������ */
typedef struct rtos_flag_grp {
    
    /** \brief �¼����ƿ�   */
    rtos_task_event_t flag_grp_event;
    
    /** \brief ��ǰ�¼���� */
    uint32_t flag;
    
}rtos_flag_grp_t;

/** \brief �¼������ṹ������ */
typedef struct rtos_flag_grp_info_get {
    
    /** \brief ��ǰ�ȴ�����ļ���   */
    uint32_t task_count;
    
    /** \brief ��ǰ�¼���� */
    uint32_t flag;
    
    
}rtos_flag_grp_info_get_t;


/**
 * \brief ��ʼ���¼������
 * \param[in] p_flag_grp: ָ���¼������ṹ���ָ��
 * \param[in] flag:  ��ʼ���¼���־
 * \return ��
 */  
void rtos_flag_grp_init(rtos_flag_grp_t *p_flag_grp, uint32_t flag);



/**
 * \brief �ȴ��¼������
 * \param[in] p_flag_grp: ָ���¼������ṹ���ָ��
 * \param[in] wait_type:  �ȴ����¼����ͣ�ֵΪ RTOS_FLAG_GRP_ANY_* ��һ��� (# RTOS_FLAG_GRP_ANY_CLR)
 * \param[in] req_flag:   �ȴ����¼���־��ı��
 * \param[out] p_req_result:  �ȴ���ǵĽ��,����ȴ�����Щ�¼����λ
 * \param[in] wait_ticks:  ���¼���������Ϊ0ʱ���ȴ���ticks����Ϊ0ʱ��ʾ��Զ�ȴ�
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */  
int32_t rtos_flag_grp_wait (rtos_flag_grp_t *p_flag_grp, 
                            uint32_t wait_type, 
                            uint32_t req_flag, 
                            uint32_t *p_req_result, 
                            uint32_t wait_ticks);

/**
 * \brief ��ȡ�¼�����飬����¼��������������ã��������˻�
 * \param[in] p_flag_grp: ָ���¼������ṹ���ָ��
 * \param[in] wait_type:  �ȴ����¼����ͣ�ֵΪ RTOS_FLAG_GRP_ANY_* ��һ��� (# RTOS_FLAG_GRP_ANY_CLR)
 * \param[in] req_flag:   �ȴ����¼���־��ı��
 * \param[in] p_req_result:  �ȴ���ǵĽ��������ȴ�����Щ�¼����λ
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK
 */  
int32_t rtos_flag_grp_get (rtos_flag_grp_t *p_flag_grp, uint32_t wait_type, uint32_t req_flag, uint32_t *p_req_result);


/**
 * \brief �����¼�������е�ĳһλ�����ѵȴ������е�һ������
 * \param[in] p_flag_grp: ָ���¼������ṹ���ָ��
 * \param[in] set_bit:  �ȴ����¼����ͣ�ֵΪ RTOS_FLAG_GRP_ANY_* ��һ��� (# RTOS_FLAG_GRP_ANY_CLR)
 * \param[in] flag:     �����¼���־��ı��
 * \return  ��
 */  
void rtos_flag_grp_notify (rtos_flag_grp_t *p_flag_grp, uint32_t set_bit, uint32_t flag);



/**
 * \brief  ��ѯ�¼���־���״̬��Ϣ
 * \param[in] p_flag_grp: ָ���¼������ṹ���ָ��
 * \param[in] p_info:  ָ���¼������״̬�ṹ���ָ��
 * \param[in] flag:     
 * \return  ��
 */  
void rtos_flag_grp_info_get (rtos_flag_grp_t *p_flag_grp, rtos_flag_grp_info_get_t *p_info);
 
 
/**
 * \brief  �����¼���־��
 * \param[in] p_flag_grp: ָ���¼������ṹ���ָ��
 * \return  �����ٸ��¼��������ƿ�����ѵ���������
 */  
uint32_t rtos_flag_grp_destroy (rtos_flag_grp_t *p_flag_grp);

    
    
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_FLAGGROUP_H */

