#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "tgpio.h"
#include "exti.h"

#define LED 2

void input_event_callback(int pin){
    if(pin == GPIO_NUM_0){
        gpio_toggle(LED);
    }
}

void app_main(void)
{
    pinMode(LED, GPIO_MODE_INPUT_OUTPUT);
    EXTI_init(GPIO_NUM_0, GPIO_MODE_INPUT, GPIO_INTR_NEGEDGE);
    input_set_callback(input_event_callback);

    // while(1){
    //     gpio_toggle(LED);
    //     vTaskDelay(1000/ portTICK_PERIOD_MS);
    //     int current_state = gpio_get_level(LED);
    //     printf("Current: %d\n", current_state);
    // }
}