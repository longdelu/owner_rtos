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
    
typedef struct rtos_flag_group {
    
    /** \brief �¼����ƿ�   */
    rtos_task_event_t flagg_rp_event;
    
    /** \brief ��ǰ�¼���� */
    uint32_t flag;
    
    
}rtos_flag_group_t;


/**
 * \brief ��ʼ���¼������
 * \param[in] p_flag_grp: ָ���¼������ṹ���ָ��
 * \param[in] flag:  ��ʼ���¼���־
 * \return ��
 */  
void rtos_flaggroup_init(rtos_flag_group_t *p_flag_grp, uint32_t flag);
    
    
    
    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_FLAGGROUP_H */

