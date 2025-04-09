#include "cz_sysInfo.h"

static SysInfo_t g_sysInfo;

/// @brief 获取系统信息全局单例
/// @param  void
/// @return 全局单例
SysInfo_t* get_sysInfo_t(void)
{
	return &g_sysInfo;
}
