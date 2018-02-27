/*
 * timers.h
 *
 *  Created on: Jan 30, 2018
 *      Author: simon
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include "definitions.h"

// Definitions

#define ADC_TIMER_VALUE_US  125     // 125µs
#define ADC_TIMER_CONFIG    0x300

#define LED0_TIMER_VALUE_US 100000  // 0.1s
#define LED0_TIMER_CONFIG    0x300

#define US_TO_PRD(us)   ((225*us)/8)   // (225MHz / 4) * (time_us/2)  Macro to find the value for the PRD register from the period value in µs

typedef enum e_timer_reg_addr
{
    TIMER_0_CTL_ADDR = 0x01940000,
    TIMER_1_CTL_ADDR = 0x01980000,
    TIMER_0_PRD_ADDR = 0x01940004,
    TIMER_1_PRD_ADDR = 0x01980004,
    TIMER_0_CNT_ADDR = 0x01940008,
    TIMER_1_CNT_ADDR = 0x01980008

} TIMER_REG_ADDR_E;

typedef enum e_timer_ctl_mask
{
    TIMER_CTL_GO = 0x40,
    TIMER_CTL_HLD = 0x80

} TIMER_CTL_MASK_E;

typedef enum e_timer_index
{
    TIMER0 = 0,
    TIMER1,
    TIMER_TOTAL

} TIMER_INDEX_E;

typedef struct s_timer_reg
{
    uint32_t period;
    uint32_t count;
    uint32_t control;

} TIMER_REG_S;


static const TIMER_REG_S timer0_init_config =
{
     US_TO_PRD(LED0_TIMER_VALUE_US),
     0,
     LED0_TIMER_CONFIG
};

static const TIMER_REG_S timer1_init_config =
{
     US_TO_PRD(ADC_TIMER_VALUE_US),
     0,
     ADC_TIMER_CONFIG
};


// Public functions

void timers_init(void);
void timer_config(TIMER_INDEX_E timerIndex, TIMER_REG_S config);
void timer_start(TIMER_INDEX_E timerIndex);
void timer_stop(TIMER_INDEX_E timerIndex);

// Interrupt Service Routines

void c_adcTimerISR(void);
void c_led0TimerISR(void);



#endif /* INCLUDE_TIMERS_H_ */
