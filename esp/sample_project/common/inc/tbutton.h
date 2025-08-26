#ifndef TBUTTON_H
#define TBUTTON_H

#include <stdint.h>
#include "hal/gpio_types.h"
#define BUTTON0 GPIO_NUM_0
typedef void (*button_callback_t)(int, uint64_t);
typedef void (*timeoutButton_t)(int);
void button_init(gpio_num_t pinNum, gpio_mode_t pin_Mode, gpio_int_type_t type);
void button_set_callback(void *cb);
void button_set_timeout_callback(void *cb);

#endif //TBUTTON_H