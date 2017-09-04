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
 * \brief microlib ΢�������� 
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */

#ifndef __RTOS_MICROLIB_ADAPTER_H
#define __RTOS_MICROLIB_ADAPTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "c_lib.h"
#include "arm.h"
#include "rtos_common.h"

    
/**
 * \brief ����ѡ�����ʵ��Ӳ��ƽ̨�ṩ
 */
typedef struct microlib_adapter_ops {
    int (*pfn_console_send) (void *p_cookie, const void *p_buf, size_t cnt);
    int (*pfn_console_receive) (void *p_cookie, void *p_buf, size_t cnt);
} microlib_adapter_ops_t;

/*******************************************************************************
   Global Variables
*******************************************************************************/
typedef struct microlib_adapter {
    const microlib_adapter_ops_t *p_ops;    /** < \brief ��������*/
    void                          *p_cookie; /** < \brief ������������  */
} microlib_adapter_t;

/**
 * \brief ��������ʼ��
 *
 * \param[in] p_ops     : ��������
 * \param[in] p_cookie  : ������������
 *
 * \return AM_OK
 */
int microlib_adapter_init (const microlib_adapter_ops_t *p_ops,
                           void                         *p_cookie);
    
    


#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MICROLIB_ADAPTER_H */ 

 




