/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

#ifndef GPIO_H
#define	GPIO_H

#include "definitions.h"

typedef struct s_gpio_io
{
    uint8_t port;
    uint8_t pin;
    bool state;
    bool direction; // Input = 1 - Output = 0

} GPIO_IO_S;

void GPIO_init(void);
void GPIO_set(GPIO_IO_S pin);
uint8_t GPIO_getPort(uint16_t port);


#endif	/* GPIO_H */

