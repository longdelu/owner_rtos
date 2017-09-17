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
#include "stm32f4xx_hal_led_init.h"

#if SUPPORT_OS   /* ʹ��OS */    

#include "rtos_task_critical.h"

#endif

//TIM_HandleTypeDef TIM2_Handler;      /** < \brief ��ʱ��2���  */
TIM_HandleTypeDef TIM3_Handler;      /** < \brief ��ʱ��3���  */
//TIM_HandleTypeDef TIM4_Handler;      /** < \brief ��ʱ��4���  */
//TIM_HandleTypeDef TIM5_Handler;      /** < \brief ��ʱ��5���  */
//TIM_HandleTypeDef TIM9_Handler;      /** < \brief ��ʱ��9���  */
//TIM_HandleTypeDef TIM10_Handler;     /** < \brief ��ʱ��10��� */
//TIM_HandleTypeDef TIM11_Handler;     /** < \brief ��ʱ��11��� */
//TIM_HandleTypeDef TIM12_Handler;     /** < \brief ��ʱ��12��� */
//TIM_HandleTypeDef TIM13_Handler;     /** < \brief ��ʱ��13��� */
//TIM_HandleTypeDef TIM14_Handler;     /** < \brief ��ʱ��14��� */

static void __tim_init_enable (TIM_TypeDef *p_tim_base) 
{
    uint32_t tim_base = (uint32_t)p_tim_base;
    switch (tim_base) {
    case (uint32_t) TIM2 :
        __HAL_RCC_TIM2_CLK_ENABLE();           //����TIM2ʱ�� 
        HAL_NVIC_EnableIRQ(TIM2_IRQn);         //����ITM2�ж�       
        break;
    
    case (uint32_t) TIM3 :
        __HAL_RCC_TIM3_CLK_ENABLE();          //����TIM3ʱ�� 
        HAL_NVIC_EnableIRQ(TIM3_IRQn);        //����ITM3�ж�     
        break;  
    
    case (uint32_t) TIM4 :
        __HAL_RCC_TIM4_CLK_ENABLE();           //����TIM4ʱ��  
        HAL_NVIC_EnableIRQ(TIM4_IRQn);         //����ITM4�ж�        
        break;
    
    case (uint32_t) TIM5 :
        __HAL_RCC_TIM5_CLK_ENABLE();           //����TIM5ʱ�� 
        HAL_NVIC_EnableIRQ(TIM5_IRQn);         //����ITM5�ж�     
        break;
    
    case (uint32_t) TIM9 :
        __HAL_RCC_TIM9_CLK_ENABLE();           //����TIM9ʱ��  
        HAL_NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);//����ITM9�ж�     
        break;
    
    case (uint32_t) TIM10 :
        __HAL_RCC_TIM10_CLK_ENABLE();           //����TIM10ʱ��        
        break;
    
    case (uint32_t) TIM11 :
        __HAL_RCC_TIM11_CLK_ENABLE();           //����TIM11ʱ��        
        break;
    
    case (uint32_t) TIM12 :
        __HAL_RCC_TIM12_CLK_ENABLE();           //����TIM12Hʱ��        
        break;
    
    case (uint32_t) TIM13 :
        __HAL_RCC_TIM13_CLK_ENABLE();           //����TIM13ʱ��        
        break; 
    
    case (uint32_t) TIM14 :
        __HAL_RCC_TIM14_CLK_ENABLE();           //����TIM14ʱ��        
        break;

    default:
        break;            
        
    }          
}



/**
  * @brief  This function call by HAL_GPIO_Init, set the uart pin and clk 
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *p_tim_handler)
{
     __tim_init_enable(p_tim_handler->Instance);             //ʹ��TIMʱ��
}

 
/**
  * @brief  This function Initializes tim
  * @retval None
  */
void stm32f4xx_hal_tim_init(TIM_HandleTypeDef *p_tim_handler, TIM_TypeDef *p_tim_base, uint32_t arr, uint32_t psc)
{    
    p_tim_handler->Instance=p_tim_base;                       //��ʱ����ַ
    p_tim_handler->Init.Prescaler=psc;                        //��Ƶϵ��
    p_tim_handler->Init.CounterMode=TIM_COUNTERMODE_UP;       //���ϼ�����
    p_tim_handler->Init.Period=arr;                           //�Զ�װ��ֵ
    p_tim_handler->Init.ClockDivision=TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
    HAL_TIM_Base_Init(p_tim_handler);
    
    HAL_TIM_Base_Start_IT(p_tim_handler);                     //ʹ�ܶ�ʱ���Ͷ�ʱ�������жϣ�TIM_IT_UPDATE                          
    
}


/**
 * \brief ��ʱ��3�жϷ�����
 */
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM3_Handler);
    

}


/**
 * \brief �ص��������ڶ�ʱ���жϷ���������
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    
#if SUPPORT_OS   /* ʹ��OS */    
    rtos_interupt_enter();    
#endif
  
    if(htim==(&TIM3_Handler))
    {
        LED1=!LED1;        /* LED1��ת */
#ifdef LWIP_NO_SUPPORT_OS
        extern uint32_t lwip_localtime;           //lwip����ʱ�������,��λ:ms
        lwip_localtime += 10;
#endif
        
    }
    
#if SUPPORT_OS   /* ʹ��OS */    
    rtos_interupt_exit();    
#endif  
    
}


 
/* end of file */ 







             




