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

/* LDR 为读取内存数据到内核寄存器中 */
/* STR 把内核寄存器的数据写到内存当中 */
/* STMDB 把内核寄存器的数据写到内存当中 */
/* LDMIA 为读取内存数据到内核寄存器中  */

__asm void PendSV_Handler (void)
{
    IMPORT currentTask
    
    LDR     R0, = currentTask        //将 currentTask 这个指针变量的地址保存到RO寄存器当中
    
    /* 利用指针变量的地址加载指针变量的值 */
    LDR     R0, [R0]                 //从R0寄存器指向的地址中取出一个字的数据,存储到R0寄存器中，取得了 run_task 的地址 
    
    /* 利用指针变量的地址加载指针变量的值 */
    LDR     R0, [R0]                 //从R0寄存器指向的地址中取出一个字的数据,存储到R0寄存器中，取得了 task_stack 的值，这个值指向 task_stack_buf 这个数组

   /* 先把RO的寄存器的值递减后，再把R4-R11寄存器里面的内容放到 RO, 即task_stack_buf 里面， 最后把RO重新写入当前的值 */
   STMDB    R0!,  {R4-R11}  //先加载R11,再加载R4

  /* 利用指针变量的地址加载指针变量的值,得到run_task的地址 */
   LDR      R1,  = currentTask
   
  /* 利用run_task的地址加载改地址上内存的值,得到 task_stack 的值， 这个值指向 task_stack_buf 这个数组最后一个元素 */
   LDR      R1,  [R1]
   
   /*　取得写入的最后一个数据的地址，通俗点是是让其重新指向栈顶, 把R0中存的数存到以 run_task为地址的单元数据当中 */   
   STR      R0,   [R1]


   ADD      R4,    R4,    #1    
   ADD      R5,    R5,    #1 
   
   
   /* 把R0指向的地址的内存数据恢复R4-R11中 */
   LDMIA    R0!, {R4-R11}
   
   /* 异常返回 */
   BX        LR
   
     
}





/* end of file */
