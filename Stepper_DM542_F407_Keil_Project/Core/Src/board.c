#include "board.h"

static void gpio_set_mode(GPIO_TypeDef *port, uint32_t pin, uint32_t mode)
{
    port->MODER = (port->MODER & ~(3UL << (pin * 2U))) | (mode << (pin * 2U));
}

static void gpio_set_pupd(GPIO_TypeDef *port, uint32_t pin, uint32_t pupd)
{
    port->PUPDR = (port->PUPDR & ~(3UL << (pin * 2U))) | (pupd << (pin * 2U));
}

static void gpio_set_speed(GPIO_TypeDef *port, uint32_t pin, uint32_t speed)
{
    port->OSPEEDR = (port->OSPEEDR & ~(3UL << (pin * 2U))) | (speed << (pin * 2U));
}

void board_gpio_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIODEN;
    (void)RCC->AHB1ENR;

    gpio_set_mode(STEP_EN_PORT, STEP_EN_PIN, 1U);
    gpio_set_mode(STEP_DIR_PORT, STEP_DIR_PIN, 1U);
    gpio_set_mode(STEP_PUL_PORT, STEP_PUL_PIN, 1U);

    gpio_set_speed(STEP_EN_PORT, STEP_EN_PIN, 2U);
    gpio_set_speed(STEP_DIR_PORT, STEP_DIR_PIN, 2U);
    gpio_set_speed(STEP_PUL_PORT, STEP_PUL_PIN, 2U);

    gpio_set_pupd(STEP_EN_PORT, STEP_EN_PIN, 0U);
    gpio_set_pupd(STEP_DIR_PORT, STEP_DIR_PIN, 0U);
    gpio_set_pupd(STEP_PUL_PORT, STEP_PUL_PIN, 0U);

    gpio_set_mode(LIMIT_LEFT_PORT, LIMIT_LEFT_PIN, 0U);
    gpio_set_mode(LIMIT_RIGHT_PORT, LIMIT_RIGHT_PIN, 0U);
    gpio_set_pupd(LIMIT_LEFT_PORT, LIMIT_LEFT_PIN, 1U);
    gpio_set_pupd(LIMIT_RIGHT_PORT, LIMIT_RIGHT_PIN, 1U);
}
