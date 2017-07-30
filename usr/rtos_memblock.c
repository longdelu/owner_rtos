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
 * \brief RTOS 存储块实现
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
 #include "rtos_memblock.h"
 
 
/**
 * \brief 初始化存储控制块
 */ 
void rtos_memblock_init (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr_start, uint32_t block_size, uint32_t block_cnt)
{
    uint8_t *p_memblock_addr_end = p_mem_addr_start + block_size * block_cnt;
    
    if (p_memblock == NULL || p_mem_addr_start == NULL) {
        return;
    } 
    
    /*
     * 每个存储块需要来放置链接指针，所以空间至少要比tNode大
     * 即便如此，实际用户可用的空间并没有少
     */
    if (block_size < sizeof(dlist_node_t)){
        return;
    }

    /* 初始化存储块事件控制块 */
    rtos_task_event_init(&p_memblock->event, RTOS_EVENT_TYPE_MEMBLOCK);

    p_memblock->mem_addr_start = p_mem_addr_start;
    p_memblock->block_size = block_size;
    p_memblock->max_count = block_cnt;

    /* 存储块链表初始化 */
    rtos_task_list_init(&p_memblock->block_list);
    
    while (p_mem_addr_start < p_memblock_addr_end) {
        rtos_task_list_add_tail(&p_memblock->block_list, (dlist_node_t *)p_mem_addr_start);

        p_memblock_addr_end += block_size;
    }

}    
    
 
 
 
/* end of file */




