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
 * \brief ����ϵͳλͼ���ݽṹ
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
#ifdef ARMCM3
#include "RTE_Components.h"
#endif  
#include "rtos_task_bitmap.h"
#include "arm.h"
#include "c_lib.h"
#include "rtos_config.h"

 
/**
 * \brief λͼ���ݽṹ��ʼ���� ��ʼ��p_rtos_task_bitmap�����е�λȫ��0
 */
void rtos_task_bitmap_init (rtos_task_bitmap_t *p_rtos_task_bitmap)
{
    p_rtos_task_bitmap->bitmap = 0; 
}

/**
 * \brief ����֧�ֵ�������ȼ���, �������ȼ�Ϊ 32 - 1
 */
uint32_t rtos_task_bitmap_prio_support (void)
{
     return 32;
}

/**
 * \brief ����λͼ���ݽṹ�е�ĳ��λ
 */
void rtos_task_bitmap_set (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num)
{
    p_rtos_task_bitmap->bitmap |= 1UL << bit_num;
}                       

/**
 * \brief ���λͼ���ݽṹ�е�ĳ��λ
 */
void rtos_task_bitmap_clr (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num)
{
    p_rtos_task_bitmap->bitmap &= ~(1UL << bit_num);
}

/**
 * \brief ��λͼ�е�0λ��ʼ���ң��ҵ���1�������õ�λ����ţ����ص�ֵΪ������1��λ
 */
uint32_t rtos_task_bitmap_first_set_get (rtos_task_bitmap_t *p_rtos_task_bitmap) 
{
    /*��������ң���8λ��ɵ�256���������λΪ1����ֵ�����������С�*/  
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

     /*����һ�����λ��ʼλ��Ϊ��0λ */
    if (p_rtos_task_bitmap->bitmap & 0xff)
    {
        return quick_find_bitmaptable[p_rtos_task_bitmap->bitmap & 0xff];         
    }
    /*���ڶ������λ��ʼλ��Ϊ��8λ */
    else if (p_rtos_task_bitmap->bitmap & 0xff00)
    {
        return quick_find_bitmaptable[(p_rtos_task_bitmap->bitmap >> 8) & 0xff] + 8;        
    }
    
    /*�����������λ��ʼλ��Ϊ��16λ */
    else if (p_rtos_task_bitmap->bitmap & 0xff0000)
    {
        return quick_find_bitmaptable[(p_rtos_task_bitmap->bitmap >> 16) & 0xff] + 16;        
    }
    
    /*�����������λ��ʼλ��Ϊ��16λ */
    else if (p_rtos_task_bitmap->bitmap & 0xFF000000)
    {
        return quick_find_bitmaptable[(p_rtos_task_bitmap->bitmap >> 24) & 0xFF] + 24;
    }
    
   /* Ϊ0����û����λ�����ҵ��˼��ޣ����ص�32λ����ʾû���������ȼ� */
    else
    {
        return rtos_task_bitmap_prio_support();
    }
}





 
 
 
 


