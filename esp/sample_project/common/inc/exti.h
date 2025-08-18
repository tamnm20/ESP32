#ifndef EXTI_H
#define EXTI_H
#include "esp_err.h"
#include "hal/gpio_types.h"

typedef void (*input_callback_t)(int);
void EXTI_init(gpio_num_t pinNum, gpio_mode_t pin_Mode, gpio_int_type_t type);
void input_set_callback(void *cb);

#endif //EXTI_H