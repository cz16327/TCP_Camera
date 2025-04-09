#ifndef __CZ_LOG_H__
#define __CZ_LOG_H__

#include "main.h"

typedef enum LOG_Level_def
{
	LOG_LEVEL_NO_LOG = 0,	// 不打印
	LOG_LEVEL_ERR,			// 只打印 ERR
	LOG_LEVEL_WARN,			// 打印 ERR 和 WARN
	LOG_LEVEL_LOG,			// 打印 ERR WARN INFO
	LOG_LEVEL_DBG,			// 打印所有
} LOG_Level_e;


#define CZ_RAW(M, ...)  \
	do {\
		printf(M, ##__VA_ARGS__);\
	} while (0)

#define CZ_ERR(M, ...)  \
	do {\
		if (cz_log_get_level() >= LOG_LEVEL_ERR)\
			printf("<ERR %s: %s %d>: " M, cz_get_time_string(), __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} while (0)

#define CZ_WARN(M, ...)  \
	do {\
		if (cz_log_get_level() >= LOG_LEVEL_WARN)\
			printf("<WARN %s: %s %d>: " M, cz_get_time_string(), __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} while (0)

#define CZ_LOG(M, ...)  \
	do {\
		if (cz_log_get_level() >= LOG_LEVEL_LOG)\
			printf("<LOG %s: %s %d>: " M, cz_get_time_string(), __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} while (0)

#define CZ_DBG(M, ...)  \
	do {\
		if (cz_log_get_level() >= LOG_LEVEL_DBG)\
			printf("<DBG %s: %s %d>: " M, cz_get_time_string(), __FUNCTION__, __LINE__, ##__VA_ARGS__);\
	} while (0)


LOG_Level_e cz_log_get_level(void);
void cz_log_set_level(LOG_Level_e level);
unsigned char* cz_get_time_string(void);

#endif
