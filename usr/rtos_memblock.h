/**
 * \file
 * \brief RTOS �洢��ͷ�ļ�
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

/** \brief �洢��ṹ������ */
typedef struct rtos_memblock
{
    /** \breif �¼����ƿ飬��������Ҫ�ô洢��ʱ�����洢��û�п���ʱ������������ȴ� */
    rtos_task_event_t event;

    /** \breif �洢����׵�ַ */
    void *mem_addr_start;

    /** \breif ÿ���洢��Ĵ�С */
    uint32_t block_size;

    /** \breif �ܵĴ洢��ĸ��� */
    uint32_t max_count;

    /** \breif �洢���б�  */
    rtos_task_list_t block_list;
}rtos_memblock_t;
    

/**
 * \brief ��ʼ���洢���ƿ�
 * \param[in] p_memblock: ָ��ȴ���ʼ���Ĵ洢���ƿ��ָ��
 * \param[in] mem_addr_start:  �洢������ʼ��ַ
 * \param[in] block_size: ÿ����Ĵ�С
 * \param[in] block_cnt: �ܵĿ�����
 * \return  ��
 */ 
void rtos_memblock_init (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr_start, uint32_t block_size, uint32_t block_cnt);    
    
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */

