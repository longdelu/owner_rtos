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
 * \brief C库适配器
 * \internal
 * \par Modification history
 * - 1.00 17-08-10  nwt, first implementation.
 * \endinternal
 */
 
#include "microlib_adapter.h"
#include "stm32f4xx_hal_uart.h"

static microlib_adapter_t  __g_adapter;

#pragma import(__use_no_semihosting)       
      
/** \brief 标准库需要的支持函数 */
                 
struct __FILE 
{ 
    int handle; 
}; 

FILE __stdout; 
       
/**
 * \brief 定义_sys_exit()以避免使用半主机模式（半主机模式打印离不开调试器）
 */
void _sys_exit(int x) 
{ 
    x = x; 
} 
/**
 * \brief fputc函数重定义
 */
int fputc (int ch, FILE *f)
{ 
    UART_HandleTypeDef *p_uart_handler = (UART_HandleTypeDef *)__g_adapter.p_cookie;        
    while ((p_uart_handler->Instance->SR & 0x40) == 0);        /* 等待发送数据完成           */
    p_uart_handler->Instance->DR = ch;                         /* 发送数据                   */
    return ch;
}

/**
 * \brief fgetc函数重定义
 */
int fgetc (FILE *f)
{   
    int ch;
    
    UART_HandleTypeDef *p_uart_handler = (UART_HandleTypeDef *)__g_adapter.p_cookie;        
   
    while ((p_uart_handler->Instance->SR & 0x20) == 0);                             /* 等待接收到数据               */
    ch = p_uart_handler->Instance->DR;                                              /* 读取数据                     */
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


 




