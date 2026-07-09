#include "board.h"
#include "stepper.h"

static int32_t g_position;
static int32_t g_left_limit_coord;
static int32_t g_right_limit_coord;

static void gpio_write(GPIO_TypeDef *port, uint32_t pin, uint8_t level)
{
    if (level != 0U) {
        port->BSRR = 1UL << pin;
    } else {
        port->BSRR = 1UL << (pin + 16U);
    }
}

static uint8_t gpio_read(GPIO_TypeDef *port, uint32_t pin)
{
    return (port->IDR & (1UL << pin)) != 0U;
}

static uint8_t left_limit_pressed(void)
{
#if LIMIT_ACTIVE_LOW
    return gpio_read(LIMIT_LEFT_PORT, LIMIT_LEFT_PIN) == 0U;
#else
    return gpio_read(LIMIT_LEFT_PORT, LIMIT_LEFT_PIN) != 0U;
#endif
}

static uint8_t right_limit_pressed(void)
{
#if LIMIT_ACTIVE_LOW
    return gpio_read(LIMIT_RIGHT_PORT, LIMIT_RIGHT_PIN) == 0U;
#else
    return gpio_read(LIMIT_RIGHT_PORT, LIMIT_RIGHT_PIN) != 0U;
#endif
}

static void set_direction_right(uint8_t right)
{
    uint8_t level = (right != 0U) ? DIR_RIGHT_LEVEL : (uint8_t)!DIR_RIGHT_LEVEL;
    gpio_write(STEP_DIR_PORT, STEP_DIR_PIN, level);
    delay_us(20U);
}

static void one_step(uint32_t interval_us)
{
    gpio_write(STEP_PUL_PORT, STEP_PUL_PIN, 1U);
    delay_us(STEP_PULSE_HIGH_US);
    gpio_write(STEP_PUL_PORT, STEP_PUL_PIN, 0U);

    if (interval_us > STEP_PULSE_HIGH_US) {
        delay_us(interval_us - STEP_PULSE_HIGH_US);
    }
}

static StepperStatus move_steps(uint8_t right, int32_t steps, uint32_t interval_us)
{
    int32_t i;

    set_direction_right(right);

    for (i = 0; i < steps; i++) {
        if (right == 0U && left_limit_pressed()) {
            return STEPPER_ERR_LEFT_LIMIT;
        }
        if (right != 0U && right_limit_pressed()) {
            return STEPPER_ERR_RIGHT_LIMIT;
        }

        one_step(interval_us);
        g_position += (right != 0U) ? 1L : -1L;
    }

    return STEPPER_OK;
}

void stepper_init(void)
{
    board_gpio_init();
    stepper_enable(0U);
    gpio_write(STEP_DIR_PORT, STEP_DIR_PIN, DIR_RIGHT_LEVEL);
    gpio_write(STEP_PUL_PORT, STEP_PUL_PIN, 0U);
}

void stepper_enable(uint8_t enable)
{
#if MOTOR_ENABLE_ACTIVE_LOW
    gpio_write(STEP_EN_PORT, STEP_EN_PIN, enable ? 0U : 1U);
#else
    gpio_write(STEP_EN_PORT, STEP_EN_PIN, enable ? 1U : 0U);
#endif
}

StepperStatus stepper_home(void)
{
    int32_t travel_steps = 0;
    int32_t center_abs;
    StepperStatus status;

    stepper_enable(1U);
    delay_ms(100U);

    set_direction_right(0U);
    while (!left_limit_pressed()) {
        if (travel_steps++ >= HOMING_MAX_STEPS) {
            return STEPPER_ERR_HOME_LEFT_TIMEOUT;
        }
        one_step(HOMING_INTERVAL_US);
        g_position--;
    }
    g_left_limit_coord = g_position;

    status = move_steps(1U, LIMIT_BACKOFF_STEPS, HOMING_INTERVAL_US);
    if (status != STEPPER_OK) {
        return status;
    }

    travel_steps = 0;
    set_direction_right(1U);
    while (!right_limit_pressed()) {
        if (travel_steps++ >= HOMING_MAX_STEPS) {
            return STEPPER_ERR_HOME_RIGHT_TIMEOUT;
        }
        one_step(HOMING_INTERVAL_US);
        g_position++;
    }
    g_right_limit_coord = g_position;

    status = move_steps(0U, LIMIT_BACKOFF_STEPS, HOMING_INTERVAL_US);
    if (status != STEPPER_OK) {
        return status;
    }

    center_abs = g_left_limit_coord + ((g_right_limit_coord - g_left_limit_coord) / 2L);
    status = stepper_move_to(center_abs);
    if (status != STEPPER_OK) {
        return status;
    }

    g_position = 0;
    g_left_limit_coord -= center_abs;
    g_right_limit_coord -= center_abs;

    return STEPPER_OK;
}

StepperStatus stepper_move_to(int32_t target)
{
    int32_t delta = target - g_position;

    if (delta > 0L) {
        return move_steps(1U, delta, STEP_INTERVAL_US);
    }
    if (delta < 0L) {
        return move_steps(0U, -delta, STEP_INTERVAL_US);
    }

    return STEPPER_OK;
}

int32_t stepper_position(void)
{
    return g_position;
}

int32_t stepper_left_limit_coord(void)
{
    return g_left_limit_coord;
}

int32_t stepper_right_limit_coord(void)
{
    return g_right_limit_coord;
}
