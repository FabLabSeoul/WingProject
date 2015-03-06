

#include "stm32f10x.h"



void SerialSetup(void);

void print_byte(short  c);

void print_short(short  c);

void __attribute__((weak)) osa_mdelay(unsigned int msec);

void putc ( void* p, char c);
