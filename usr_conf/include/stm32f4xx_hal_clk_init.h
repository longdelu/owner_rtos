/**
  ******************************************************************************
  * @file    stm32f4xx_hal_clk_init.h
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   Header file of CLK_INIT HAL module.
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_HAL_CLK_INIT_H
#define __STM32F4xx_HAL_CLK_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
     
/**
 * \brief clk device information.
 */
typedef struct stm32f4xx_clk_devinfo {

    /** \brief apb1ʱ�ӷ�Ƶ�������� 1,2,4,8,16 ֮����ֵ */    
    uint32_t apb1_div;
    
    /** \brief apb2ʱ�ӷ�Ƶ�������� 1,2,4,8,16 ֮����ֵ */    
    uint32_t apb2_div;
    
    /** \brief ahbʱ�ӷ�Ƶ�������� 1- 512 ֮����ֵ */
    uint32_t ahb_div
    ;
    /** \brief ��pll��Ƶϵ�� */
    uint32_t plln;
    
    /** \brief ������pll����Ƶpll֮ǰ��Ƶϵ�� */
    uint32_t pllm;    

    /** \brief ������pll֮��ķ�Ƶϵ�� */
    uint32_t pllp;    
    
    /** \brief usb/sdio/������������ȵ�����Ƶϵ�� */
    uint32_t pllq;   

} stm32f4xx_clk_devinfo_t;


/**
 * \brief CLK �豸�ṹ��
 */
typedef struct stm32f4xx_clk_dev {

    /** \brief ָ��CLK �豸��Ϣ��ָ��   */
    const stm32f4xx_clk_devinfo_t *p_devinfo;

    /** \brief system clock frequency,divIde from main clock. */
    uint32_t sysclk;   
    
    /** \brief  the ahb clock frequency */
    uint32_t ahb_clk;

    /** \brief  the apb clock frequency */
    uint32_t apb1_clk;

    /** \brief  the apb clock frequency */
    uint32_t apb2_clk;

} stm32f4xx_clk_dev_t;


extern  stm32f4xx_clk_devinfo_t clk_info;

extern stm32f4xx_clk_dev_t clk_dev;

     

/**
  * @brief  This function Initializes  clk
  * @param  p_dev:    ʱ���豸�ṹ���ָ��
  * @param  p_deinfo: ʱ���豸��Ϣ�ṹ���ָ��
  * @retval HAL status
  */
int stm32f4xx_hal_clk_init(stm32f4xx_clk_dev_t *p_dev, stm32f4xx_clk_devinfo_t *p_deinfo);


/**
  * @brief ��ȡʱ��Ƶ��
  *             
  * @param  clk_id: 0Ϊ��ȡϵͳʱ��Ƶ�ʣ�1ΪAHBʱ��Ƶ��; 2ΪAPB1ʱ��Ƶ��; 3ΪAPB2ʱ��Ƶ��
  *
  * @retval the clk frq;
  */
uint32_t stm32f4xx_clk_frq_get (uint32_t clk_id);


#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_CLK_INIT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
