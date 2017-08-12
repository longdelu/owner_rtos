/**
 * \file
 * \brief ����ϵͳ����ʵ�� 
 * \note  ����Ƴ�֮�����ظ�ɾ��
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_task_list.h"
#include "rtos_config.h"

/** \brief �����Ǽ򻯴����д��ӵĺ� */
#define first_node   head_node.p_next
#define last_node    head_node.p_prev

/**
 * \brief RTOS���������ʼ�� 
 */  
void rtos_task_list_init (rtos_task_list_t *p_rtos_task_list)
{
     p_rtos_task_list->head_node.p_next = &p_rtos_task_list->head_node; 
     p_rtos_task_list->head_node.p_prev = &p_rtos_task_list->head_node;  

     p_rtos_task_list->count            = 0;
}

/**
 * \brief ͳ�������н�������
 */
uint32_t rtos_task_list_count (rtos_task_list_t *p_rtos_task_list)
{
    return p_rtos_task_list->count;    
}

/**
 * \brief ��������������׸���� 
 */
dlist_node_t *rtos_task_list_begin_get(rtos_task_list_t *p_rtos_task_list)
{
    if (p_rtos_task_list->count != 0) {
        
        return dlist_begin_get(&p_rtos_task_list->head_node);
       
    } else {        
        return NULL;
    }        
}

/**
 * \brief ����������������һ����� 
 */
dlist_node_t *rtos_task_list_tail_get(rtos_task_list_t *p_rtos_task_list)
{
    if (p_rtos_task_list->count != 0) {
        
        return dlist_tail_get(&p_rtos_task_list->head_node);
       
    } else {        
        return NULL;
    }        
}

/**
 * \brief ������������ָ��������һ����� 
 */
dlist_node_t *rtos_task_list_prev_get(rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node)
{
    if (p_rtos_task_list->count != 0) {
        
        return dlist_prev_get(&p_rtos_task_list->head_node, p_node);
       
    } else {        
        return NULL;
    }        
}

/**
 * \brief ������������ָ��������һ����� 
 */
dlist_node_t *rtos_task_list_next_get(rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node)
{
    if (p_rtos_task_list->count != 0) {
        
        return dlist_next_get(&p_rtos_task_list->head_node, p_node);
       
    } else {        
        return NULL;
    }        
}


/**
 * \brief �Ƴ��������������еĽ��
 */
void rtos_task_list_remove_all (rtos_task_list_t *p_rtos_task_list)
{
    /* ��������     */
    dlist_destroy(&p_rtos_task_list->head_node);

    /* ��������� */
    p_rtos_task_list->count = 0;  
}


/**
 * \brief ����������ͷ��������
 */
void rtos_task_list_add_head (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node)
{
    /* ��ʼ����� */
    dlist_init(p_node);
    
    dlist_add_head(&p_rtos_task_list->head_node,p_node); 

    p_rtos_task_list->count++;    
    
}

/**
 * \brief ����������β��������
 */
void rtos_task_list_add_tail (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node)
{
    /* ��ʼ����� */
    dlist_init(p_node);
    
    dlist_add_tail(&p_rtos_task_list->head_node,p_node); 

    p_rtos_task_list->count++;    
    
}



/**
 * \brief ����������ָ��λ�ú��������
 */
void rtos_task_list_add (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_pos, dlist_node_t *p_node)
{
    /* ��ʼ����� */
    dlist_init(p_node);
    
    dlist_add(&p_rtos_task_list->head_node, p_pos, p_node); 

    p_rtos_task_list->count++;    
    
}



/**
 * \brief ������������ɾ����1���û���㣬ɾ��֮��������ָ������
 */
dlist_node_t *rtos_task_list_remove_first (rtos_task_list_t *p_rtos_task_list)
{
    
    int ret = 0;
    
    dlist_head_t *p_node =  p_rtos_task_list->head_node.p_next;
  
    /* ɾ����� */    
    ret = dlist_del(&p_rtos_task_list->head_node, p_rtos_task_list->head_node.p_next);
    
    if (ret == RTOS_OK) {
        
        p_rtos_task_list->count--;
        
        return p_node;
        
    } else {
        return NULL;
    }
}



/**
 * \brief ������������ɾ�����1���û���㣬ɾ��֮��������ָ������
 */
dlist_node_t * rtos_task_list_remove_last (rtos_task_list_t * p_rtos_task_list)
{
    
    int ret = 0;
    
    dlist_head_t *p_node = p_rtos_task_list->head_node.p_prev;
    
    ret = dlist_del(&p_rtos_task_list->head_node, p_rtos_task_list->head_node.p_prev);
    
    if (ret == RTOS_OK) {
        
        p_rtos_task_list->count--;
        
        return p_node;
        
    } else {                                     
        return NULL;
    }
}

/**
 * \brief ����������ɾ��ָ����㣬ɾ��֮��������ָ������
 */
void rtos_task_list_remove (rtos_task_list_t * p_rtos_task_list, dlist_node_t *p_node)
{
    
     dlist_del(&p_rtos_task_list->head_node, p_node);
    
     p_rtos_task_list->count--;
}


 
 
 
 /* end of file */
 
 
 
 


