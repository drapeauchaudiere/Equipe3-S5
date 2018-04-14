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
    "Volume    5         ",   //i_menu = 1;
    "Bass      5         ",
    "Mid       5         ",                     
    "Treble    5         "
};

//********************unsigned char LCD_inc_pos_curseur (void)****************//
//Description : Function increments cursor and return its new value
//Function prototype : unsigned char LCD_inc_pos_curseur (void)
//
//Enter params       : None
//
//Exit params        : unsigned char : new incremented cursor position
//
//Function call      : cursor = LCD_inc_pos_curseur()
//
//Philippe Gobeil    MPLab X v3.45    27-02-2018 
// Changelog : 
//****************************************************************************//
unsigned char LCD_inc_pos_curseur (void)
{
    if ((i_menu==1) || (i_menu==2) || (i_menu==8) || (i_menu==12))      //les menus "menu" vont de ligne 1 à 3
    {
        if (++pos_cur > 3){
            pos_cur = 1;}
    }

    if ((i_menu==4) || (i_menu==10) || (i_menu==14))      //les menus "reg" vont de 3 à 4
    {
        if (++pos_cur > 4){
            pos_cur = 3;}
    }
            
    if ((i_menu==5) || (i_menu==6) || (i_menu==7) || (i_menu==11) || (i_menu==15))     //les menus "modif" vont de 2 à 4 
    {            
        if (++pos_cur > 4){
            pos_cur = 2;}
    }
    return pos_cur;
}

//********************unsigned char LCD_dec_pos_curseur (void)****************//
//Description : Function decrements cursor and return its new value
//Function prototype : unsigned char LCD_dec_pos_curseur (void)
//
//Enter params       : None
//
//Exit params        : unsigned char : new decremented cursor position
//
//Function call      : cursor = LCD_dec_pos_curseur()
//
//Philippe Gobeil    MPLab X v3.45    27-02-2018 
// Changelog : 
//****************************************************************************//
unsigned char LCD_dec_pos_curseur (void)
{
    if ((i_menu==1) || (i_menu==2) || (i_menu==8) || (i_menu==12))      //les menus "menu" vont de ligne 1 à 3
    {
        if (--pos_cur < 1){
            pos_cur = 3;} 
    }
        
    if ((i_menu==4) || (i_menu==10) || (i_menu==14))      //les menus "reg" vont de 3 à 4
    {
        if (--pos_cur < 3){
            pos_cur = 4;}
    }
            
    if ((i_menu==5) || (i_menu==6) || (i_menu==7) || (i_menu==11) || (i_menu==15))     //les menus "modif" vont de 2 à 4 
    {            
        if (--pos_cur < 2){
            pos_cur = 4;}
    }   
    
    return pos_cur;
}

//******************unsigned char LCD_get_curseur (void)**********************//
//Description : Function returns actual cursor position (0-19)
//Function prototype : unsigned char LCD_get_curseur (void)
//
//Enter params       : None
//
//Exit params        : unsigned char : actual cursor position
//
//Function call      : cursor = LCD_get_curseur()
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog : 
//****************************************************************************//
unsigned char LCD_get_curseur (void)
{
    return pos_cur;
}

//**************************char * LCD_get_menu (void)************************//
//Description : Function returns pointer to actual menu
//Function prototype : char * LCD_get_menu (void)
//
//Enter params       : None
//
//Exit params        : char * : pointer to menu
//
//Function call      : menu_actuel = LCD_get_menu()
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog : 
//****************************************************************************//
char * LCD_get_menu (void)
{
    return menu_present;
}

//***********************char * LCD_select_menu (void)************************//
//Description : Function switch menu and return pointer to actual menu
//Function prototype : char * LCD_select_menu (void)
//
//Enter params       : None
//
//Exit params        : char * : pointer to menu
//
//Function call      : menu_actuel = LCD_select_menu()
//
//Philippe Gobeil    MPLab X v3.45    27-02-2018 
// Changelog :  Jean-Francois Bilodeau, 21-03-2018
//              Modification de la fonction pour retourner un pointeur sur le 
//              menu actuel
//****************************************************************************//
//char * LCD_select_menu (void)
//{
//    switch (i_menu)
//    {
//        case 1 : 
//            switch (pos_cur)
//            {
//                case 1 :
//                    menu_present = menu_lumiere;
//                    i_menu = 2;
//                break;
//                
//                case 2 : 
//                    pos_cur = 1;
//                    menu_present = menu_sediments;
//                    i_menu = 8;
//                break;
//                
//                case 3 : 
//                    pos_cur = 1;
//                    menu_present = menu_acidite;
//                    i_menu = 12;
//                break;
//            }
//        break;
//
//    return menu_present;
//}

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
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10); // 40mS delay for VDD POR
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10); // 40mS delay for VDD POR
    __delay_ms(10);
    __delay_ms(10);
    __delay_ms(10);
    
    LCD_write_data(0, 0x38); // Boot : 8bit interface 
    __delay_ms(10);
    LCD_write_data(0, 0x38); // Boot : 8bit interface
    __delay_ms(10);  
    LCD_write_data(0, 0x38); // Boot : 8bit interface
    __delay_ms(10); 
    LCD_write_data(0, 0x28); // Boot : 4bit interface
    __delay_ms(10);    
    LCD_write_data(0, 0x10); // Cursor/display shift : Cursor shift left
    __delay_ms(10);      
    LCD_write_data(0, 0x06); // Entry mode : Increment, no display shift
    __delay_ms(10);     
    LCD_write_data(0, 0x0C); // Display on/off : Display on, cursor on, blink off 
    __delay_ms(10);
    LCD_write_data(0, 0x01); // Clear display
    __delay_ms(10);             
    LCD_write_data(0, 0x02); // return home
    __delay_ms(10); 
    
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

//*******************************void LCD_clr(void)***************************//
//Description : Function clears screen
//Function prototype : void LCD_clr(void)
//
//Enter params       : None
//
//Exit params        : None
//
//Function call      : LCD_clr();
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog :
//****************************************************************************//
void LCD_clr(void)
{
    LCD_write_data(0, 0x01); // Clear screen
}

//******void LCD_place_cursor_C0L1(unsigned char Col, unsigned char Lig)******//
//Description : Function places cursor on screen
//Function prototype : void LCD_place_cursor_C0L1(unsigned char Col, unsigned char Lig)
//
//Enter params       : unsigned char col : column (0-19)
//                     unsigned char lig : line (1-4)
//
//Exit params        : None
//
//Function call      : LCD_place_cursor_C0L1(0, 1);
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog :
//****************************************************************************//
void LCD_place_cursor_C0L1(unsigned char Col, unsigned char Lig)
{
    unsigned char ucPosition = 0; // Pour la postion finale
    
    if (Col <= 19)
    {
        switch (Lig)
        {
            case 1:
                ucPosition = Col;
                break;
            case 2:
                ucPosition = 0x14 + Col;
                break;
            case 3:
                ucPosition = 0x40 + Col;
                break;
            case 4:
                ucPosition = 0x54 + Col;
                break;
        }
    }
    LCD_write_data(0, ucPosition | 0x80);
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


//*******************void LCD_aff_string(const char *string)******************//
//Description : Function write string to screen (max length of 20)
//
//Function prototype : void LCD_aff_string(const char *string)
//
//Enter params       : const char *s : string of ascii characters
//
//Exit params        : None
//
//Function call      : LCD_aff_menu(menu_actuel);
//
//Jean-Francois Bilodeau    MPLab X v3.45    27-02-2018 
// Changelog :
//****************************************************************************//
void LCD_write_string(const char *string)
{
    while(*string != 0)             // Tant qu'on a pas atteint la fin de 
    {                               // la chaine.
        LCD_write_char(*string);      // Affiche le caractere.
        string++;                   // Passe au caractere suivant.
    }
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
        LCD_place_cursor_C0L1(0,uci);   //Place curseur col0, lign increm dans for
        for (ucj=0; ucj<20; ucj++)      //Faire 20 fois
        {
            LCD_write_char(*menu);        //Affiche le caractere pointé dans chaine
            menu++;                     //increm pointeur dans chaine            
        }                               //Add 1, passe a ligne2 ainsi de suite
    }
}

//*********************void LCD_move_cursors(EFFECT_CONFIG_S *config)*************************//
//Description : Moves the cursor pointers to the appropriate address
//
//Function prototype : void LCD_move_cursor(EFFECT_CONFIG_S *config)
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
void LCD_update_menu(EFFECT_CONFIG_U *config)
{
    
}