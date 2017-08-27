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
 * \brief ���������л��������
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "rtos_task_switch.h"


/**
 * \brief ����PendSVC
 */
void PendSVC_Trigger(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}


/**
 * \brief �����л� 
 */
void rtos_task_switch (void)
{
    PendSVC_Trigger();
}    



/* LDR Ϊ��ȡ�ڴ����ݵ��ں˼Ĵ����� */
/* STR ���ں˼Ĵ���������д���ڴ浱�� */
/* STMDB ���ں˼Ĵ���������д���ڴ浱�� */
/* LDMIA Ϊ��ȡ�ڴ����ݵ��ں˼Ĵ�����  */
/* MRS ָ��һ�����ڻ�ȡCPU��ص�״̬�Ĵ���  */

__asm void PendSV_Handler (void)
{
    IMPORT p_current_task
    IMPORT p_next_task
  
    /* ��ȡ��ǰ����Ķ�ջָ�� */    
    MRS     R0, PSP                
    
    /* if ������ rtos_task_switch ()������(��ʱ��PSP�϶�������0�ˣ�0�Ļ��ض��� rtos_task_run_first() ������ */
    CBZ     R0, PendSVHandler_nosave   
       
    /* 
     * �Ȱ�RO�ļĴ�����ֵ�ݼ����ٰ�S0-S15,FPSCR�Ĵ�����������ݷŵ���RO��ֵΪ��ַ���ڴ浥Ԫ����, 
     * ��PSPָ����û���ջ����. ע����ʹ��FPU��ʱ��Ӳ���Զ���ջS0-S15,FPSCR 
     */ 
#ifdef STM32F429xx     
    TST      R14,  #0X10      /* �ж��Ƿ�ʹ��Ӳ��FPU */
    IT       EQ
    VSTMDBEQ   R0!, {S16-S31} /* ���ʹ�ã���ѹջS16-S31 */
#endif
    
    
    /* �Ȱ�RO�ļĴ�����ֵ�ݼ����ٰ�R11-R4�Ĵ�����������ݷŵ���RO��ֵΪ��ַ���ڴ浥Ԫ����, ��PSPָ����û���ջ���棬 �����RO����д�뵱ǰ��ֵR0��ֵ */
    STMDB    R0!,  {R4-R11}  //�ȼ���R11,�ټ���R4
      
    LDR     R1, = p_current_task   //�� p_current_task ���ָ������ĵ�ַ���浽R1�Ĵ�������
    
    /* ����ָ������ĵ�ַ����ָ�������ֵ */
    LDR     R1, [R1]                 //��R1�Ĵ���ָ��ĵ�ַ��ȡ��һ���ֵ�����,�洢��R1�Ĵ����У�ȡ���� run_task �ĵ�ַ 
    
    /*��ȡ��д������һ�����ݵĵ�ַ��ͨ�׵�������������ָ��ջ��, ��R0�д�����ݣ�ջ���ĵ�ַ�����ص� run_task�� task_stack_top, �����ñ��ָ���ֵ   */ 
    STR     R0, [R1]                 //
   
   
PendSVHandler_nosave 
    LDR     R0, = p_current_task      //�� p_current_task ���ָ������ĵ�ַ���浽RO�Ĵ�������    
    LDR     R1, = p_next_task         //�� p_nxet_task ���ָ������ĵ�ַ���浽RO�Ĵ�������

    LDR     R2, [R1]  
    STR     R2, [R0]                  // �Ƚ� p_current_task���ָ��ָ���ֵ����Ϊ p_nxet_task ָ��ָ���ֵ��Ҳ������һ�������˵�ǰ����   
      
    
    /* ͨ�� p_current_task ���ָ�������ֵ����õ�ǰ������ջ����ָ�� */
    LDR     R0, [R2]
    
    /* ��R0ָ��ĵ�ַ���ڴ����ݻָ�R4-R11��,ÿ��һ��R0��ַ�ͼ�1  */
    LDMIA    R0!, {R4-R11}   // �Ȼָ� R4�� �ٻָ�R11 

#ifdef STM32F429xx     
    TST      R14,  #0X10      /* �ж��Ƿ�ʹ��Ӳ��FPU */
    IT       EQ  
      
    VLDMIAEQ  R0!, {S16-S31}  /* ���ʹ�ã����ջ�ָ�S16-S31 */
#endif   
   
    /* Ӳ���Զ��ָ� R0 ,R1, R2, R3, LR(R14), PC(R15), XPSR */
   
   
    /* ��󣬻ָ������Ķ�ջָ�뵽PSP, ��ʱPSP��ֵӦ���Ǹ������ջ����ַ����ջ��ָ���ֵ���  */  
    MSR     PSP, R0                   // 
   
    /* ����·��ر�ǣ�ָ�����˳�LRʱ���л���PSP��ջ��(PendSVʹ�õ���MSP) */ 
    ORR     LR, LR, #0x04             // 
   
    /* �쳣���� */
    BX        LR
   
    /*���˳�ʱ������һ���ָ�PSP��ֵ���Լ�RO-R3��ֵ�����ʹ��FPU�Ļ�������ָ�S0~S15��FPSCR */
    
}

#if (RTOS_CPU_ARM_FP_EN == 1)
/**
 * \brief Ӳ�������ջ
 */
void  rtos_cpu_fp_reg_push  (void)
{
 
}

/**
 * \brief Ӳ�������ջ
 */
void  rtos_cpu_fp_reg_pop  (void)
    
{

}

#endif






                                                        
/* end of file */
