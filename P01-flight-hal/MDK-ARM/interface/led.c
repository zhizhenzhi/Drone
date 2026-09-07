#include "led.h"


void led_turn_on(led_t *led)
{
    HAL_GPIO_WritePin(led->port,led->pin,GPIO_PIN_RESET);
}

void led_turn_off(led_t *led)
{
    HAL_GPIO_WritePin(led->port,led->pin,GPIO_PIN_SET);
}

void led_toggle(led_t *led)
{
    HAL_GPIO_TogglePin(led->port,led->pin);
}
