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
 * \brief 操作系统链表实现 
 * \internal
 * \par Modification history
 * - 1.00 17-07-10  nwt, first implementation.
 * \endinternal
 */
 
#include "rtos_list.h"
#include "rtos_config.h"

/*****************************************************************************/
 
/** \brief 单向链表实现,带有头结点实现 */


/**
 * \brief 单项链表初始化函数
 */
int slist_init (slist_head_t *p_head)
{
    if (p_head == NULL) {
        
        return -RTOS_EINVAILD;

    }
    
    p_head->p_next = NULL;

    return RTOS_OK;      
}

/**
 * \brief 获取某一结点的前一结点
 */ 
slist_node_t *slist_prev_get(slist_head_t *p_head, slist_node_t *p_pos)
{
    slist_node_t *p_tmp = p_head; 
    
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_pos == NULL) {
        
        return NULL;
        
    } 

    while ((p_tmp != NULL) && (p_tmp->p_next != p_pos)) {
        p_tmp = p_tmp->p_next;

    }

    return p_tmp;    
}


/**
 * \brief 获取某一结点的后一结点
 */ 
slist_node_t *slist_next_get(slist_head_t *p_head, slist_node_t *p_pos)
{
    if (p_pos) {
        return p_pos->p_next; 
    }
    
    return NULL;  
}


/**
 * \brief 获取开始位置，第一个用户结点
 */ 
slist_node_t *slist_begin_get(slist_head_t *p_head)
{
    
    return  slist_next_get(p_head, p_head); 
}


/**
 * \brief 获取尾结点
 */ 
slist_node_t *slist_tail_get (slist_head_t *p_head)
{
    
    /* 返回尾结点 */
    return  slist_prev_get(p_head, NULL);  
}


/**
 * \brief 获取结束位置，尾结点下一结点的位置
 */ 
slist_node_t *slist_end_get (slist_head_t *p_head)
{
    
    return  NULL;  
}

/**
 * \brief 在单项链表将结点添加到指定结点的后面
 */ 
int slist_add(slist_head_t *p_head, slist_node_t *p_pos, slist_node_t *p_node)
{
    /* 结点没有初始化 */
    if (p_head == NULL || p_node == NULL || p_pos == NULL) {
        
        return -RTOS_EINVAILD;
    } 

    /* 新结点中的p_next指向p_pos指向结点的下一个结点 */
    p_node->p_next = p_pos->p_next;

    /* 修改p_pos->p_next的指向，使其指向新结点 */
    p_pos->p_next = p_node; 
    
    
    return RTOS_OK;    
}

#if 0

/**
 * \brief 在单项链表尾部添加一个结点
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tmp = p_head;
    
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* 寻找到最后一个链表结点 */
    while (p_tmp->p_next != NULL) {
        
        p_tmp = p_tmp->p_next;
    }
   
     /* 将结点添加到链表尾部 */ 
    return slist_add(p_head, p_tmp, p_node);            
}

/**
 * \brief 在单项链表尾部添加一个结点
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tmp = NULL
    
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* 寻找到最后一个链表结点 */   
    p_tmp = slist_tail_get(p_head);
  
     /* 将结点添加到链表尾部 */ 
    return slist_add(p_head, p_tmp, p_node);            
}

/**
 * \brief 在单项链表头部添加一个结点
 */ 
int slist_add_head(slist_head_t *p_head, slist_node_t *p_node)
{
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
    }
    
    
      /* 将结点添加到链表头部 */    
    return slist_add(p_head, p_head, p_node);  
              
}

#endif /* 0 */ 





#if 1

/**
 * \brief 在单项链表尾部添加一个结点
 */ 
int slist_add_tail(slist_head_t *p_head, slist_node_t *p_node)
{
    slist_node_t *p_tmp = p_head;
    
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    while (p_tmp->p_next != NULL) {
        
        p_tmp = p_tmp->p_next;
    }
    
    /* 尾结点的p_next指向新结点 */
    p_tmp->p_next = p_node;
    
    /* 新结点的p_next指向NULL */
    p_node->p_next = NULL;
    
    return RTOS_OK;
            
}

/**
 * \brief 在单项链表头部添加一个结点
 */ 
int slist_add_head(slist_head_t *p_head, slist_node_t *p_node)
{
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
    }
    
    /* 加进来的结点指向第一个用户结点 */
    p_node->p_next = p_head->p_next;
    
    
    /* 头结点的指向的第一个用户结点重新调整，使其重新指向新结点 */
    p_head->p_next = p_node; 
    
    
    return RTOS_OK;
               
}

#endif /* 1 */ 


/**
 * \brief 遍历链表
 */  
int slist_foreach (slist_head_t *p_head, slist_node_process_t pfn_node_process, void *p_arg)
{
    slist_node_t *p_tmp = NULL; 
    
    slist_node_t *p_end = NULL; 

    int ret = 0;

    
    /* 头结点实例没有初始化 */
    if (p_head == NULL || pfn_node_process == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
    p_tmp  = slist_begin_get(p_head);
    
    p_end  = slist_end_get(p_head);
    
    
    while(p_tmp != p_end) {
        
        ret = pfn_node_process(p_arg, p_tmp);
        
        if (ret == 0)  {
            return ret; /* 不再遍历 */
        }
        
        p_tmp = slist_next_get(p_head, p_tmp); 
        
    }
    
    
    return -RTOS_ERROR;
       
}

/**
 * \brief 删除结点 
 */  
int slist_del (slist_head_t *p_head, slist_node_t *p_node) 
{
    slist_node_t *p_prev = NULL; 
    
    /* 头结点实例没有初始化,可以防止误删头结点 */
    if (p_head == NULL || p_node == NULL || (p_node == p_head)) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* 获得待删除结点的前一结点 */
    p_prev =  slist_prev_get(p_head, p_node);
    
    if (p_prev) {
        
        /* 使待删除结点的上一个结点的p_next指向待删除结点的下一个结点*/
        p_prev->p_next = p_node->p_next;

        /* 待删除结点的p_next置为空 */
        p_node->p_next = NULL;
        
        return RTOS_OK;
        
    } else {
        return -RTOS_EINVAILD; 
    }    
}



                   


/*****************************************************************************/ 
/** \brief 双向键表实现，带有头结点实现 */
 
/**
 * \brief 双向链表初始化函数
 */
int dlist_init (dlist_head_t *p_head)
{
    if (p_head == NULL) {
        
        return -RTOS_EINVAILD;

    }
    
    p_head->p_next = p_head;
    p_head->p_prev = p_head;

    return RTOS_OK;      
}

/**
 * \brief 获取某一结点的前一结点
 */ 
dlist_node_t *dlist_prev_get(dlist_head_t *p_head, dlist_node_t *p_pos)
{
    dlist_node_t *p_tmp = p_head; 
    
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_pos == NULL) {
        
        return NULL;
        
    } 

    if (p_pos != NULL) {
        
        return p_pos->p_prev;
    }

    return p_tmp;    
}


/**
 * \brief 获取某一结点的后一结点
 */ 
dlist_node_t *dlist_next_get(dlist_head_t *p_head, dlist_node_t *p_pos)
{
    if (p_pos) {
        return p_pos->p_next; 
    }
    
    return NULL;  
}


/**
 * \brief 获取开始位置，第一个用户结点
 */ 
dlist_node_t *dlist_begin_get(dlist_head_t *p_head)
{
    
    if (p_head != NULL) {
        
        /* 返回第一个用户结点 */
        return p_head->p_next;
    }
    
    return NULL;
}


/**
 * \brief 获取尾结点
 */ 
dlist_node_t *dlist_tail_get (dlist_head_t *p_head)
{
    
    if (p_head != NULL) {
        
        /* 返回尾结点 */
        return p_head->p_prev;
    }
    
    return NULL;
        
    
}


/**
 * \brief 获取结束位置，尾结点下一结点的位置
 */ 
dlist_node_t *dlist_end_get (dlist_head_t *p_head)
{
    
    if (p_head != NULL) {
        
        /* 返回尾结点下一结点的位置,即头结点本身 */
        return p_head->p_prev->p_next;
    }
    
    return NULL; 
}

/**
 * \brief 在双向链表将结点添加到指定结点的后面
 */ 
int dlist_add(dlist_head_t *p_head, dlist_node_t *p_pos, dlist_node_t *p_node)
{
    /* 结点没有初始化 */
    if (p_head == NULL || p_node == NULL || p_pos == NULL) {
        
        return -RTOS_EINVAILD;
    } 
    
    /* 新结点中的p_prev指向p_pos这一个前结点 */
    p_node->p_prev = p_pos;

    /* 新结点中的p_next指向p_pos这一个结点的下一个结点 */
    p_node->p_next = p_pos->p_next;
    
    /* 后结点p_pos->p_next的p_prev指向新结点 */
    p_pos->p_next->p_prev = p_node; 

    /* p_pos这一个前结点的p_next指向新结点 */
    p_pos->p_next = p_node; 
    
    
    return RTOS_OK;    
}



/**
 * \brief 在双向链表尾部添加一个结点
 */ 
int dlist_add_tail(dlist_head_t *p_head, dlist_node_t *p_node)
{
    
    /* 头结点实例没有初始化 */
    /* (fix)bug 第4个任务的值进到这里的时候，delaylist延时列表的值突然间被改变了 */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    
     /* 将结点添加到链表尾部 */ 
    return dlist_add(p_head, p_head->p_prev, p_node);            
}


/**
 * \brief 在双向链表头部添加一个结点
 */ 
int dlist_add_head(dlist_head_t *p_head, dlist_node_t *p_node)
{
    /* 头结点实例没有初始化 */
    if (p_head == NULL || p_node == NULL) {
        
        return -RTOS_EINVAILD;
    }
    
    
      /* 将结点添加到链表头部 */    
    return dlist_add(p_head, p_head, p_node);  
              
}


/**
 * \brief 遍历链表
 */  
int dlist_foreach (dlist_head_t *p_head, dlist_node_process_t pfn_node_process, void *p_arg)
{
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 

    int ret = 0;

    
    /* 头结点实例没有初始化 */
    if (p_head == NULL || pfn_node_process == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    /* 获得第一个用户结点 */
    p_tmp  = dlist_begin_get(p_head);
    
    /* 结束位置为头结点本身 */
    p_end  = dlist_end_get(p_head);
    
    
    while(p_tmp != p_end) {
        
        ret = pfn_node_process(p_arg, p_tmp);
        
        if (ret == 0)  {
            return ret; /* 不再遍历 */
        }
        
        p_tmp = dlist_next_get(p_head, p_tmp); 
        
    }
    
    
    return -RTOS_ERROR;
       
}

/**
 * \brief 删除结点 
 */  
int dlist_del (dlist_head_t *p_head, dlist_node_t *p_node) 
{
    
    /* 头结点实例没有初始化，防止误删除头结点 */
    if (p_head == NULL || p_node == NULL || (p_head == p_node)) {
        
        return -RTOS_EINVAILD;
        
    }
    
    /* 前结点p_node->p_prev的p_next修改指向后结点 */
    p_node->p_prev->p_next =  p_node->p_next;
    
    /* 后结点p_node->p_next的p_prevt修改指向前结点 */
    p_node->p_next->p_prev =  p_node->p_prev;   

    /* 使结点指向自身 */
    p_node->p_next = p_node;
    p_node->p_prev = p_node;

    return RTOS_OK;    
}

/**
 * \brief 销毁链表
 */  
int dlist_destroy (dlist_head_t *p_head)
{
    dlist_node_t *p_tmp = NULL; 
    
    dlist_node_t *p_end = NULL; 
    
    dlist_node_t *p_next = NULL; 

    
    /* 头结点实例没有初始化 */
    if (p_head == NULL) {
        
        return -RTOS_EINVAILD;
        
    }
    /* 获得第一个用户结点 */
    p_next  = dlist_begin_get(p_head);
    
    /* 结束位置为头结点本身 */
    p_end  = dlist_end_get(p_head);
    
  
    while(p_next != p_end) {   

        /* 销毁前先纪录下当前结点 */
        p_tmp =  p_next;
    
        
        /* 销毁前先记录下一个结点的信息 */
        p_next = dlist_next_get(p_head, p_next);

        /* 重置结点自己的信息,指向结点自身 */
        p_tmp->p_next = p_tmp;            
        p_tmp->p_prev = p_tmp;   
        
              
    }
    
    /*  重置头结点自己的信息,指向头结点自身 */
    p_next->p_next =  p_next;
    p_next->p_prev =  p_next;
    
    return RTOS_OK;
       
}

 
/* end of file */


