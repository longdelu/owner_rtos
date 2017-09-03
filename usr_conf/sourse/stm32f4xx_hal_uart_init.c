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
 * \brief ���ڳ�ʼ������ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-08-10  nwt, first implementation.
 * \endinternal
 */
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

/* UART��� */
UART_HandleTypeDef UART1_Handler;




/**
  * @brief  This function call by HAL_GPIO_Init, set the uart pin and clk 
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *p_huart_base)
{
    /* GPIO�˿����� */
    GPIO_InitTypeDef GPIO_Initure;
    
    if(p_huart_base->Instance==USART1) {          //����Ǵ���1�����д���1 MSP��ʼ��
        __HAL_RCC_GPIOA_CLK_ENABLE();             //ʹ��GPIOAʱ��
        __HAL_RCC_USART1_CLK_ENABLE();            //ʹ��USART1ʱ��
    
        GPIO_Initure.Pin=GPIO_PIN_9;              //PA9
        GPIO_Initure.Mode=GPIO_MODE_AF_PP;        //�����������
        GPIO_Initure.Pull=GPIO_PULLUP;            //����
        GPIO_Initure.Speed=GPIO_SPEED_FAST;       //����
        GPIO_Initure.Alternate=GPIO_AF7_USART1;   //����ΪUSART1
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);       //��ʼ��PA9

        GPIO_Initure.Pin=GPIO_PIN_10;             //PA10
        HAL_GPIO_Init(GPIOA,&GPIO_Initure);       //��ʼ��PA10
          
    }

}
 
/**
  * @brief  This function Initializes uart
  * @retval HAL status
  */
void stm32f4xx_uart_init(UART_HandleTypeDef *p_uart_handler, USART_TypeDef *p_uart_base, uint32_t bound_rate)
{    
    /* UART ��ʼ������ */
    p_uart_handler->Instance=p_uart_base;                        //USART1
    p_uart_handler->Init.BaudRate=bound_rate;                    //������
    p_uart_handler->Init.WordLength=UART_WORDLENGTH_8B;          //�ֳ�Ϊ8λ���ݸ�ʽ
    p_uart_handler->Init.StopBits=UART_STOPBITS_1;               //һ��ֹͣλ
    p_uart_handler->Init.Parity=UART_PARITY_NONE;                //����żУ��λ
    p_uart_handler->Init.HwFlowCtl=UART_HWCONTROL_NONE;          //��Ӳ������
    p_uart_handler->Init.Mode=UART_MODE_TX_RX;                   //�շ�ģʽ
    HAL_UART_Init(p_uart_handler);                               //HAL_UART_Init()��ʹ��UART
    
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 /* end of file */ 

  

