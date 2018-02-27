/*
 * timers.c
 *
 *  Created on: Jan 30, 2018
 *      Author: simon
 */


#include "timers.h"
#include "gpio.h"

// Public functions

void timers_init(void)
{
    timer_config(TIMER0,timer0_init_config); // Initialize timer 0
    timer_config(TIMER1,timer1_init_config); // Initialize timer 1
}

void timer_config(TIMER_INDEX_E timerIndex, TIMER_REG_S config)
{
    *((uint32_t *)(TIMER_0_PRD_ADDR + (0x40000*timerIndex))) = config.period;   // Set the period
    *((uint32_t *)(TIMER_0_CNT_ADDR + (0x40000*timerIndex))) = config.count;    // Set the starting count of the timer
    *((uint32_t *)(TIMER_0_CTL_ADDR + (0x40000*timerIndex))) = config.control;  // Set the control configuration
}

void timer_start(TIMER_INDEX_E timerIndex)
{
    *((uint32_t *)ICR_ADDR) |= INT14_MASK+timerIndex;   // Clear the interrupt flag
    *((uint32_t *)(TIMER_0_CTL_ADDR + (0x40000*timerIndex))) |= TIMER_CTL_HLD;  // Disable hold
    *((uint32_t *)(TIMER_0_CTL_ADDR + (0x40000*timerIndex))) |= TIMER_CTL_GO;   // Send go signal
}

void timer_stop(TIMER_INDEX_E timerIndex)
{
    *((uint32_t *)(TIMER_0_CTL_ADDR + (0x40000*timerIndex))) &= ~TIMER_CTL_HLD; // Enable hold bit
    *((uint32_t *)ICR_ADDR) |= INT14_MASK+timerIndex;                           // Clear the interrupt flag
}


interrupt void c_led0Timer_ISR(void)
{
    gpio_toggle(CPLD_LED0);
    *((uint32_t *)ICR_ADDR) |= INT14_MASK;
}




