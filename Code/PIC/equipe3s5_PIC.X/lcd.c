//****************************************************************************//
// File : Ecran.c
//
// Functions :  void LCD_init(void);
//              void LCD_clr(void);
//              void LCD_place_cursor_C0L1(unsigned char Col, unsigned char Lig);
//              void LCD_aff_string(const char * string);
//              void LCD_aff_menu(char * menu);
//              void LCD_aff_char(unsigned char Car);
//              void LCD_busy (void);
//              unsigned char LCD_flip_data (unsigned char input);
//              unsigned char LCD_inc_pos_curseur (void);
//              unsigned char LCD_dec_pos_curseur (void);
//              char * LCD_select_menu (void);
//              char * LCD_get_menu (void);
//              unsigned char LCD_get_curseur (void);
//
// Includes : "Ecran.h"
//
//Jean-Francois Bilodeau
//MPLab X v3.45 21/03/2018
//****************************************************************************//
#include "lcd.h"

char * menu_present;
unsigned char pos_cur = 1;
unsigned char i_menu = 1;
char *pVolume, *pBass, *pMid, *pTreble;     // Pointers for the actual volume/eq cursor positions
SPI_PERIPHERAL_S *lcdSpi;            // Pointer towards the SPI peripheral of the LCD

// Menu constants
char menu_main[4][20] = 
{
    " Volume    5        ",   //i_menu = 1;
    " Bass      5        ",
    " Mid       5        ",                     
    " Treble    5        "
};


//*****************************void LCD_init (void)***************************//
//Description : Function initialise screen to 4x20, 4 bit mode SPI
//Function prototype : void LCD_init(void)
//
//Enter params       : None
//
//Exit params        : None
//
//Function call      : LCD_init();
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog :
//****************************************************************************//
char *LCD_init (SPI_PERIPHERAL_S *peripheral)
{
    lcdSpi = peripheral; // Link the spi peripheral with the LCD functions
              
    __delay_ms(1); // 40mS delay for VDD POR
    __delay_ms(15);
    __delay_ms(15);
    __delay_ms(15);
    __delay_ms(15); // 40mS delay for VDD POR
    __delay_ms(15);
    __delay_ms(15);
    __delay_ms(15);
    __delay_ms(15); // 40mS delay for VDD POR
    __delay_ms(15);
    __delay_ms(15);
    __delay_ms(15);
    
    LCD_write_data(0, 0x38); // Boot : 8bit interface 
    __delay_ms(15);
    LCD_write_data(0, 0x38); // Boot : 8bit interface
    __delay_ms(15);  
    LCD_write_data(0, 0x38); // Boot : 8bit interface
    __delay_ms(15); 
    LCD_write_data(0, 0x28); // Boot : 4bit interface
    __delay_ms(15);    
    LCD_write_data(0, 0x10); // Cursor/display shift : Cursor shift left
    __delay_ms(15);      
    LCD_write_data(0, 0x06); // Entry mode : Increment, no display shift
    __delay_ms(15);     
    LCD_write_data(0, 0x0C); // Display on/off : Display on, cursor on, blink off 
    __delay_ms(15);
    LCD_write_data(0, 0x01); // Clear display
    __delay_ms(15);             
    LCD_write_data(0, 0x02); // return home
    __delay_ms(15); 
    
    return(menu_main);      
} 

//**********void LCD_write_data (unsigned char rs, unsigned char data)********//
//Description : Function writes data to LCD
//
//Function prototype : void LCD_write_data (unsigned char rs, unsigned char data)
//
//Enter params       : rs   : register select (0, 1)
//                     data : 8 bit data
//
//Exit params        : None
//
//Function call      : LCD_write_data(0, 0xAE)
//
//Jean-Francois Bilodeau    MPLab X v3.30    27-02-2018 
// Changelog :
//****************************************************************************//
void LCD_write_data (unsigned char rs, unsigned char data)
{
    unsigned char data_to_send[3] = {0, 0, 0};
    unsigned char data_flipped = 0;
    data_to_send[0] = 0xF8 + (rs*2);            // F8 if RS = 0, FA if RS = 1
    
    data_flipped = LCD_flip_data(data);    // LSB first 
    data_to_send[1] = (unsigned char)(data_flipped&0xF0);
    data_to_send[2] = ((data_flipped&0x0F)<<4);
    
    __delay_us(25);
    lcdSpi->txdata = data_to_send;
    lcdSpi->txsize = 3;
    SPI_write(lcdSpi); // Write data on SPI bus
}                

//***************unsigned char LCD_flip_data (unsigned char input)****************//
//Description : Function flips data from MSB 1st to LSB 1st
//
//Function prototype : unsigned char flip_data (unsigned char input)
//
//Enter params       : input : 8 bit data
//
//Exit params        : unsigned char : Data flipped
//
//Function call      : data = flip_data(0xAE)
//
//Jean-Francois Bilodeau    MPLab X v3.30    27-02-2018 
// Changelog : 
//****************************************************************************//
unsigned char LCD_flip_data (unsigned char input)
{
    //Flips the char so it becomes LSB first
    input = (input & 0xF0) >> 4 | (input & 0x0F) << 4;
    input = (input & 0xCC) >> 2 | (input & 0x33) << 2;
    input = (input & 0xAA) >> 1 | (input & 0x55) << 1;   
    return input;
}


//******************void LCD_aff_char (unsigned char Car)*********************//
//Description : Function writes a byte to screen
//
//Function prototype : void LCD_aff_char (unsigned char Car)
//
//Enter params       : unsigned char Car : byte to write
//
//Exit params        : None
//
//Function call      : LCD_aff_char('A');
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog :
//****************************************************************************//
void LCD_write_char (unsigned char Car)
{
    if (Car != 0x0D) 
    {
        LCD_write_data(1, Car);  //Write char to screen
    } 
    else {LCD_write_data(1, 0xFF);}             //write err to screen
}


//*********************void LCD_aff_menu(char * menu)*************************//
//Description : Function writes whole menu (4x20) on LCD
//
//Function prototype : void LCD_aff_menu(char * menu)
//
//Enter params       : char * menu : Pointer to menu
//
//Exit params        : None
//
//Function call      : LCD_aff_menu(menu_actuel);
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog :
//****************************************************************************//
void LCD_write_menu(char * menu)
{
    unsigned char uci = 0;
    unsigned char ucj = 0;
    
    for (uci=1; uci<5; uci++)           //Faire 4 fois
    {        
        //LCD_place_cursor_C0L1(0,uci);   //Place curseur col0, lign increm dans for
        for (ucj=0; ucj<20; ucj++)      //Faire 20 fois
        {
            LCD_write_char(*menu);        //Affiche le caractere pointé dans chaine
            menu++;                     //increm pointeur dans chaine            
        }                               //Add 1, passe a ligne2 ainsi de suite
    }
}

