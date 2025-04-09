#ifndef __PUBLIC_H
#define __PUBLIC_H

#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdint.h"
#include "stdlib.h"
#include "ctype.h"

#include "cz_log.h"
#include "cmsis_os2.h"

// 定义内存块大小和内存池中的块数
#define MEMORY_BLOCK_SIZE	512
#define NUM_MEMORY_BLOCKS	5

typedef enum Able_def
{
	CZ_DISABLE,
	CZ_ENABLE
} Able_e;

typedef enum Ret_Status_def
{
	CZ_OK,
	CZ_ERROR,
	CZ_BUSY,
	CZ_TIMEOUT
} Ret_Status_e;

osSemaphoreId_t get_atAck_semap(void);
osMemoryPoolId_t get_memoryPoolId_t(void);

#endif
