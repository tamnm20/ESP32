#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "tgpio.h"


 #define NUM_TIMERS 2
TimerHandle_t xTimers[ NUM_TIMERS ];

void vTask1( void * pvParameters )
{
    for( ;; )
    {
       printf("Task 1\n");
       vTaskDelay(1000/portTICK_PERIOD_MS);
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


void app_main(void)
{
   pinMode(GPIO_NUM_2, GPIO_MODE_INPUT_OUTPUT);
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
    xTimerStart(xTimers[0], 0);
    xTimerStart(xTimers[1], 0);
    xTaskCreate(
        vTask1,       /* Function that implements the task. */
        "vTask1",          /* Text name for the task. */
        1024,      /* Stack size in words, not bytes. */
        NULL,    /* Parameter passed into the task. */
        4,/* Priority at which the task is created. */
        NULL );      /* Used to pass out the created task's handle. */
    
}
