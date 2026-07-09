#ifndef BOARD_H
#define BOARD_H

#include "stm32f407_min.h"

#define STEP_EN_PORT         GPIOA
#define STEP_EN_PIN          13U
#define STEP_DIR_PORT        GPIOA
#define STEP_DIR_PIN         14U
#define STEP_PUL_PORT        GPIOD
#define STEP_PUL_PIN         15U

#define LIMIT_RIGHT_PORT     GPIOD
#define LIMIT_RIGHT_PIN      13U
#define LIMIT_LEFT_PORT      GPIOD
#define LIMIT_LEFT_PIN       14U

#define MOTOR_ENABLE_ACTIVE_LOW   0
#define LIMIT_ACTIVE_LOW          1

#define DIR_RIGHT_LEVEL           1
#define STEP_PULSE_HIGH_US        5U
#define STEP_INTERVAL_US          800U
#define HOMING_INTERVAL_US        1200U
#define LIMIT_BACKOFF_STEPS       200L
#define HOMING_MAX_STEPS          200000L

void board_gpio_init(void);

#endif
