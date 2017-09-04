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
     
#define RXBUFFERSIZE         1     /** < \brief �����С */
#define USART_REC_LEN        200   /** < \brief �����������ֽ��� 200 */ 

#define ENABLE_UART_REC_INT  1     /** < \brief ʹ�ô��ڽ����ж� */ 

extern uint8_t USART_RX_BUF[USART_REC_LEN];     /* ���ջ���,���USART_REC_LEN���ֽ�. */

//����״̬
//bit15��    ������ɱ�־
//bit14��    ���յ�0x0d
//bit13~0��    ���յ�����Ч�ֽ���Ŀ
extern uint16_t USART_RX_STA;       //����״̬���    

extern uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���    
     
/**
  * @brief  This function Initializes  uart
  * @param  p_uart_handler:    ���ھ��
  * @param  p_uart_base:       ����  
  * @param  bound_rate:        ���ڲ�����          
  * @retval None
  */
void stm32f4xx_uart_init(UART_HandleTypeDef *p_uart_handler, USART_TypeDef *p_uart_base, uint32_t bound_rate);
     
     
/* UART��� */
extern UART_HandleTypeDef UART1_Handler;


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_UART_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
