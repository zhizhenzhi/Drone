#ifndef __LED_H__
#define __LED_H__

#include "main.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
} led_t;

void led_turn_on(led_t *led);
void led_turn_off(led_t *led);
void led_toggle(led_t *led);
#endif
