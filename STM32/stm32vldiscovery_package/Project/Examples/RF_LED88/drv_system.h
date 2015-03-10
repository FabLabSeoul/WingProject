
#pragma once


void systemInit(void);
void delayMicroseconds(uint32_t us);
void delay(uint32_t ms);

uint32_t micros(void);
uint32_t millis(void);

void failureMode(uint8_t mode);

void systemBeep(bool onoff);

