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
 * \brief 触发任务切换函数入口
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#include "arm.h"
#include "c_lib.h"


/**
 * \brief 触发PendSVC
 */
void PendSVC_Trigger(void)
{
    SCB->ICSR = SCB_ICSR_PENDSVSET_Msk;
}


/**
 * \brief 任务切换 
 */
void rtos_task_switch (void)
{
    PendSVC_Trigger();
}    



/* LDR 为读取内存数据到内核寄存器中 */
/* STR 把内核寄存器的数据写到内存当中 */
/* STMDB 把内核寄存器的数据写到内存当中 */
/* LDMIA 为读取内存数据到内核寄存器中  */
/* MRS 指令一般用于获取CPU相关的状态寄存器  */

__asm void PendSV_Handler (void)
{
    IMPORT p_current_task
    IMPORT p_next_task
  
    /* 获取当前任务的堆栈指针 */    
    MRS     R0, PSP                
    
    /* if 这是由 rtos_task_switch ()触发的(此时，PSP肯定不会是0了，0的话必定是 rtos_task_run_first() 触发的 */
    CBZ     R0, PendSVHandler_nosave    

    /* 先把RO的寄存器的值递减后，再把R11-R4寄存器里面的内容放到以RO的值为地址的内存单元里面, 即PSP指向的用户堆栈里面， 最后向RO重新写入当前的值R0的值 */
    STMDB    R0!,  {R4-R11}  //先加载R11,再加载R4
    
    
    LDR     R1, = p_current_task   //将 p_current_task 这个指针变量的地址保存到R1寄存器当中
    
    /* 利用指针变量的地址加载指针变量的值 */
    LDR     R1, [R1]                 //从R1寄存器指向的地址中取出一个字的数据,存储到R1寄存器中，取得了 run_task 的地址 
    
    /*　取得写入的最后一个数据的地址，通俗点是是让其重新指向栈顶, 把R0中存的数据（栈顶的地址）加载到 run_task的 task_stack_top, 即更该变个指针的值   */ 
    STR     R0, [R1]                 //
   
   
PendSVHandler_nosave 
    LDR     R0, = p_current_task      //将 p_current_task 这个指针变量的地址保存到RO寄存器当中    
    LDR     R1, = p_next_task         //将 p_nxet_task 这个指针变量的地址保存到RO寄存器当中

    LDR     R2, [R1]  
    STR     R2, [R0]                  // 先将 p_current_task这个指针指向的值设置为 p_nxet_task 指针指向的值，也就是下一任务变成了当前任务   
    
    /* 通过 p_current_task 这个指针变量的值来获得当前作任务栈顶的指针 */
    LDR     R0, [R2]
   
   
   /* 把R0指向的地址的内存数据恢复R4-R11中,每放一个R0地址就加1  */
   LDMIA    R0!, {R4-R11}   // 先恢复 R4， 再恢复R11 
   
   /* 硬件自动恢复 R0 ,R1, R2, R3, LR(R14), PC(R15), XPSR */
   
   
   /* 最后，恢复真正的堆栈指针到PSP, 此时PSP的值应该是该任务的栈顶地址，与栈顶指针的值相等  */  
   MSR     PSP, R0                   // 
   
   /* 标记下返回标记，指明在退出LR时，切换到PSP堆栈中(PendSV使用的是MSP) */ 
   ORR     LR, LR, #0x04             // 
   
   /* 异常返回 */
   BX        LR
   
   /*　退出时，会再一步恢复PSP的值，以及RO-R3的值 */
    
}



                                                        
/* end of file */
