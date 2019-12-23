#define BASIC_NAME bigraph_cur
#define FILE_NAME "bigr.cur"

#define CAT2(X,Y) CATX(X,Y)
#define CAT3(A,B,C) CAT2(CAT2(A,B),C)
#define CATX(X,Y) X ## Y 

#if defined(__MINGW32__) && !defined(__MINGW64__)
#define NAME_PREFIX _
#else
#define NAME_PREFIX
#endif

#if !defined(__MINGW32__) && !defined(__MINGW64__)
#define TYPE_IS_SUPPORTED
#endif

#define RESOURCE_NAME CAT2(NAME_PREFIX,BASIC_NAME)
#define RESOURCE_SIZE CAT3(NAME_PREFIX,BASIC_NAME,_size)

    .section .rodata

    .global RESOURCE_NAME;
    .global RESOURCE_SIZE;

#ifdef TYPE_IS_SUPPORTED
    .type   RESOURCE_NAME,  @object;
    .type   RESOURCE_SIZE,  @object;
#endif

    .balign 32
RESOURCE_NAME:
    .incbin FILE_NAME
    .byte   0
resource_end:

    .balign 4, 0
RESOURCE_SIZE:
    .int    resource_end - RESOURCE_NAME
