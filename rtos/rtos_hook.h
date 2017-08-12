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
 * \brief RTOS ���Ӻ����ӿ��ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_HOOK_H
#define __RTOS_HOOK_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task.h"
    
/**
 * \brief cpu����ʱ��hooks
 * \param �� 
 * \return ��
 */ 
void rtos_hook_cpu_idle (void);


/**
 * \brief ʱ�ӽ���Hooks
 * \param �� 
 * \return ��
 */ 
void rtos_hook_systick (void);

/**
 * \brief �����л�hooks
 * \param[in] p_task_from: ����ṹ��ָ��
 * \param[in] p_task_to: ����ṹ��ָ��
 * \return ��
 */ 
void rtos_hook_task_swtich (rtos_task_t *p_task_from, rtos_task_t *p_task_to);


/**
 * \brief �����ʼ����Hooks
 * \param[in] p_task: ����ṹ��ָ��
 * \return ��
 */ 
void rtos_hook_task_init (rtos_task_t *p_task); 
    

    

#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_HOOK_H */
