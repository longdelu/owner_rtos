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
 * \brief RTOS ռ���ʽӿ��ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_CPU_USE_H
#define __RTOS_CPU_USE_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_config.h"

/**
 * \brief ��������������д���++
 */ 
void rtos_cpu_idle_count_inc (void );
    
/**
 * \brief Ϊ���cpuʹ������ϵͳʱ�ӽ���ͬ��
 * \param[in] ��
 * \return    ��    
 */ 
void  rtos_cpu_use_sync_with_systick (void);   
    
    
/**
 * \brief  ��ʼ��cpuͳ��
 */  
void rtos_cpu_use_init (void);


/**
 * \brief ���cpuʹ����
 */ 
void rtos_cpu_use_check (void);

/**
 * \brief cpuʹ���ʻ�ȡ
 */ 
float rtos_cpu_use_get (void) ;

    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_CPU_USE_H */
