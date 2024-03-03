#include <stdio.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#define BUTTON_INPUT_GPIO GPIO_NUM_13
#define LED_OUTPUT_GPIO GPIO_NUM_2

void static setup(void) {
    gpio_reset_pin(BUTTON_INPUT_GPIO);
    gpio_reset_pin(LED_OUTPUT_GPIO);

    gpio_set_direction(BUTTON_INPUT_GPIO, GPIO_MODE_INPUT);
    gpio_set_direction(LED_OUTPUT_GPIO, GPIO_MODE_OUTPUT);
}

static bool previous_state = 0;

void button_task(void *pvParameters) {
    while (1) {
        bool button_state = gpio_get_level(BUTTON_INPUT_GPIO);
        gpio_set_level(LED_OUTPUT_GPIO, !button_state);
        if (button_state != previous_state) {
            previous_state = button_state;
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

void app_main(void) {
    setup();
    xTaskCreate(&button_task, "button_task", 2048, NULL, 5, NULL);
}
