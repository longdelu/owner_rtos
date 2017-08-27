/**
  ******************************************************************************
  * @file    stm32f4xx_hal_rcc.c
  * @author  MCD Application Team
  * @version V1.4.2
  * @date    10-November-2015
  * @brief   RCC HAL module driver.
  *          This file provides firmware functions to manage the following 
  *          functionalities of the Reset and Clock Control (RCC) peripheral:
  *           + Initialization and de-initialization functions
  *           + Peripheral Control functions
  *       
  @verbatim                
  ==============================================================================
                      ##### RCC specific features #####
  ==============================================================================
    [..]  
      After reset the device is running from Internal High Speed oscillator 
      (HSI 16MHz) with Flash 0 wait state, Flash prefetch buffer, D-Cache 
      and I-Cache are disabled, and all peripherals are off except internal
      SRAM, Flash and JTAG.
      (+) There is no prescaler on High speed (AHB) and Low speed (APB) busses;
          all peripherals mapped on these busses are running at HSI speed.
      (+) The clock for all peripherals is switched off, except the SRAM and FLASH.
      (+) All GPIOs are in input floating state, except the JTAG pins which
          are assigned to be used for debug purpose.
    
    [..]          
      Once the device started from reset, the user application has to:        
      (+) Configure the clock source to be used to drive the System clock
          (if the application needs higher frequency/performance)
      (+) Configure the System clock frequency and Flash settings  
      (+) Configure the AHB and APB busses prescalers
      (+) Enable the clock for the peripheral(s) to be used
      (+) Configure the clock source(s) for peripherals which clocks are not
          derived from the System clock (I2S, RTC, ADC, USB OTG FS/SDIO/RNG)

                      ##### RCC Limitations #####
  ==============================================================================
    [..]  
      A delay between an RCC peripheral clock enable and the effective peripheral 
      enabling should be taken into account in order to manage the peripheral read/write 
      from/to registers.
      (+) This delay depends on the peripheral mapping.
      (+) If peripheral is mapped on AHB: the delay is 2 AHB clock cycle 
          after the clock enable bit is set on the hardware register
      (+) If peripheral is mapped on APB: the delay is 2 APB clock cycle 
          after the clock enable bit is set on the hardware register

    [..]  
      Implemented Workaround:
      (+) For AHB & APB peripherals, a dummy read to the peripheral register has been
          inserted in each __HAL_RCC_PPP_CLK_ENABLE() macro.

  @endverbatim
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_HAL_Driver
  * @{
  */

/** @defgroup RCC CLK INIT
  * @brief RCC HAL module driver
  * @{
  */
  
stm32f4xx_clk_devinfo_t clk_info = {
    RCC_HCLK_DIV4,
    RCC_HCLK_DIV2, 
    RCC_SYSCLK_DIV1,
    360,
    25,
    2,
    8, 
};

stm32f4xx_clk_dev_t clk_dev;



/**
  * @brief  This function Initializes  clk
  * @retval HAL status
  */
int stm32f4xx_hal_clk_init (stm32f4xx_clk_dev_t *p_dev, stm32f4xx_clk_devinfo_t *p_deinfo)
{
   
    if (p_dev == NULL || p_deinfo == NULL) {
        return -1;
    }
    
    p_dev->p_devinfo = p_deinfo; 
    
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //ʹ��PWRʱ��
    
    //������������������õ�ѹ�������ѹ�����Ա�������δ�����Ƶ�ʹ���
    //ʱʹ�����빦��ʵ��ƽ�⣬�˹���ֻ��STM32F42xx��STM32F43xx�����У�
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//���õ�ѹ�������ѹ����1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    //ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      //��HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;        //��PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;//PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLM = p_deinfo->pllm; //��PLL����ƵPLL��Ƶϵ��(PLL֮ǰ�ķ�Ƶ),ȡֵ��Χ:2~63.
    RCC_OscInitStructure.PLL.PLLN = p_deinfo->plln; //��PLL��Ƶϵ��(PLL��Ƶ),ȡֵ��Χ:64~432.  
    RCC_OscInitStructure.PLL.PLLP = p_deinfo->pllp; //ϵͳʱ�ӵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2,4,6,8.(������4��ֵ!)
    RCC_OscInitStructure.PLL.PLLQ = p_deinfo->pllq; //USB/SDIO/������������ȵ���PLL��Ƶϵ��(PLL֮��ķ�Ƶ),ȡֵ��Χ:2~15.
    
    /* ��ʼ�� */
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
    
    if(ret!=HAL_OK) while(1);
    
    ret=HAL_PWREx_EnableOverDrive(); //����Over-Driver����
    if(ret!=HAL_OK) while(1);
    
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider= p_deinfo->ahb_div;//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=p_deinfo->apb1_div; //APB1��Ƶϵ��Ϊ4
    RCC_ClkInitStructure.APB2CLKDivider=p_deinfo->apb2_div; //APB2��Ƶϵ��Ϊ2
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);//ͬʱ����FLASH��ʱ����Ϊ5WS��Ҳ����6��CPU���ڡ�
            
    if(ret!=HAL_OK) {
        while(1);    
    }
    
    /* ����ϵͳʱ��Ƶ�� */
    SystemCoreClockUpdate();
    
    p_dev->sysclk  =  SystemCoreClock;
    p_dev->ahb_clk =  SystemCoreClock;
    p_dev->apb1_clk =  p_dev->ahb_clk / 4;    
    p_dev->apb2_clk =  p_dev->ahb_clk / 2;    

    return  HAL_OK;    
    
}

/**
  * @brief ��ȡʱ��Ƶ��
  */
uint32_t stm32f4xx_clk_frq_get (uint32_t clk_id)
{
    uint32_t clk_frq = 0;
    
    switch (clk_id) {
    case 0:
        clk_frq = clk_dev.sysclk;
        break;
    
    case 1:
        clk_frq = clk_dev.ahb_clk;    
        break;
    
    case 2:
        clk_frq = clk_dev.apb1_clk;  
        break;
    
    case 3:
        clk_frq = clk_dev.apb2_clk;  
        break;
    
    default:
            
        break;
   
    }
    
    return clk_frq;
    
}



/**
  * @}
  */


/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
