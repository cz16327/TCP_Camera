#ifndef DELAY_H
#define DELAY_H

#include "main.h"

// 时基中断累计时间
typedef struct TimeBaseIrq_Ms_def
{
	volatile unsigned int spi_timeout;	// SPI传输超时时间
} TimeBaseIrq_Ms_t;


TimeBaseIrq_Ms_t* get_TimeBaseIrq_Ms_t(void);
void delay_init(void);
void delay_us(unsigned int nus);
void delay_ms(unsigned int nms);
void timebase_irq_callback(void);

#endif
