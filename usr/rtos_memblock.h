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
    rtos_task_event_t event;

    /** \breif 存储块的首地址 */
    void *mem_addr_start;

    /** \breif 每个存储块的大小 */
    uint32_t block_size;

    /** \breif 总的存储块的个数 */
    uint32_t max_count;

    /** \breif 存储块列表  */
    rtos_task_list_t block_list;
}rtos_memblock_t;
    

/**
 * \brief 初始化存储控制块
 * \param[in] p_memblock: 指向等待初始化的存储控制块的指针
 * \param[in] mem_addr_start:  存储区的起始地址
 * \param[in] block_size: 每个块的大小
 * \param[in] block_cnt: 总的块数量
 * \return  无
 */ 
void rtos_memblock_init (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr_start, uint32_t block_size, uint32_t block_cnt);    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */

