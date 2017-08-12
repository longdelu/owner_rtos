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
    rtos_task_event_t memblock_event;

    /** \breif �洢����׵�ַ */
    void *mem_addr_start;

    /** \breif ÿ���洢��Ĵ�С */
    uint32_t block_size;

    /** \breif �ܵĴ洢��ĸ��� */
    uint32_t max_count;

    /** \breif �洢���б�  */
    rtos_task_list_t block_list;
}rtos_memblock_t;


/** \brief �洢��״̬��Ϣ�ṹ������ */
typedef struct rtos_memblock_info
{
    /** \brief ��ǰ�洢��ļ��� */
    uint32_t cur_count;

    /** \brief �洢������������� */
    uint32_t max_count;

    /** \brief ÿ���洢��Ĵ�С */
    uint32_t block_size;

    /** \brief ��ǰ�ȴ���������� */
    uint32_t task_count;
}rtos_memblock_info_t;


/**
 * \brief ��ʼ���洢���ƿ�
 * \param[in] p_memblock: ָ��ȴ���ʼ���Ĵ洢���ƿ��ָ��
 * \param[in] mem_addr_start:  �洢������ʼ��ַ
 * \param[in] block_size: ÿ����Ĵ�С
 * \param[in] block_cnt: �ܵĿ�����
 * \return  ��
 */ 
void rtos_memblock_init (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr_start, uint32_t block_size, uint32_t block_cnt);

/**
 * \brief �ȴ��洢��
 * \param[in] p_memblock: ָ��洢���ƿ��ָ��
 * \param[in] pp_mem_addr:  �洢��洢�ĵ�ַ
 * \param[in] wait_ticks: ��û�д洢��ʱ���ȴ���ticks����Ϊ0ʱ��ʾ��Զ�ȴ�
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK,-RTOS_TIMEOUT
 */
int32_t rtos_memblock_wait (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr, uint32_t wait_ticks);  

/**
 * \brief  ��ȡ�洢�飬���û�д洢�飬�������˻�
 * \param[in] p_memblock: ָ��洢���ƿ��ָ��
 * \param[in] pp_mem_addr:  �洢��洢�ĵ�ַ
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK
 */
int32_t rtos_memblock_get (rtos_memblock_t *p_memblock, uint8_t **pp_mem_addr);

/**
 * \brief  ֪ͨ�洢����ã����ѵȴ������е�һ�����񣬻��߽��洢�����洢�����������
 * \param[in] p_memblock: ָ��洢���ƿ��ָ��
 * \param[in] p_mem_addr:  �洢��洢�ĵ�ַ
 * \return  �ȴ����,-RTOS_UNUSED.RTOS_OK
 */
void rtos_memblock_notify (rtos_memblock_t *p_memblock, uint8_t *p_mem_addr);

/**
 * \brief��ѯ�洢���ƿ��״̬��Ϣ
 * \param[in] p_memblock: ָ��洢���ƿ��ָ��
 * \param[in] p_info:     ָ��洢���ƿ���Ϣ״̬�ṹ�ݵ�ָ��
 * \return ��
 */  
void rtos_memblock_info_get (rtos_memblock_t *p_memblock, rtos_memblock_info_t *p_info);

/**       
 * \brief ���ٴ洢���ƿ�  
 * \param[in] p_memblock: ָ��洢���ƿ��ָ��
 * \return ���ѵ���������
 */  
uint32_t rtos_memblock_destroy (rtos_memblock_t *p_memblock);
    
#ifdef __cplusplus
}
#endif

#endif /* ___RTOS_MEMBLOCK_H */

