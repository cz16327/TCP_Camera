#include "cz_log.h"
#include "rtc.h"

static volatile LOG_Level_e g_debug_level = LOG_LEVEL_NO_LOG;

/// @brief 获取打印级别
/// @param  void
/// @return 级别
LOG_Level_e cz_log_get_level(void)
{
	return g_debug_level;
}


/// @brief 设置打印级别
/// @param level 级别
void cz_log_set_level(LOG_Level_e level)
{
	g_debug_level = level;
	CZ_RAW("print level: %d\r\n", level);
}


/// @brief 获取时间字符串
/// @param  void
/// @return 时间字符串
unsigned char* cz_get_time_string(void)
{
	static unsigned char t[20];
	unsigned char time[7] = { 0 };

	rtc_time_get(time);
	memset(t, 0, sizeof(t));
	sprintf((char *)t, "%02d%02d-%02d-%02d %02d:%02d:%02d", \
		time[0], time[1], time[2], time[3], time[4], time[5], time[6]);
	return t;
}
