#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "tgpio.h"
#include "exti.h"
#include "tuart.h"
#include "ttimer.h"

#define LED 2

void input_event_callback(int pin){
    if(pin == GPIO_NUM_0){
        gpio_toggle(LED);

        //uart_put((uint8_t*)"Hello Tam.\n", strlen("Hello Tam.\n"));
    }
}

void tam_uart_callback(uint8_t *data, uint16_t length){
    //printf("%s", data);
    uart_put(data, length);
}

void app_main(void)
{
    pinMode(LED, GPIO_MODE_INPUT_OUTPUT);
    EXTI_init(GPIO_NUM_0, GPIO_MODE_INPUT, GPIO_INTR_NEGEDGE);
    input_set_callback(input_event_callback);

    uart_init();
    uart_set_callback(tam_uart_callback);

    timer_init();

    // while(1){
    //     gpio_toggle(LED);
    //     vTaskDelay(1000/ portTICK_PERIOD_MS);
    //     int current_state = gpio_get_level(LED);
    //     printf("Current: %d\n", current_state);
    // }
}