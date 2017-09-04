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

#if SUPPORT_OS   /* ʹ��OS */    

#include "rtos_task_critical.h"

#endif

/* UART��� */
UART_HandleTypeDef UART1_Handler;


uint8_t USART_RX_BUF[USART_REC_LEN];     /* ���ջ���,���USART_REC_LEN���ֽ�. */

//����״̬
//bit15��    ������ɱ�־
//bit14��    ���յ�0x0d
//bit13~0��    ���յ�����Ч�ֽ���Ŀ
uint16_t USART_RX_STA=0;       //����״̬���    

uint8_t aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���


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
                                                                  
#ifdef ENABLE_UART_REC_INT        
        HAL_NVIC_EnableIRQ(USART1_IRQn);          //ʹ��USART1�ж�ͨ��  
#endif
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
    
#ifdef ENABLE_UART_REC_INT
    __HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);                    //���������ж�

#endif    
    
}

#ifdef ENABLE_UART_REC_INT

/** \brief ����1�жϷ������ */
void USART1_IRQHandler(void)                    
{ 
    uint8_t Res = 0;
    
#if SUPPORT_OS   /* ʹ��OS */    
    rtos_interupt_enter();    
#endif
    
    /* ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� */
    if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
        if((USART_RX_STA&0x8000)==0)//����δ���
        {
            if(USART_RX_STA&0x4000)//���յ���0x0d
            {
                if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
                else USART_RX_STA|=0x8000;    //��������� 
            }
            else //��û�յ�0X0D
            {    
                if(Res==0x0d)USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����      
                }         
            }
        }            
    }
    
    HAL_UART_IRQHandler(&UART1_Handler); 
    
#if SUPPORT_OS   /* ʹ��OS */    
    rtos_interupt_exit();    
#endif    
}

#endif
  
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 /* end of file */ 

  

