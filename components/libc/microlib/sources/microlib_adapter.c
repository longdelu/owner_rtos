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
 * \brief C��������
 * \internal
 * \par Modification history
 * - 1.00 17-08-10  nwt, first implementation.
 * \endinternal
 */
 
#include "microlib_adapter.h"
#include "stm32f4xx_hal_uart.h"

static microlib_adapter_t  __g_adapter;

#pragma import(__use_no_semihosting)       
      
/** \brief ��׼����Ҫ��֧�ֺ��� */
                 
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout; 
       
/**
 * \brief ����_sys_exit()�Ա���ʹ�ð�����ģʽ��������ģʽ��ӡ�벻����������
 */
void _sys_exit(int x) 
{ 
    x = x; 
} 
/**
 * \brief fputc�����ض���
 */
int fputc (int ch, FILE *f)
{ 
    UART_HandleTypeDef *p_uart_handler = (UART_HandleTypeDef *)__g_adapter.p_cookie;        
    while ((p_uart_handler->Instance->SR & 0x40) == 0);        /* �ȴ������������           */
    p_uart_handler->Instance->DR = (uint8_t)ch;                         /* ��������                   */
    return ch;
}

/**
 * \brief fgetc�����ض���
 */
int fgetc (FILE *f)
{   
    int ch;
    
    UART_HandleTypeDef *p_uart_handler = (UART_HandleTypeDef *)__g_adapter.p_cookie;        
   
    while ((p_uart_handler->Instance->SR & 0x20) == 0);                             /* �ȴ����յ�����               */
    ch = p_uart_handler->Instance->DR;                                              /* ��ȡ����                     */
    return ch;
}

/*******************************************************************************
   Public Functions
*******************************************************************************/
int microlib_adapter_init (const microlib_adapter_ops_t *p_ops,
                           void                          *p_cookie)
{

    __g_adapter.p_ops    = p_ops;
    __g_adapter.p_cookie = p_cookie;

    return RTOS_OK;
}


 




