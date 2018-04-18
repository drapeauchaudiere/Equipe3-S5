#include "uart.h"

UART_PERIPHERAL_S UART[1];

void UART_init(void)
{
    TRISDbits.TRISD6 = 0;   // TX : Output
    TRISDbits.TRISD7 = 1;   // Rx : Input
    
    //RPINR0_1<7:4> = 0x06; RP26 -> U1TX
    RPOR26_27bits.RPO26R = 0x01;
    
    //RPINR0_1<3:0> = 0x06; RP27 -> U1RX
    RPINR0_1 |= 0x06; 
    RPINR0_1 &= 0xF6;
    
    //Interuption UART
    PIE1bits.TX1IE = 0;
    IPR1bits.RC1IP = 0; //low priority
    IPR1bits.TX1IP = 0;
    PIE1bits.RC1IE = 1;  //enable uart rx interrupt
        
    UART_config();
}

void UART_config(void)
{    
    TXSTA1 = 0b00100110;//0x26;
    RCSTA1 = 0b10010000;//0x96;
    BAUDCON1bits.BRG16 = 1;//BAUDCON = 0b01001000;
    SPBRG1 = 138 ;//baudrate = 115.2kbaud fosc = 64mhz    
}

void UART_write(unsigned char data)
{
    while (!TXSTA1bits.TRMT);
    TXREG1 = data;
}

uint8_t UART_read(void)
{
    UART_errorCheck();
    uint8_t value = RCREG1;
    return value;
}

bool UART_errorCheck(void)
{
    if(RCSTA1bits.FERR == 1)
    {
        char dump;
        dump = RCREG;
        return 1;
    }
    else if(RCSTA1bits.OERR == 1)
    {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
        return 1;
    }
    return 0;
}

UART_PERIPHERAL_S *UART_getPeripheral(UART_INDEX_E index)
{
    return &UART[index];
}