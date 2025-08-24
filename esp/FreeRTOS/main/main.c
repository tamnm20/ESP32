#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "tgpio.h"
#include "exti.h"
#include "tuart.h"


#define NUM_TIMERS 2
#define BIT_EVEN_BUTTON_PRESS    ( 1 << 0 )
#define BIT_EVENT_UART_RECV    ( 1 << 1 )
TimerHandle_t xTimers[ NUM_TIMERS ];
EventGroupHandle_t xEventGroup;



void vTask1( void * pvParameters )
{
    for( ;; )
    {
      EventBits_t uxBits;
        //const TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS;
         /* Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
            the event group. Clear the bits before exiting. */
         uxBits = xEventGroupWaitBits(
                     xEventGroup,   /* The event group being tested. */
                     BIT_EVEN_BUTTON_PRESS | BIT_EVENT_UART_RECV, /* The bits within the event group to wait for. */
                     pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
                     pdFALSE,       /* Don't wait for both bits, either bit will do. */
                     portMAX_DELAY  );/* Wait a maximum of 100ms for either bit to be set. */

         if( ( uxBits & ( BIT_EVEN_BUTTON_PRESS | BIT_EVENT_UART_RECV ) ) == ( BIT_EVEN_BUTTON_PRESS | BIT_EVENT_UART_RECV ) )
         {
            /* xEventGroupWaitBits() returned because both bits were set. */
         }
         else if(uxBits & BIT_EVEN_BUTTON_PRESS)
         {
            /* xEventGroupWaitBits() returned because just BIT\_0 was set. */
            printf("BUTTON PRESS\n");
            gpio_toggle(GPIO_NUM_2);
         }
         else if(uxBits & BIT_EVENT_UART_RECV)
         {
            /* xEventGroupWaitBits() returned because just BIT\_4 was set. */
            printf("UART RECV\n");
         }
         else
         {
            /* xEventGroupWaitBits() returned because xTicksToWait ticks passed
               without either BIT\_0 or BIT\_4 becoming set. */
               printf("Time out\n");
         }
    }
}

void vTimerCallback( TimerHandle_t xTimer )
{
   uint32_t ulCount;
   /* Optionally do something if the pxTimer parameter is NULL. */
   configASSERT( xTimer );
   ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
   if(ulCount == 0){
      gpio_toggle(GPIO_NUM_2);
   }
   else if(ulCount ==1){
      printf("Hello\n");
   }
}

void button_callback(int pin){
   if(pin == GPIO_NUM_0){
      BaseType_t xHigherPriorityTaskWoken;

      /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
      xHigherPriorityTaskWoken = pdFALSE;
      xEventGroupSetBitsFromISR(
         xEventGroup,   /* The event group being updated. */
         BIT_EVEN_BUTTON_PRESS, /* The bits being set. */
         &xHigherPriorityTaskWoken );
   }
}

void uart_event_callback(void){
   BaseType_t xHigherPriorityTaskWoken;

   /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
   xHigherPriorityTaskWoken = pdFALSE;
   xEventGroupSetBitsFromISR(
   xEventGroup,   /* The event group being updated. */
   BIT_EVENT_UART_RECV, /* The bits being set. */
   &xHigherPriorityTaskWoken );
}

void app_main(void)
{
   uart_init();
   uart_set_callback(uart_event_callback);
   xEventGroup = xEventGroupCreate();
   pinMode(GPIO_NUM_2, GPIO_MODE_INPUT_OUTPUT);
   EXTI_init(GPIO_NUM_0, GPIO_MODE_INPUT, GPIO_INTR_POSEDGE);
   input_set_callback(button_callback);
    xTimers[ 0 ] = xTimerCreate
            ( /* Just a text name, not used by the RTOS kernel. */
                "TimerBlink",
                /* The timer period in ticks, must be greater than 0. */
                pdMS_TO_TICKS(500),
                /* The timers will auto-reload themselves when they expire. */
                pdTRUE,
                /* The ID is used to store a count of the number of times the
                    timer has expired, which is initialised to 0. */
                ( void * ) 0,
                /* Each timer calls the same callback when it expires. */
                vTimerCallback
            );
    xTimers[ 1 ] = xTimerCreate("TimerPrint", pdMS_TO_TICKS(1000), pdTRUE, ( void * ) 1, vTimerCallback);
   //  xTimerStart(xTimers[0], 0);
   // xTimerStart(xTimers[1], 0);
    xTaskCreate(
        vTask1,       /* Function that implements the task. */
        "vTask1",          /* Text name for the task. */
        1024,      /* Stack size in words, not bytes. */
        NULL,    /* Parameter passed into the task. */
        4,/* Priority at which the task is created. */
        NULL );      /* Used to pass out the created task's handle. */    
}
