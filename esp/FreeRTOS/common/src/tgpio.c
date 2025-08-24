#include<stdio.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "tgpio.h"

void pinMode(gpio_num_t pinNum, gpio_mode_t pin_Mode){   
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << pinNum);          /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
    io_conf.mode = pin_Mode;               /*!< GPIO mode: set input/output mode                     */
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;       /*!< GPIO pull-up                                         */
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;   /*!< GPIO pull-down                                       */
    io_conf.intr_type = GPIO_INTR_DISABLE;      /*!< GPIO interrupt type                                  */
    gpio_config(&io_conf);
}

void gpio_toggle(gpio_num_t pinNum){
    //static int toggle_state = 0;  // Dùng biến static để lưu trạng thái
    int toggle_state = gpio_get_level(pinNum);
    toggle_state = !toggle_state;
    gpio_set_level(pinNum, toggle_state);
    //printf("GPIO %d set to: %d\n", pinNum, toggle_state);
}


