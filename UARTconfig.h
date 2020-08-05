/*
 * Author:Mohamed Khaled Mohamed
 * Code for  running UART0
 * UART0 is normally connected to the PC through the usb connection
 *
 * */

#ifndef UART_CONFIG
#define UART_CONFIG

#include "tm4c123gh6pm.h"
#include "PLL_SYSTICK.h"
//////////////////////* Prototypes *////////////////////////

//UART0
void UART0setup(void);
void UART0Tx(char c);
char UART0Rx(void);
void printWordUART0(char *ptr);

//UART1

void UART1setup(void);
void UART1Tx(char c);
char UART1Rx(void);
void printWordUART1(char *ptr);



//
int size(char *ptr);




///////////////////////* Implementation *//////////////////////////


/////////////////////////////**  UART0  **///////////////////////////


void UART0setup(void)
{
    //Assuming the clock is 80 mhz ,if another value the integer and fraction registers must be recalculated and changed
    SYSCTL_RCGCUART_R|=0x01;  // Enabling clock for UART0
    SYSCTL_RCGCGPIO_R|=0x01;  //Enabling clock for PORTA
    UART0_CTL_R &=~(0x01);    //Disabling the UART0 to be able to setup the UART
    UART0_IBRD_R =520;         //Integer part of baud rate is 104 (required 9600)
    UART0_FBRD_R =54;           //Fraction part of baud rate is 11 (required 9600)
    UART0_CC_R=0;               // Selecting the system clock as UART clock
    UART0_LCRH_R=0x70;         // Configuring the line control regsiter as desired (see the datasheet to know the usage oe each bit)
    UART0_CTL_R |=0x300;       //Enabling Tx and Rx for UART0
    UART0_CTL_R |=0x01;        //Enabling UART again after finishing setup the UART regs
    GPIO_PORTA_DEN_R|=0x03;    // Making digital enable for PA0,PA1
    GPIO_PORTA_AFSEL_R |=0x03;  //Enabling alternate function for PA1,PA0
    GPIO_PORTA_PCTL_R |=0x11;    //Choosing UART function for PA1,PA0
    delayMS(1);

}

void UART0Tx(char c)
{
    while((UART0_FR_R & 0x20) != 0 );
    UART0_DR_R=c;
}


char UART0Rx(void)
{
    while((UART0_FR_R & 0x10) !=0);
    char temp=UART0_DR_R;
    return (temp);
}




void printWordUART0(char *ptr)
{
    int i=0;
    for(i=0;i<size(ptr);i++)
    {
        UART0Tx(ptr[i]);
    }
}


/////////////////////////////**  UART1  **///////////////////////////

//Using PB0,PB1   ofr UART1
void UART1setup(void)
{
    //Assuming the clock is 80 mhz ,if another value the integer and fraction registers must be recalculated and changed
    SYSCTL_RCGCUART_R|=0x02;  // Enabling clock for UART1
    SYSCTL_RCGCGPIO_R|=0x02;  //Enabling clock for PORTB
    UART1_CTL_R &=~(0x01);    //Disabling the UART1 to be able to setup the UART
    UART1_IBRD_R =520;         //Integer part of baud rate is 104 (required 9600)
    UART1_FBRD_R =54;           //Fraction part of baud rate is 11 (required 9600)
    UART1_CC_R=0;               // Selecting the system clock as UART clock
    UART1_LCRH_R=0x70;         // Configuring the line control regsiter as desired (see the datasheet to know the usage oe each bit)
    UART1_CTL_R |=0x300;       //Enabling Tx and Rx for UART1
    UART1_CTL_R |=0x01;        //Enabling UART again after finishing setup the UART regs
    GPIO_PORTB_DEN_R|=0x03;    // Making digital enable for PB0,PB1
    GPIO_PORTB_AFSEL_R |=0x03;  //Enabling alternate function for PB1,PB0
    GPIO_PORTB_PCTL_R |=0x11;    //Choosing UART function for PA1,PA0
    delayMS(1);

}

void UART1Tx(char c)
{
    while((UART1_FR_R & 0x20) != 0 );
    UART1_DR_R=c;
}


char UART1Rx(void)
{
    while((UART1_FR_R & 0x10) !=0);
    char temp=UART1_DR_R;
    return (temp);
}




void printWordUART1(char *ptr)
{
    int i=0;
    for(i=0;i<size(ptr);i++)
    {
        UART1Tx(ptr[i]);
    }
}


////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
//returns the size of a character array using a pointer to the first element of the character array
int size(char *ptr)
{
    //variable used to access the subsequent array elements.
    int offset = 0;
    //variable that counts the number of elements in your array
    int count = 0;

    //While loop that tests whether the end of the array has been reached
    while (*(ptr + offset) != '\0')
    {
        //increment the count variable
        ++count;
        //advance to the next element of the array
        ++offset;
    }
    //return the size of the array
    return count;
}

#endif
