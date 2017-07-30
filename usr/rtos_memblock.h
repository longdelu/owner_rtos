/**
 * \file
 * \brief RTOS 存储块头文件
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */


#ifndef __RTOS_MEMBLOCK_H
#define __RTOS_MEMBLOCK_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "c_lib.h"
#include "rtos_task_event.h"

/** \brief 存储块结构体声明 */
typedef struct rtos_memblock
{
    /** \breif 事件控制块，当任务需要该存储块时，而存储块没有空闲时，可以让任务等待 */
    rtos_task_event_t memblock_event;

    /** \breif 存储块的首地址 */
    void *mem_addr_start;

    /** \breif 每个存储块的大小 */
    uint32_t block_size;

    /** \breif 总的存储块的个数 */
    uint32_t max_count;

    /** \breif 存储块列表  */
    rtos_task_list_t block_list;
}rtos_memblock_t;


/** \brief 存储块状态信息结构体声明 */
typedef struct rtos_memblock_info
{
    /** \brief 当前存储块的计数 */
    uint32_t cur_count;

    /** \brief 存储块允许的最大计数 */
    uint32_t max_count;

    /** \brief 每个存储块的大小 */
    uint32_t block_size;

    /** \brief 当前等待的任务计数 */
    uint32_t task_count;
}rtos_memblock_info_t;


/**
 * \brief 初始化存储控制块
 * \param[in] p_memblock: 指向等待初始化的存储控制块的指针
 * \param[in] mem_addr_start:  存储区的起始地址
 * \param[in] block_size: 每个块的大小
 * \param[in] block_cnt: 总的块数量
 * \return  无
 */ 
void rtos_memblock_init (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr_start, uint32_t block_size, uint32_t block_cnt);

/**
 * \brief 等待存储块
 * \param[in] p_memblock: 指向存储控制块的指针
 * \param[in] pp_mem_addr:  存储块存储的地址
 * \param[in] wait_ticks: 当没有存储块时，等待的ticks数，为0时表示永远等待
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */
int32_t rtos_memblock_wait (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr, uint32_t wait_ticks);  

/**
 * \brief  获取存储块，如果没有存储块，则立即退回
 * \param[in] p_memblock: 指向存储控制块的指针
 * \param[in] pp_mem_addr:  存储块存储的地址
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK
 */
int32_t rtos_memblock_get (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr);

/**
 * \brief  通知存储块可用，唤醒等待队列中的一个任务，或者将存储块加入存储块队列链表中
 * \param[in] p_memblock: 指向存储控制块的指针
 * \param[in] p_mem_addr:  存储块存储的地址
 * \return  等待结果,-RTOS_UNUSED.RTOS_OK
 */
void rtos_memblock_notify (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr);

/**
 * \brief查询存储控制块的状态信息
 * \param[in] p_memblock: 指向存储控制块的指针
 * \param[in] p_info:     指向存储控制块信息状态结构休的指针
 * \return 无
 */  
void rtos_memblock_info_get (rtos_memblock_t *p_memblock, rtos_memblock_info_t *p_info);

/**       
 * \brief 销毁存储控制块  
 * \param[in] p_memblock: 指向存储控制块的指针
 * \return 唤醒的任务数量
 */  
uint32_t rtos_memblock_destroy (rtos_memblock_t *p_memblock);
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */

