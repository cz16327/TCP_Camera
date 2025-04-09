#ifndef __CZ_SYSINFO_H__
#define __CZ_SYSINFO_H__

#include "main.h"

typedef enum Able_def
{
	CZ_DISABLE,
	CZ_ENABLE
} Able_e;

#pragma pack(1)
typedef struct SysInfo_def
{
	float vbat;
} SysInfo_t;
#pragma pack()

SysInfo_t* get_sysInfo_t(void);

#endif
