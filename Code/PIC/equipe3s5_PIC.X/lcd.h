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
#include "spi.h"

void LCD_init(SPI_PERIPHERAL_S *peripheral);             // Fonction pour initialiser l'ecran.
void LCD_clr(void);              // Fonction pour effacer l'ecran.
void LCD_place_cursor_C0L1(unsigned char Col, unsigned char Lig);// Fct pour positionner le curseur.
void LCD_write_char_hex(unsigned char Car);  // Fonction pour afficher en hexadecimal.
void LCD_write_string(const char * string);// Afficher une chaine de caracteres
void LCD_write_menu(char * menu);   // Fonction pour afficher un menu.
void LCD_write_char(unsigned char Car);       // Fonction pour afficher un caractere.
void LCD_write_car_hex (unsigned char byte);
void LCD_write_data (unsigned char rs, unsigned char data);
void LCD_busy (void);
unsigned char LCD_read_busy (void);
unsigned char LCD_flip_data (unsigned char input);
unsigned char LCD_inc_pos_curseur (void);
unsigned char LCD_dec_pos_curseur (void);
char * LCD_select_menu (void);
char * LCD_get_menu (void);
unsigned char LCD_get_cursor (void);
void LCD_move_cursors()

char text[17] = {'a'};

/*************************************************
 ***************   Definitions   *****************
 ************************************************/





// Menu constants
char menu_main[4][20] = 
{
    "Volume        ",0xFF,"     ",   //i_menu = 1;
    "Bass          ",0xFF,"     ",
    "Mid           ",0xFF,"     ",                     
    "Treble        ",0xFF,"     "
};


#endif