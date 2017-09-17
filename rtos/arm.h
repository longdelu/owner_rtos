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
    
    
//����Ϊ��ຯ��
void WFI_SET(void);		//ִ��WFIָ��
void INTX_DISABLE(void);//�ر������ж�
void INTX_ENABLE(void);	//���������ж�
void MSR_MSP(uint32_t addr);	//���ö�ջ��ַ     


#ifdef __cplusplus
}
#endif

#endif /* __ARM_H */



