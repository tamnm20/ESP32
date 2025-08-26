#include<stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_attr.h" 
#include "tbutton.h"

#define T_DEBOUNCE_MS      30
#define T_DEBOUNCE     pdMS_TO_TICKS(T_DEBOUNCE_MS)
static volatile TickType_t s_last_isr_tick = 0;

button_callback_t button_callback = NULL;
timeoutButton_t timeoutButton_callback = NULL;
static uint64_t _start, _stop, _press_tick;

TimerHandle_t xTimers;
static void IRAM_ATTR button_input_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    uint32_t rtc = xTaskGetTickCountFromISR();
    // Debounce: bỏ qua nếu 2 lần ISR quá gần nhau
    if ((rtc - s_last_isr_tick) < T_DEBOUNCE) {
        return;
    }
    s_last_isr_tick = rtc;
    if(gpio_get_level(gpio_num) == 0){
        _start = rtc;
        xTimerStart(xTimers, 0);
    }
    else{
        _stop = rtc;
        xTimerStop(xTimers, 0);
        _press_tick = _stop - _start;
        button_callback(gpio_num, _press_tick);
    }
}

static void vTimerCallback( TimerHandle_t xTimer )
{
   uint32_t ulCount;
   /* Optionally do something if the pxTimer parameter is NULL. */
   configASSERT( xTimer );
   ulCount = ( uint32_t ) pvTimerGetTimerID( xTimer );
   if(ulCount == 0){
        timeoutButton_callback(BUTTON0);
      //gpio_toggle(GPIO_NUM_2);
   }
}

void button_init(gpio_num_t pinNum, gpio_mode_t pin_Mode, gpio_int_type_t type){
    esp_rom_gpio_pad_select_gpio(pinNum);
    gpio_set_direction(pinNum, pin_Mode);
    gpio_set_pull_mode(pinNum, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(pinNum, type);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pinNum, button_input_handler, (void*) pinNum);
    xTimers = xTimerCreate
        ( /* Just a text name, not used by the RTOS kernel. */
            "TimerForTimeOut",
            /* The timer period in ticks, must be greater than 0. */
            pdMS_TO_TICKS(5000),
            /* The timers will auto-reload themselves when they expire. */
            pdFALSE,
            /* The ID is used to store a count of the number of times the
                timer has expired, which is initialised to 0. */
            ( void * ) 0,
            /* Each timer calls the same callback when it expires. */
            vTimerCallback
        );
}

void button_set_callback(void *cb)
{
    button_callback = cb;
}

void button_set_timeout_callback(void *cb)
{
    timeoutButton_callback = cb;
}


