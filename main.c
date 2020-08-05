#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "PLL_SYSTICK.h"
#include "UARTconfig.h"

void setup(void);
void loop(void);

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
    printWordUART0("PC working\n");
    printWordUART1("Arduino working working\n");
    delayMS(1000);
}

void setup(void)
{
    setupPLL();
    setupSystick();
    UART0setup();
    UART1setup();
}


