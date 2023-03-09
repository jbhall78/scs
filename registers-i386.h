#ifndef REGISTERS_H
#define REGISTERS_H

#include <stdint.h>

typedef struct {
    union {
	struct r32_s {
	    union {
		int32_t eax;
		uint32_t ueax;
	    };
	    union {
		int32_t edx;
		uint32_t uedx;
	    };
	    union {
		int32_t ecx;
		uint32_t uecx;
	    };
	    union {
		int32_t ebx;
		uint32_t uebx;
	    };
	    union {
		int32_t ebp;
		uint32_t uebp;
	    };
	    union {
		int32_t esi;
		uint32_t uesi;
	    };
	    union {
		int32_t edi;
		uint32_t uedi;
	    };
	    union {
		int32_t esp;
		uint32_t uesp;
	    };
	} r32;
	struct r16_s {
	    union {
		int16_t ax;
		uint16_t uax;
	    };
	    int16_t padding1;
	    union {
		int16_t dx;
		uint16_t udx;
	    };
	    int16_t padding2;
	    union {
		int16_t cx;
		uint16_t ucx;
	    };
	    int16_t padding3;
	    union {
		int16_t bx;
		uint16_t ubx;
	    };
	    int16_t padding4;
	    union {
		int16_t bp;
		uint16_t upx;
	    };
	    int16_t padding5;
	    union {
		int16_t si;
		uint16_t usi;
	    };
	    int16_t padding6;
	    union {
		int16_t di;
		uint16_t udi;
	    };
	    int16_t padding7;
	    union {
		int16_t sp;
		uint16_t usp;
	    };
	    int16_t padding8;
	} r16;
    };
} registers_t;

#define registers_get(r) \
{ \
    int32_t val = 0; \
    memset(r, 0, sizeof(registers_t)); \
    asm volatile( \
	    "movl %%eax, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.eax = val; \
    asm volatile( \
	    "movl %%edx, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.edx = val; \
    asm volatile( \
	    "movl %%ecx, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.ecx = val; \
    asm volatile( \
	    "movl %%ebx, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.ebx = val; \
    asm volatile( \
	    "movl %%ebp, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.ebp = val; \
    asm volatile( \
	    "movl %%esi, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.esi = val; \
    asm volatile( \
	    "movl %%edi, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.edi = val; \
    asm volatile( \
	    "movl %%esp, %0\n" \
	    : "=r" (val) : "g"((long)(val)) \
       ); \
    (r)->r32.esp = val; \
}

#define registers_print(r) \
{ \
    printf("eax=%08x edx=%08x ecx=%08x ebx=%08x\nebp=%08x esi=%08x edi=%08x esp=%08x\n",  \
	    (r)->r32.eax, (r)->r32.edx, (r)->r32.ecx, (r)->r32.ebx, (r)->r32.ebp, (r)->r32.esi, (r)->r32.edi, (r)->r32.esp); \
}

#endif
