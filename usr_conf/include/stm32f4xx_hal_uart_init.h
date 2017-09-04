/**
  ******************************************************************************
  * @file    stm32f4xx_hal_uart_init.h
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   Header file of UART_INIT HAL module.
  ***********************************************************************  *******
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_UART_INIT_H
#define __STM32F4xx_HAL_UART_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
     
#define RXBUFFERSIZE         1     /** < \brief 缓存大小 */
#define USART_REC_LEN        200   /** < \brief 定义最大接收字节数 200 */ 

#define ENABLE_UART_REC_INT  1     /** < \brief 使用串口接收中断 */ 

extern uint8_t USART_RX_BUF[USART_REC_LEN];     /* 接收缓冲,最大USART_REC_LEN个字节. */

//接收状态
//bit15，    接收完成标志
//bit14，    接收到0x0d
//bit13~0，    接收到的有效字节数目
extern uint16_t USART_RX_STA;       //接收状态标记    

extern uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲    
     
/**
  * @brief  This function Initializes  uart
  * @param  p_uart_handler:    串口句柄
  * @param  p_uart_base:       串口  
  * @param  bound_rate:        串口波特率          
  * @retval None
  */
void stm32f4xx_uart_init(UART_HandleTypeDef *p_uart_handler, USART_TypeDef *p_uart_base, uint32_t bound_rate);
     
     
/* UART句柄 */
extern UART_HandleTypeDef UART1_Handler;


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_UART_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
