#ifndef __RTOS_TASK_BITMAP_H
#define __RTOS_TASK_BITMAP_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"

typedef struct rtos_task_bitmap {
    
    /** \brief 位图数据结构 */
    uint32_t bitmap;
    
}rtos_task_bitmap_t;


/**
 * \brief 位图数据结构初始化， 初始化p_rtos_task_bitmap将所有的位全清0
 * \param[in] p_rtos_task_bitmap, 位图数据结构体指针
 * \return  无
 */
void rtos_task_bitmap_init (rtos_task_bitmap_t *p_rtos_task_bitmap);


/**
 * \brief 返回支持的最大优先级数, 可用优先及为 32 - 1
 * \param[in] 无
 * \return  最大支持优先级的数量
 */
uint32_t rtos_task_bitmap_prio_support (void);


/**
 * \brief 设置位图数据结构中的某个位
 * \param[in] p_rtos_task_bitmap, 位图数据结构体指针
 * \param[in] bit_num : 给位图数据结构某1位置1
 * \return 无
 */
void rtos_task_bitmap_set (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num);
                      

/**
 * \brief 清除位图数据结构中的某个位
 * \param[in] p_rtos_task_bitmap, 位图数据结构体指针
 * \param[in] bit_num : 清除位图数据结构的某1位
 * \return 无
 */
void rtos_task_bitmap_clr (rtos_task_bitmap_t *p_rtos_task_bitmap, uint32_t bit_num);


/**
 * \brief 从位图中第0位开始查找，找到第1个被设置的位置序号，
 * \param[in] p_rtos_task_bitmap, 位图数据结构体指针
 * \return  返回的值为最先置1的位（低位优先)
 */
uint32_t rtos_task_bitmap_first_set_get (rtos_task_bitmap_t *p_rtos_task_bitmap); 



#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_TASK_BITMAPL_H */
