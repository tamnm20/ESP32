#include "ttimer.h"

static volatile uint32_t delay_cnt = 0;

static void Timer_Callback(void *arg);

static void Timer_Callback(void *arg){
    printf(" timer is running!! \n");
    //delay_cnt++;
}

void timer_init(void){
        /*----timer----*/
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &Timer_Callback,
        .name = "periodic"
    };
    esp_timer_handle_t periodic_timer;

    esp_timer_create(&periodic_timer_args, &periodic_timer);
    esp_timer_start_periodic(periodic_timer, 1000000);
}

void delay_ms(uint32_t ms)
{
	uint32_t current_cnt = delay_cnt;
	while((uint32_t)(delay_cnt-current_cnt) < ms);
}