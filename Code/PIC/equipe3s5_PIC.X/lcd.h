//****************************************************************************//
// File : lcd.h
//
// Functions :  void LCD_init(void);
//              void LCD_clr(void);
//              void LCD_place_cursor_C0L1(unsigned char Col, unsigned char Lig);
//              void LCD_aff_char_hex(unsigned char Car);
//              void LCD_aff_string(const char * string);
//              void LCD_aff_menu(char * menu);
//              void LCD_aff_char(unsigned char Car);
//              void LCD_write_data (unsigned char rs, unsigned char data);
//              void LCD_busy (void);
//              unsigned char LCD_read_busy (void);
//              unsigned char LCD_flip_data (unsigned char input);
//              unsigned char LCD_inc_pos_curseur (void);
//              unsigned char LCD_dec_pos_curseur (void);
//              char * LCD_select_menu (void);
//              char * LCD_get_menu (void);
//              unsigned char LCD_get_curseur (void);
//              void LCD_aff_car_hex (unsigned char byte);
//
// Includes :   definitions.h
//              spi.h
//
//Jean-Francois Bilodeau
//MPLab X v3.45 21/03/2018
//Revision : Simon Milhomme
//MPLAB X v.4.01 21/03/2018
//****************************************************************************//

#ifndef __LCD_H
#define __LCD_H

#include "definitions.h"
#include "effects.h"
#include "spi.h"

char *LCD_init(SPI_PERIPHERAL_S *peripheral);                        // Fonction pour initialiser l'ecran.
void LCD_write_data (unsigned char rs, unsigned char data);
void LCD_write_char(unsigned char Car);                             // Fonction pour afficher un caractere.
void LCD_write_menu(char * menu);                                   // Fonction pour afficher un menu.
unsigned char LCD_flip_data (unsigned char input);

#endif