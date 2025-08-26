#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "tgpio.h"
#include "tbutton.h"

#define BIT_PRESS_SHORT  ( 1 << 0 )
#define BIT_PRESS_NORMAL  ( 1 << 1 )
#define BIT_PRESS_LONG  ( 1 << 2 )

#define LED 2

static EventGroupHandle_t xEventGroup;

void button_event_callback(int pin, uint64_t tick){    
    if(pin == GPIO_NUM_0){
        int press_ms = tick*portTICK_PERIOD_MS;
        BaseType_t xHigherPriorityTaskWoken;
        /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
        xHigherPriorityTaskWoken = pdFALSE;
        if(press_ms < 1000){
            //press short
            xEventGroupSetBitsFromISR(
            xEventGroup,   /* The event group being updated. */
            BIT_PRESS_SHORT, /* The bits being set. */
         &xHigherPriorityTaskWoken );
        }
        else if(press_ms < 3000){
            //press normal
            xEventGroupSetBitsFromISR(
            xEventGroup,   /* The event group being updated. */
            BIT_PRESS_NORMAL, /* The bits being set. */
         &xHigherPriorityTaskWoken );
        }
        else if(press_ms > 3000){
            //press long
        //     xEventGroupSetBitsFromISR(
        //     xEventGroup,   /* The event group being updated. */
        //     BIT_PRESS_LONG, /* The bits being set. */
        //  &xHigherPriorityTaskWoken );
        }
    }
}

void button_timeout_callback(int pin){
    if(pin == BUTTON0){
        printf("TIMEOUT\n");
    }
}

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
                     BIT_PRESS_SHORT | BIT_PRESS_NORMAL |BIT_PRESS_LONG, /* The bits within the event group to wait for. */
                     pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
                     pdFALSE,       /* Don't wait for both bits, either bit will do. */
                     portMAX_DELAY  );/* Wait a maximum of 100ms for either bit to be set. */

         if(uxBits & BIT_PRESS_SHORT)
         {
            printf("Press Short\n");
         }
         else if(uxBits & BIT_PRESS_NORMAL)
         {
            printf("Press Normal\n");
         }
         else if(uxBits & BIT_PRESS_LONG)
         {
            printf("Press Long\n");
         }
         else
         {
            printf("Time out\n");
         }
    }
}

void app_main(void)
{
    xEventGroup = xEventGroupCreate();
    pinMode(LED, GPIO_MODE_INPUT_OUTPUT);
    button_init(GPIO_NUM_0, GPIO_MODE_INPUT, GPIO_INTR_ANYEDGE);
    button_set_callback(button_event_callback);
    button_set_timeout_callback(button_timeout_callback);

    xTaskCreate(
        vTask1,       /* Function that implements the task. */
        "vTask1",          /* Text name for the task. */
        1024,      /* Stack size in words, not bytes. */
        NULL,    /* Parameter passed into the task. */
        4,/* Priority at which the task is created. */
        NULL );      /* Used to pass out the created task's handle. */    

}