#ifndef __ARM_H
#define __ARM_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ARMCM3
#include "ARMCM3.H"
#endif
    
#ifdef STM32F429xx
#include "stm32f4xx.h"    
#endif
    
    
//以下为汇编函数
void WFI_SET(void);		//执行WFI指令
void INTX_DISABLE(void);//关闭所有中断
void INTX_ENABLE(void);	//开启所有中断
void MSR_MSP(uint32_t addr);	//设置堆栈地址     


#ifdef __cplusplus
}
#endif

#endif /* __ARM_H */



