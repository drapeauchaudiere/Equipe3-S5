/*
 * main.c
 */

#include "definitions.h"
#include "gpio.h"

uint8_t recordingState = 0;

int main(void) {

    gpio_init();
    comm_intr();
    enableInterrupts();

	while(1)
	{
	    if(!gpio_get(CPLD_DIP0) && !recordingState)
	    {
	        recordingStart();
	    }
	    else if(gpio_get(CPLD_DIP0) && recordingState)
	    {
	        recordingStop();
	    }
	};
}

void recordingStart(void)
{
    recordingState = 1;
    // vide le buffer
    enableInterrupt11();

}

void recordingStop(void)
{
    recordingState = 0;
    disableInterrupt11();
}

interrupt void c_aic23_ISR(void)
{
    static sampleIndex = 0;

    *((uint32_t *) SDRAM_CE0_ADDR + sampleIndex) = input_left_sample();

    if(sampleIndex > output_buffer_offset)
    {
        output_left_sample(*((uint32_t *) SDRAM_CE0_ADDR + (sampleIndex - output_buffer_offset)));
    }
    else
    {
        output_left_sample(*((uint32_t *) SDRAM_CE0_ADDR + (max_record_size - output_buffer_offset + sampleIndex)));
    }

    sampleIndex++;

    if(sampleIndex > max_record_size)
    {
        sampleIndex = 0;
        toggle(CPLD_LED1);
    }

    *((uint32_t *)ICR_ADDR) |= INT11_MASK;  // Clear interrupt flag

    return;
}
