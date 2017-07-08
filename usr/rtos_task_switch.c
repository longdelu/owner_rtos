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
#include "arm.h"
#include "c_lib.h"

/* LDR Ϊ��ȡ�ڴ����ݵ��ں˼Ĵ����� */
/* STR ���ں˼Ĵ���������д���ڴ浱�� */
/* STMDB ���ں˼Ĵ���������д���ڴ浱�� */
/* LDMIA Ϊ��ȡ�ڴ����ݵ��ں˼Ĵ�����  */

__asm void PendSV_Handler (void)
{
    IMPORT currentTask
    
    LDR     R0, = currentTask        //�� currentTask ���ָ������ĵ�ַ���浽RO�Ĵ�������
    
    /* ����ָ������ĵ�ַ����ָ�������ֵ */
    LDR     R0, [R0]                 //��R0�Ĵ���ָ��ĵ�ַ��ȡ��һ���ֵ�����,�洢��R0�Ĵ����У�ȡ���� run_task �ĵ�ַ 
    
    /* ����ָ������ĵ�ַ����ָ�������ֵ */
    LDR     R0, [R0]                 //��R0�Ĵ���ָ��ĵ�ַ��ȡ��һ���ֵ�����,�洢��R0�Ĵ����У�ȡ���� task_stack ��ֵ�����ֵָ�� task_stack_buf �������

   /* �Ȱ�RO�ļĴ�����ֵ�ݼ����ٰ�R4-R11�Ĵ�����������ݷŵ� RO, ��task_stack_buf ���棬 ����RO����д�뵱ǰ��ֵ */
   STMDB    R0!,  {R4-R11}  //�ȼ���R11,�ټ���R4

  /* ����ָ������ĵ�ַ����ָ�������ֵ,�õ�run_task�ĵ�ַ */
   LDR      R1,  = currentTask
   
  /* ����run_task�ĵ�ַ���ظĵ�ַ���ڴ��ֵ,�õ� task_stack ��ֵ�� ���ֵָ�� task_stack_buf ����������һ��Ԫ�� */
   LDR      R1,  [R1]
   
   /*��ȡ��д������һ�����ݵĵ�ַ��ͨ�׵�������������ָ��ջ��, ��R0�д�����浽�� run_taskΪ��ַ�ĵ�Ԫ���ݵ��� */   
   STR      R0,   [R1]


   ADD      R4,    R4,    #1    
   ADD      R5,    R5,    #1 
   
   
   /* ��R0ָ��ĵ�ַ���ڴ����ݻָ�R4-R11�� */
   LDMIA    R0!, {R4-R11}
   
   /* �쳣���� */
   BX        LR
   
     
}





/* end of file */
