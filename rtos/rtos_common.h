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
 * \brief RTOS�����ļ�
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_TASK_COMMON_H
#define __RTOS_TASK_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"

    
/**
 * \name RTOS ������� 
 * @{
 */     
#define  RTOS_OK                         0   /** \brief �����ɹ�OK */               
#define  RTOS_ERROR                      1   /** \brief ����ʧ�� */
#define  RTOS_EINVAILD                   2   /** \brief ������Ч */  
#define  RTOS_TIMEOUT                    3   /** \brief ������ʱ */
#define  RTOS_UNUSED                     4   /** \brief ��Դ������ */
#define  RTOS_DEL                        5   /** \brief ��Դ��ɾ�� */
#define  RTOS_FULL                       6   /** \brief ��Դ���� */
#define  RTOS_ERROR_OWER                 7   /** \brief��ƥ��������� */


/** @} */
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_COMMON_H */




