/*
===============================================================================
Name
: proj_jibin_QueuefreertosBlink.c
Author : $(jibin)
Version :
Copyright : $(copyright)
Description : Queue freeRTOS Blink
===============================================================================
*/
//Defining the headers
#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "math.h"
//Defing the queue
xQueueHandle xQueue;
//Inititalizing the variables
double clkOneSecond = 1000; //Clock rate for one second
float random_Delay
= 0; //Random delay time of senderint xQueue_SIZE = 1000; //Queue size
float randValue
int transmitTime = 0; //Random Value
= 0; //Transmit Clock. Ticks in every one second
int xQvalue_Send
int xS_Status = 0; //Value to send
= 0; //Send Status
int xQvalue_Received
int xR_Status = 0; //Received Value
= 0; //Receiver Status
//Clearing the LEDs
void clearLEDs(void)
{
Board_LED_Set(0, true);
Board_LED_Set(1, true);
Board_LED_Set(2, true);
}
//Seting up the hardware
static void prvSetupHardware(void)
{
SystemCoreClockUpdate();
Board_Init();
//At the start all the leds are off
clearLEDs();
}
//Sender Task defenition
void senderTask( void *pvParameters )
{
while(1)
{
if(transmitTime <= 4)
{
printf("Send : %i \n",xQvalue_Send);
xS_Status = xQueueSend(xQueue,&xQvalue_Send, (portTickType) 100);
if( xS_Status != pdPASS )
{
printf("Cannot Send to the queue\n");
}
xQvalue_Send++;
if (xQvalue_Send>2)
{
xQvalue_Send=0;
}
}
else if(transmitTime > 34)
{
transmitTime = 0;
}
else{
}
}
randValue = (float)((float)rand())/((float)__RAND_MAX);
random_Delay += (randValue * clkOneSecond);
if(random_Delay >= clkOneSecond)
{
transmitTime++;
random_Delay = 0;
}
//Random Delay uniform over 0 to 1 second
vTaskDelay((portTickType)(randValue * clkOneSecond));
}
//Receiver Task Defenition
void receiverTask( void *pvParameters )
{
while(1)
{
xR_Status = xQueueReceive( xQueue, &xQvalue_Received, (portTickType)100 );
if(xR_Status == pdPASS)
{
printf ("\tReceived : %d\n" , xQvalue_Received);
clearLEDs();
Board_LED_Set(xQvalue_Received, false);
//Waiting for 2 Seconds
vTaskDelay((portTickType)(2 * clkOneSecond));
}
}
}
int main(void)
{
prvSetupHardware();
SystemCoreClockUpdate();
Board_Init();
clearLEDs();
xQueue = xQueueCreate(xQueue_SIZE, sizeof(int));
xTaskCreate(senderTask, (signed char *) "senderTask",
configMINIMAL_STACK_SIZE, NULL, 1,(xTaskHandle *) NULL);
xTaskCreate(receiverTask, (signed char *) "receiverTask",
configMINIMAL_STACK_SIZE, NULL, 2,(xTaskHandle *) NULL);
vTaskStartScheduler();
return 1;
}
