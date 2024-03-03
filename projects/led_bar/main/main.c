#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/projdefs.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_13

static const gpio_num_t ledPins[10] = {47, 48, 21, 38, 39, 40, 41, 42, 2, 1};
static bool powerState = false;
static bool prevButtonState = true;

void buttonToggleTask(void *pvParameters){
    while(1){
        bool currButtonState = gpio_get_level(BUTTON_PIN);
        // Capture falling edge (low-active)
        if(!currButtonState && prevButtonState){
            powerState = !powerState;
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        prevButtonState = currButtonState;
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void ledDisplayTask(void *pvParameters){
    while(1){
        for(int idx = 0; idx < 10; idx++){
            while(!powerState){
                vTaskDelay(pdMS_TO_TICKS(50));
            }
            gpio_set_level(ledPins[idx], 1);
            vTaskDelay(pdMS_TO_TICKS(100));
            gpio_set_level(ledPins[idx], 0);
        }
    }
}

void setup(void){
    // Setup Button as Input
    // gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);

    // Setup Led Pins as Outputs
    for(int idx = 0; idx < 10; idx++){
        gpio_reset_pin(ledPins[idx]);
        gpio_set_direction(ledPins[idx], GPIO_MODE_OUTPUT);
    }
}





void app_main(void)
{
    setup();
    xTaskCreate(&buttonToggleTask, "button toggle task", 2048, NULL, 0, NULL);
    xTaskCreate(&ledDisplayTask, "led display task", 2048, NULL, 0, NULL);
}
