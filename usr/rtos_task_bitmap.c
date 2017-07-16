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
 * \brief 操作系统位图数据结构
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "RTE_Components.h"             // Component selection
#include "rtos_task_bitmap.h"
#include "arm.h"
#include "c_lib.h"

 
/**
 * \brief 位图数据结构初始化， 初始化p_rtos_task_bitmap将所有的位全清0
 */
void rtos_task_bitmap_init (rtos_task_bitmap_t *p_rtos_task_bitmap)
{
    p_rtos_task_bitmap->bitmap = 0; 
}

/**
 * \brief 返回支持的最大优先级数, 可用优先及为 32 - 1
 */
uint32_t rtos_task_bitmap_prio_support (void)
{
     return 32;
}

/**
 * \brief 设置位图数据结构中的某个位
 */
void rtos_task_bitmap_set (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num)
{
    p_rtos_task_bitmap->bitmap |= 1UL << bit_num;
}                       

/**
 * \brief 清除位图数据结构中的某个位
 */
void rtos_task_bitmap_clr (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num)
{
    p_rtos_task_bitmap->bitmap &= ~(1UL << bit_num);
}

/**
 * \brief 从位图中第0位开始查找，找到第1个被设置的位置序号，返回的值为最先置1的位
 */
uint32_t rtos_task_bitmap_first_set_get (rtos_task_bitmap_t *p_rtos_task_bitmap) 
{
    /*　分组查找，将8位某1位置1的数值算出，填到表中　*/  
    static const uint8_t quick_find_bitmaptable[] =     
    {
        /* 00 */ 0xff, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 10 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 20 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 30 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 40 */ 6,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 50 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 60 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 70 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 80 */ 7,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* 90 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* A0 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* B0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* C0 */ 6,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* D0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* E0 */ 5,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        /* F0 */ 4,    0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
    };

     /*　第一组最低位起始位置为第0位 */
    if (p_rtos_task_bitmap->bitmap & 0xff)
    {
        return quick_find_bitmaptable[p_rtos_task_bitmap->bitmap & 0xff];         
    }
    /*　第二组最低位起始位置为第8位 */
    else if (p_rtos_task_bitmap->bitmap & 0xff00)
    {
        return quick_find_bitmaptable[(p_rtos_task_bitmap->bitmap >> 8) & 0xff] + 8;        
    }
    
    /*　第三组最低位起始位置为第16位 */
    else if (p_rtos_task_bitmap->bitmap & 0xff0000)
    {
        return quick_find_bitmaptable[(p_rtos_task_bitmap->bitmap >> 16) & 0xff] + 16;        
    }
    
    /*　第三组最低位起始位置为第16位 */
    else if (p_rtos_task_bitmap->bitmap & 0xFF000000)
    {
        return quick_find_bitmaptable[(p_rtos_task_bitmap->bitmap >> 24) & 0xFF] + 24;
    }
    
   /* 为0，都没有置位，查找到了极限，返回第32位，表示没有任务优先级 */
    else
    {
        return rtos_task_bitmap_prio_support();
    }
}





 
 
 
 


