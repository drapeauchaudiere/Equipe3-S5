/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef GPIO_H
#define	GPIO_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "definitions.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

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

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* GPIO_H */

