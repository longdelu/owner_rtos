/**
 * \file
 * \brief 操作系统链表实现 
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_task_list.h"
#include "rtos_config.h"

/** \brief 以下是简化代码编写添加的宏 */
#define first_node   head_node.p_next
#define last_node    head_node.p_prev

/**
 * \brief RTOS任务链表初始化 
 */  
void rtos_task_list_init (rtos_task_list_t *p_rtos_task_list)
{
     p_rtos_task_list->head_node.p_next = &p_rtos_task_list->head_node; 
     p_rtos_task_list->head_node.p_prev = &p_rtos_task_list->head_node;  

     p_rtos_task_list->count            = 0;
}

/**
 * \brief 统计链表中结点的数量
 */
uint32_t rtos_task_list_count (rtos_task_list_t *p_rtos_task_list)
{
    return p_rtos_task_list->count;    
}

/**
 * \brief 近回任务链表的首个结点 
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
 * \brief 近回任务链表的最后一个结点 
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
 * \brief 近回任务链表指定结点的上一个结点 
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
 * \brief 近回任务链表指定结点的下一个结点 
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
 * \brief 移除任务链表中所有的结点
 */
void rtos_task_list_remove_all (rtos_task_list_t *p_rtos_task_list)
{
    /* 销毁链表     */
    dlist_destroy(&p_rtos_task_list->head_node);

    /* 清零结点计数 */
    p_rtos_task_list->count = 0;  
}


/**
 * \brief 在任务链表头部插入结点
 */
void rtos_task_list_add_head (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node)
{
    /* 初始化结点 */
    dlist_init(p_node);
    
    dlist_add_head(&p_rtos_task_list->head_node,p_node); 

    p_rtos_task_list->count++;    
    
}

/**
 * \brief 在任务链表尾部插入结点
 */
void rtos_task_list_add_tail (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_node)
{
    /* 初始化结点 */
    dlist_init(p_node);
    
    dlist_add_tail(&p_rtos_task_list->head_node,p_node); 

    p_rtos_task_list->count++;    
    
}



/**
 * \brief 在任务链表指定位置后面插入结点
 */
void rtos_task_list_add (rtos_task_list_t *p_rtos_task_list, dlist_node_t *p_pos, dlist_node_t *p_node)
{
    /* 初始化结点 */
    dlist_init(p_node);
    
    dlist_add(&p_rtos_task_list->head_node, p_pos, p_node); 

    p_rtos_task_list->count++;    
    
}



/**
 * \brief 在任务链表中删除第1个用户结点
 */
dlist_node_t * rtos_task_list_remove_first (rtos_task_list_t * p_rtos_task_list)
{
    
    int ret = 0;
    
    dlist_head_t *p_node =  p_rtos_task_list->head_node.p_next;
  
    /* 删除结点 */    
    ret = dlist_del(&p_rtos_task_list->head_node, p_rtos_task_list->head_node.p_next);
    
    if (ret == RTOS_OK) {
        
        p_rtos_task_list->count--;
        
        return p_node;
        
    } else {
        return NULL;
    }
}



/**
 * \brief 在任务链表中删除最后1个用户结点
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
 * \brief 在任务链表删除指定结点
 */
void rtos_task_list_remove (rtos_task_list_t * p_rtos_task_list, dlist_node_t *p_node)
{
    
     dlist_del(&p_rtos_task_list->head_node, p_node);
    
     p_rtos_task_list->count--;
}


 
 
 
 /* end of file */
 
 
 
 


