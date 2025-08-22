#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

void vTask1( void * pvParameters )
{
    for( ;; )
    {
       printf("Task 1\n");
       vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask2( void * pvParameters )
{
    for( ;; )
    {
       printf("Task 2\n");
       vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void vTask3( void * pvParameters )
{
    for( ;; )
    {
       printf("Task 3\n");
       vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(
                    vTask1,       /* Function that implements the task. */
                    "vTask1",          /* Text name for the task. */
                    1024,      /* Stack size in words, not bytes. */
                    NULL,    /* Parameter passed into the task. */
                    4,/* Priority at which the task is created. */
                    NULL );      /* Used to pass out the created task's handle. */
    xTaskCreate(vTask2, "vTask2", 1024, NULL, 5, NULL);
    xTaskCreate(vTask3, "vTask3", 1024, NULL, 6, NULL);
}
