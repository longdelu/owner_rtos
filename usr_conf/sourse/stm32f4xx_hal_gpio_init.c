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
 * \brief gpio��ʼ������ʵ��
 * \internal
 * \par Modification history
 * - 1.00 17-08-10  nwt, first implementation.
 * \endinternal
 */
 
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h" 

static uint8_t __pin_init_status = 0;

static void __gpio_port_enable (GPIO_TypeDef *p_gpio_port) 
{
    uint32_t port_base = (uint32_t)p_gpio_port;
    switch (port_base) {
    case (uint32_t) GPIOA :
        __HAL_RCC_GPIOA_CLK_ENABLE();           //����GPIOAʱ��        
        break;
    
    case (uint32_t) GPIOB :
        __HAL_RCC_GPIOB_CLK_ENABLE();          //����GPIOBʱ��           
        break;  
    
    case (uint32_t) GPIOC :
        __HAL_RCC_GPIOC_CLK_ENABLE();           //����GPIOCʱ��           
        break;
    
    case (uint32_t) GPIOD :
        __HAL_RCC_GPIOD_CLK_ENABLE();           //����GPIODʱ��          
        break;
    
    case (uint32_t) GPIOE :
        __HAL_RCC_GPIOE_CLK_ENABLE();           //����GPIOEʱ��          
        break;
    
    case (uint32_t) GPIOF :
        __HAL_RCC_GPIOF_CLK_ENABLE();           //����GPIOFʱ��        
        break;
    
    case (uint32_t) GPIOG :
        __HAL_RCC_GPIOG_CLK_ENABLE();           //����GPIOGʱ��        
        break;
    
    case (uint32_t) GPIOH :
        __HAL_RCC_GPIOH_CLK_ENABLE();           //����GPIOHʱ��        
        break;
    
    case (uint32_t) GPIOI :
        __HAL_RCC_GPIOI_CLK_ENABLE();           //����GPIOIʱ��        
        break; 
    
    case (uint32_t) GPIOJ :
        __HAL_RCC_GPIOJ_CLK_ENABLE();           //����GPIOJʱ��        
        break;
    
    case (uint32_t) GPIOK :
         __HAL_RCC_GPIOK_CLK_ENABLE();           //����GPIOKʱ��       
        break; 

    default:
        break;            
        

    }        
    
}


/**
  * @brief  This function Initializes gpio
  * @retval None
  */
void stm32f4xx_gpio_init(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_port, uint8_t pin_init_status)
{ 
    GPIO_InitTypeDef GPIO_Initure;
    
    __gpio_port_enable(p_gpio_port);        //������Ӧ�˿�ʱ��
    
    __pin_init_status = pin_init_status;
    
    
    /* Ĭ�ϳ�ʼ������� */
    if (pin_init_status == 1) {
        HAL_GPIO_WritePin(GPIOB,gpio_pin,GPIO_PIN_SET);  
    } else {
        HAL_GPIO_WritePin(GPIOB,gpio_pin,GPIO_PIN_RESET);  
         
    }
    
    GPIO_Initure.Pin=gpio_pin;               //LED�ܽ�
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(p_gpio_port,&GPIO_Initure);   
}


/**
  * @brief  light on gpio
  */     
void stm32f4xx_gpio_on(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_por)
{
     if (__pin_init_status == 1) {
         HAL_GPIO_WritePin(GPIOB,gpio_pin,GPIO_PIN_RESET);           
                 
     } else {
         HAL_GPIO_WritePin(GPIOB,gpio_pin,GPIO_PIN_SET); 
     }
}    

/**
  * @brief  light off gpio
  */  
void stm32f4xx_gpio_off(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_port)
{
     if (__pin_init_status == 1) {
         HAL_GPIO_WritePin(GPIOB,gpio_pin,GPIO_PIN_SET);           
                 
     } else {
         HAL_GPIO_WritePin(GPIOB,gpio_pin,GPIO_PIN_RESET); 
     }
}    

/**
  * @brief  gpio state toggle
  */  
void stm32f4xx_gpio_toggle(uint32_t gpio_pin, GPIO_TypeDef *p_gpio_port)
{
     HAL_GPIO_TogglePin(p_gpio_port,gpio_pin);      
}    
 
 
 
 
 /* end of file */


  




