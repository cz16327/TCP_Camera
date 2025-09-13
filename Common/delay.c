#include "delay.h"

#define Timebase_Source_is_SysTick	(0)	// 当Timebase Source为SysTick时改为1    当使用FreeRTOS，Timebase Source为其他定时器时改为0

#if	(!Timebase_Source_is_SysTick)
extern TIM_HandleTypeDef	htim1;	// 当使用FreeRTOS，Timebase Source为其他定时器时，修改为对应的定时器
#define Timebase_htim	(htim1)
#define Delay_GetCounter()		__HAL_TIM_GetCounter(&Timebase_htim)
#define Delay_GetAutoreload()	__HAL_TIM_GetAutoreload(&Timebase_htim)
#else
#define Delay_GetCounter()		(SysTick->VAL)
#define Delay_GetAutoreload()	(SysTick->LOAD)
#endif

static unsigned int fac_us = 0;
static unsigned int fac_ms = 0;
static TimeBaseIrq_Ms_t g_timebase_ms = {0};


/// @brief 获取时基中断累计时间句柄
/// @param  void
/// @return 时基中断累计时间句柄
TimeBaseIrq_Ms_t* get_TimeBaseIrq_Ms_t(void)
{
	return &g_timebase_ms;
}


/// @brief 延时函数初始化
/// @param  void
void delay_init(void)
{
#if	(!Timebase_Source_is_SysTick)
	fac_ms = 1000000; // 作为时基的计数器时钟频率在HAL_InitTick()中被设为了1MHz
	fac_us = fac_ms / 1000;
#else
	fac_ms = SystemCoreClock / 1000;
	fac_us = fac_ms / 1000;
#endif
}


/// @brief 微秒级延时
/// @param nus 微秒
void delay_us(unsigned int nus)
{
	unsigned int ticks = 0;
	unsigned int told = 0;
	unsigned int tnow = 0;
	unsigned int tcnt = 0;
	unsigned int reload = 0;
	reload = Delay_GetAutoreload();
	ticks = nus * fac_us;
	told = Delay_GetCounter();
	while (1) {
		tnow = Delay_GetCounter();
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow;
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break;
		}
	}
}


/// @brief 毫秒级延时
/// @param nms 毫秒
void delay_ms(unsigned int nms)
{
	unsigned int ticks = 0;
	unsigned int told = 0;
	unsigned int tnow = 0;
	unsigned int tcnt = 0;
	unsigned int reload = 0;
	reload = Delay_GetAutoreload();
	ticks = nms * fac_ms;
	told = Delay_GetCounter();
	while (1) {
		tnow = Delay_GetCounter();
		if (tnow != told) {
			if (tnow < told)
				tcnt += told - tnow;
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break;
		}
	}
}


/// @brief 时基中断回调函数
/// @param  void
void timebase_irq_callback(void)
{
	g_timebase_ms.spi_timeout++;
}
