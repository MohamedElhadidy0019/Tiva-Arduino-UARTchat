#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL_SYSTICK.h"
#include "UARTconfig.h"

void setup(void);
void loop(void);
void Ask(void);
void Reply(void);

void QuestionMode(int);
void QuestionSend(int);

void ReplyMode(int);
void ReplySend(int);


int isEqualString(char*,char*);
int arr_size(char *ptr);



int main(void)
{
	setup();
	while(1)
	{
	    loop();
	}
}

void loop(void)
{
    if(UART1Rx()==0)     //Then this is Asking mode
    {
        Ask();   //and wait for reply from the Arduino
    }
    else
    {
        Reply();     //reply after receiving question from arduino
    }


}

void setup(void)
{
    setupPLL();
    setupSystick();
    UART0setup();
    UART1setup();
    SYSCTL_RCGCGPIO_R|=0x30;   //Enabling clocks for PORTF, PORTE

    //Configuring PE4,PE5 as switches (will use pull down circuit on the bread board)
    GPIO_PORTE_DIR_R&=~(0x30);        //making the pins input
    GPIO_PORTE_DEN_R|=0x30;           //making the pins digital

    //Configuring PF1,PF2,PF3 as RGB leds which is built in tiva c
    GPIO_PORTF_DIR_R |= 0x0E;         //making the pins output by setting them high
    GPIO_PORTF_DEN_R |= 0x0E;         // making the pins digital

}

void Ask(void)
{
    static unsigned char theQuestion=0;
    unsigned char Switch_State=(GPIO_PORTE_DATA_R&0x10)>>4;
    unsigned char Switch_Send= (GPIO_PORTE_DATA_R&0x20)>>5;
    delayMS(2);
    //unsigned char last_state=theQuestion;
    if(Switch_State)  //the switch went high
    {
        if(theQuestion==3)   //as we have only 3 questions
        {
            theQuestion=1;
            QuestionMode(theQuestion);   //Changing the Question
        }
        else
        {
            theQuestion++;
            QuestionMode(theQuestion);
        }
        Switch_State=(GPIO_PORTE_DATA_R&0x10)>>4;
        delayMS(2);
        //loop to wait for the switch to go low again so the qeutions wont change again if the button is long pressed
        while(Switch_State){Switch_State=(GPIO_PORTE_DATA_R&0x10)>>4; delayMS(2);}
    }

    if(Switch_Send)
    {
        char received=0;

        Switch_Send= (GPIO_PORTE_DATA_R&0x20)>>5;
        delayMS(2);
        while(Switch_Send){Switch_Send= (GPIO_PORTE_DATA_R&0x20)>>5; delayMS(2);}
        QuestionSend(theQuestion);
        while(1)
        {
            received=UART1Rx();
            if(received==0);
            else
            {
                UART0Tx(received);
            }
            if(received=='\n')
            {
                break;
            }
        }

    }




}

void Reply(void)
{

 while(1)
 {
     char out =UART1Rx(); //out character stores the receiveed char <it will be zero if no character in fifo buffer
     if(out!=0)
     {
       UART0Tx(out);  //print it on the serial monitor
     }
     if(out=='\n')  // if out has the end line character this means no chars will be received again (Each message sent or received should be terminated with a new line as a convention in this code)
     {
         break;
     }
 }
     unsigned char theAnswer=0;

     while(1)
     {
     unsigned char Switch_State=(GPIO_PORTE_DATA_R&0x10)>>4;
     unsigned char Switch_Send= (GPIO_PORTE_DATA_R&0x20)>>5;
     delayMS(2);
     if(Switch_State)  //the switch went high
         {
             if(theAnswer==3)   //as we have only 3 questions
             {
                 theAnswer=1;
                 ReplyMode(theAnswer);   //Changing the Answer
             }
             else
             {
                 theAnswer++;
                 ReplyMode(theAnswer);   //Changing the Answer
             }
             Switch_State=(GPIO_PORTE_DATA_R&0x10)>>4;
             delayMS(2);
             //loop to wait for the switch to go low again so the qeutions wont change again if the button is long pressed
             while(Switch_State){Switch_State=(GPIO_PORTE_DATA_R&0x10)>>4; delayMS(2);}
         }
     if(Switch_Send)
     {
         Switch_Send= (GPIO_PORTE_DATA_R&0x20)>>5;
         while(Switch_Send){Switch_Send= (GPIO_PORTE_DATA_R&0x20)>>5; delayMS(2);}
         ReplySend(theAnswer); //Sends the Answer to the connected controller
         break;
     }
     }



}



void QuestionMode(int x)
{
    GPIO_PORTF_DATA_R&=~(0x0E);       //Erasing any value on led
    switch(x)
    {
    case 1:
        GPIO_PORTF_DATA_R|=0x0A;
        printWordUART0("Tiva send:Are you hungry? \n");

        break;
    case 2:
        GPIO_PORTF_DATA_R|=0x06;
        printWordUART0("Tiva send:Are you thirsty? \n");
        break;
    case 3:
        GPIO_PORTF_DATA_R|=0x0E;
        printWordUART0("Tiva send:Are you happy? \n");
        break;


    }


}

void ReplyMode(int x)
{


    GPIO_PORTF_DATA_R&=~(0x0E);       //Erasing any value on led
        switch(x)
        {
        case 1:
            GPIO_PORTF_DATA_R|=0x0A;
            printWordUART0("Tiva send:Yes \n");

            break;
        case 2:
            GPIO_PORTF_DATA_R|=0x06;
            printWordUART0("Tiva send:No \n");
            break;
        case 3:
            GPIO_PORTF_DATA_R|=0x0E;
            printWordUART0("Tiva send:Maybe \n");
            break;


        }



}

void ReplySend(int x)
{

    switch(x)
            {
            case 1:

                printWordUART1("Yes\n");
                //printWordUART0("Arduino,Are you hungry? \n");

                break;
            case 2:

                printWordUART1("No\n");
                //printWordUART0("Arduino,Are you thirsty? \n");
                break;
            case 3:

                printWordUART1("Maybe\n");
               // printWordUART0("Arduino,Are you happy? \n");
                break;


            }



}


void QuestionSend(int x)
{

    switch(x)
        {
        case 1:

            printWordUART1("Hey,Are you hungry? \n");
            //printWordUART0("Arduino,Are you hungry? \n");

            break;
        case 2:

            printWordUART1("Hey,Are you thirsty? \n");
            //printWordUART0("Arduino,Are you thirsty? \n");
            break;
        case 3:

            printWordUART1("Hey,Are you happy? \n");
           // printWordUART0("Arduino,Are you happy? \n");
            break;


        }


}



int isEqualString(char* arr1,char* arr2)
{
  if(arr_size(arr1)!= arr_size(arr2))
    return 0;
int i=0;
  while(arr1[i]!='\0')
  {
    if(arr1[i] != arr2[i])
      return 0;
    i++;
  }
  return 1;

}



int arr_size(char *ptr)
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



