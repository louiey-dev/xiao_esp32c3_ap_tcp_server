/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/
#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_system.h"


#define PCF8563_READ_ADDR               0xA3
#define PCF8563_WRITE_ADDR              0xA2

#define PCF_ALARM_FLAG                  (1<<3)
#define PCF_TIMER_FLAG                  (1<<2)
#define PCF_ALARM_INTERRUPT_ENABLE      (1<<1)
#define PCF_TIMER_INTERRUPT_ENABLE      (1<<0)

#define PCF_CLKOUT_32768HZ              0b10000000
#define PCF_CLKOUT_1024HZ               0b10000001
#define PCF_CLKOUT_32HZ                 0b10000010
#define PCF_CLKOUT_1HZ                  0b10000011
#define PCF_CLKOUT_DISABLED             0b00000000

#define PCF_TIMER_4096HZ                0b10000000
#define PCF_TIMER_64HZ                  0b10000001
#define PCF_TIMER_1HZ                   0b10000010
#define PCF_TIMER_1_60HZ                0b10000011
#define PCF_TIMER_DISABLED              0b00000011

#define PCF_DISABLE_ALARM               80


typedef struct {
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t weekday;
} PCF_Alarm;

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t weekday;
    uint8_t month;
    uint16_t year;
} PCF_DateTime;


int PCF_Init(uint8_t mode);

int bsp_rtc_pcf8563_init(uint8_t mode);
int bsp_rtc_pcf8563_systohc(void);
int bsp_rtc_pcf8563_hctosys(void);
int bsp_rtc_pcf8563_get_date_time(PCF_DateTime *dateTime);
int bsp_rtc_pcf8563_set_date_time(PCF_DateTime *dateTime);
int bsp_rtc_pcf8563_get_alarm(PCF_Alarm *alarm) ;

int bsp_rtc_pcf8563_set_alarm(PCF_Alarm *alarm);
int bsp_rtc_pcf8563_get_timer(void);
int bsp_rtc_pcf8563_set_timer(uint8_t mode, uint8_t count);
int bsp_rtc_pcf8563_set_clock_out(uint8_t mode);
int bsp_rtc_pcf8563_get_clear_flags(void);

