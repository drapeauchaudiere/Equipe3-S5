/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef DEFINITIONS_H
#define	DEFINITIONS_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <stdbool.h>

#define _XTAL_FREQ 64000000
#define FCY (_XTAL_FREQ/4)


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    
    
//    typedef enum e_keyboard_coordinates
//{
//    COORD_Y4 = LATEbits.LATE0,  // PORTE pin 0
//    COORD_Y3 = LATEbits.LATE1,  // PORTE pin 1
//    COORD_Y2 = LATEbits.LATE2,  // PORTE pin 2
//    COORD_Y1 = LATEbits.LATE3,  // PORTE pin 3
//    COORD_X4 = LATEbits.LATE4,  // PORTE pin 4
//    COORD_X3 = LATEbits.LATE5,  // PORTE pin 5
//    COORD_X2 = LATEbits.LATE6,  // PORTE pin 6
//    COORD_X1 = LATEbits.LATE7,  // PORTE pin 7
//    ALL_COORD = LATE
//            
//}KEYBOARD_COORDINATES_E;

typedef enum e_keyboard_keys
{
    KEY_0 = 0x41,       // X2 Y4    0100 0001   
    KEY_1 = 0x88,       // X1 Y1    1000 1000
    KEY_2 = 0x48,       // X2 Y1    0100 1000
    KEY_3 = 0x28,       // X3 Y1    0010 1000
    KEY_4 = 0x84,       // X1 Y2    1000 0100
    KEY_5 = 0x44,       // X2 Y2    0100 0100
    KEY_6 = 0x24,       // X3 Y2    0010 0100
    KEY_7 = 0x82,       // X1 Y3    1000 0010
    KEY_8 = 0x42,       // X2 Y3    0100 0010
    KEY_9 = 0x22,       // X3 Y3    0010 0010
    KEY_star = 0x81,    // X1 Y4    1000 0001
    KEY_hash = 0x21,    // X3 Y4    0010 0001
    KEY_A = 0x18,       // X4 Y1    0001 1000
    KEY_B = 0x14,       // X4 Y2    0001 0100
    KEY_C = 0x12,       // X4 Y3    0001 0010
    KEY_D = 0x11,       // X4 Y4   0001 0001
    NO_KEY = 0x0
            
}KEYBOARD_KEYS_E;




#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* DEFINITIONS_H */

