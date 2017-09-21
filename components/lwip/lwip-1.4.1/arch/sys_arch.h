/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#ifndef __ARCH_SYS_ARCH_H__
#define __ARCH_SYS_ARCH_H__ 
#include <rtos_init.h>
#include "cc.h"
#include "rtos_init.h"

#ifdef SYS_ARCH_GLOBALS
#define SYS_ARCH_EXT
#else
#define SYS_ARCH_EXT extern
#endif

#ifdef LWIP_NO_SUPPORT_OS
u32_t sys_now(void);
#endif


#ifdef LWIP_SUPPORT_OS
 
#define MAX_QUEUES        		10	// 消息邮箱的数量
#define MAX_QUEUE_ENTRIES 		20	// 每个消息邮箱的大小
//LWIP消息邮箱结构体
typedef struct {
    void*       pvQEntries[MAX_QUEUE_ENTRIES];//消息队列 MAX_QUEUE_ENTRIES消息队列中最多消息数
} TQ_DESCR, *PQ_DESCR;


typedef rtos_sem_t *sys_sem_t;    //LWIP使用的信号量
typedef rtos_mutex_sem_t *sys_mutex_t; 	//LWIP使用的互斥信号量
typedef rtos_mbox_t *sys_mbox_t;    //LWIP使用的消息邮箱,其实就是UCOS中的消息队列
typedef int8_t sys_thread_t;     //线程ID,也就是任务优先级



#endif  /* LWIP_SUPPORT_OS */

#endif

/* __SYS_RTXC_H__ */

































