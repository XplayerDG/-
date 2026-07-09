#ifndef STEPPER_H
#define STEPPER_H

#include <stdint.h>

typedef enum {
    STEPPER_OK = 0,
    STEPPER_ERR_LEFT_LIMIT,
    STEPPER_ERR_RIGHT_LIMIT,
    STEPPER_ERR_HOME_LEFT_TIMEOUT,
    STEPPER_ERR_HOME_RIGHT_TIMEOUT
} StepperStatus;

void stepper_init(void);
void stepper_enable(uint8_t enable);
StepperStatus stepper_home(void);
StepperStatus stepper_move_to(int32_t target);
int32_t stepper_position(void);
int32_t stepper_left_limit_coord(void);
int32_t stepper_right_limit_coord(void);

#endif
