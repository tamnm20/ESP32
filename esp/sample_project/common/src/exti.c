#include<stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "exti.h"
#include "esp_attr.h" 

input_callback_t input_callback = NULL;

static void IRAM_ATTR gpio_input_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    input_callback(gpio_num);
}

void EXTI_init(gpio_num_t pinNum, gpio_mode_t pin_Mode, gpio_int_type_t type){
    esp_rom_gpio_pad_select_gpio(pinNum);
    gpio_set_direction(pinNum, pin_Mode);
    gpio_set_pull_mode(pinNum, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(pinNum, type);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pinNum, gpio_input_handler, (void*) pinNum);
}

void input_set_callback(void *cb)
{
    input_callback = cb;
}