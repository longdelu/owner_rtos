#ifndef __C_LIB_H
#define __C_LIB_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stddef.h>    
#include <stdint.h>
#include <stdio.h>   
#include <string.h>  
#include <math.h> 

    
/** \brief ����һЩ���õ��������Ͷ̹ؼ��� */
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  
typedef const int16_t sc16;  
typedef const int8_t sc8;  

typedef volatile int32_t  vs32;
typedef volatile int16_t  vs16;
typedef volatile int8_t   vs8;

typedef volatile const int32_t vsc32;  
typedef volatile const int16_t vsc16; 
typedef volatile const int8_t vsc8;   

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  
typedef const uint16_t uc16;  
typedef const uint8_t uc8; 

typedef volatile uint32_t  vu32;
typedef volatile uint16_t vu16;
typedef volatile uint8_t  vu8;

typedef volatile const uint32_t vuc32;  
typedef volatile const uint16_t vuc16; 
typedef volatile const uint8_t vuc8;

#ifndef LWIP_TYPE

typedef unsigned   char    u8_t;    /* Unsigned 8 bit quantity         */
typedef signed     char    s8_t;    /* Signed    8 bit quantity        */
typedef unsigned   short   u16_t;   /* Unsigned 16 bit quantity        */
typedef signed     short   s16_t;   /* Signed   16 bit quantity        */
typedef unsigned   long    u32_t;   /* Unsigned 32 bit quantity        */

#endif
    


#ifdef __cplusplus
}
#endif

#endif /* __C_LIB_H */

/* end of file */
