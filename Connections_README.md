# Tiva-Arduino-UARTchat
Making a chatting device between Tiva C and Arduino Uno using UART communication protocol
This code also will work if burn on 2 tivas
The Arduino code will work if burnt on 2 Arduinos 

For Tiva 
PBO is Rx
PB1 is Tx
PE4 is connected to a switch with pull down circuit, used to change the question
PE5 is connected to a switch with pull down circuit, used to send the question 

The three questions are : Are you hungry?, Are you thirsty? , Are you happy?.
The three answers are :   Yes , No , Maybe


Arduino connections can be found at the beginnid of the code as #define
