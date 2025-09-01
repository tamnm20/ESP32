#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "tgpio.h"
#include "esp_random.h"

#define BUFFER_SIZE 5

static SemaphoreHandle_t xMutex;
static SemaphoreHandle_t xfull;
static SemaphoreHandle_t xempty;

static inline uint32_t rand_mod(uint32_t max)
{
   // Kỹ thuật multiply-high tránh lệch
   uint32_t x = esp_random();
   return (uint64_t)x * max >> 32;
}

void producer(void *pvParameters)
{
   for (;;)
   {
      // Chờ còn slot trống
      xSemaphoreTake(xempty, portMAX_DELAY);

      // Vào vùng critical để truy cập buffer
      xSemaphoreTake(xMutex, portMAX_DELAY);
      // TODO: push item vào buffer thực (nếu có)
      xSemaphoreGive(xMutex);

      // Báo có thêm 1 item
      xSemaphoreGive(xfull);

      printf("Producer creates something\n");
      vTaskDelay(pdMS_TO_TICKS(rand_mod(5)*1000));
   }
}

void consumer(void *pvParameters)
{
   for (;;)
   {
      // Chờ có item
      xSemaphoreTake(xfull, portMAX_DELAY);

      // Vào vùng critical để truy cập buffer
      xSemaphoreTake(xMutex, portMAX_DELAY);
      // TODO: pop item khỏi buffer thực (nếu có)
      xSemaphoreGive(xMutex);

      // Trả lại 1 slot trống
      xSemaphoreGive(xempty);

      printf("Consumer takes something\n");
      vTaskDelay(pdMS_TO_TICKS((rand_mod(5)+3)*1000));
   }
}

void count(void *pvParameters)
{
   for (;;)
   {
      vTaskDelay(pdMS_TO_TICKS(10000));
      UBaseType_t takenSlots = uxSemaphoreGetCount(xfull);
      printf("Items in the buffer: %u/%d\n", (unsigned)takenSlots, BUFFER_SIZE);
      //printf("dice = %lu\n", rand_mod(6));
   }
}

void app_main(void)
{
   // Tạo mutex đúng loại
   xMutex = xSemaphoreCreateMutex();
   configASSERT(xMutex);

   // Counting semaphores
   xfull = xSemaphoreCreateCounting(BUFFER_SIZE, 0);            // 0 item
   xempty = xSemaphoreCreateCounting(BUFFER_SIZE, BUFFER_SIZE); // 5 slot trống
   configASSERT(xfull && xempty);

   pinMode(GPIO_NUM_2, GPIO_MODE_INPUT_OUTPUT);

   xTaskCreate(producer, "producer", 2048, NULL, 4, NULL);
   xTaskCreate(consumer, "consumer", 2048, NULL, 4, NULL);
   xTaskCreate(count, "count", 2048, NULL, 5, NULL);
}