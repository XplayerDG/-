#include "board.h"
#include "usart.h"

static void gpio_config_usart1_pa9_pa10(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    (void)RCC->AHB1ENR;

    GPIOA->MODER = (GPIOA->MODER & ~((3UL << 18U) | (3UL << 20U))) |
                   (2UL << 18U) | (2UL << 20U);
    GPIOA->OSPEEDR |= (2UL << 18U) | (2UL << 20U);
    GPIOA->PUPDR = (GPIOA->PUPDR & ~((3UL << 18U) | (3UL << 20U))) |
                   (1UL << 18U) | (1UL << 20U);
    GPIOA->AFR[1] = (GPIOA->AFR[1] & ~((0xFUL << 4U) | (0xFUL << 8U))) |
                    (7UL << 4U) | (7UL << 8U);
}

void usart1_init(uint32_t baudrate)
{
    gpio_config_usart1_pa9_pa10();

    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    (void)RCC->APB2ENR;

    USART1->CR1 = 0;
    USART1->BRR = (SystemCoreClock + (baudrate / 2UL)) / baudrate;
    USART1->CR2 = 0;
    USART1->CR3 = 0;
    USART1->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void usart1_putc(char ch)
{
    while ((USART1->SR & USART_SR_TXE) == 0U) {
    }
    USART1->DR = (uint32_t)(uint8_t)ch;
}

void usart1_puts(const char *s)
{
    while (*s != '\0') {
        usart1_putc(*s++);
    }
}

int usart1_getline(char *buf, uint32_t max_len)
{
    static uint32_t index;

    while ((USART1->SR & USART_SR_RXNE) != 0U) {
        char ch = (char)(USART1->DR & 0xFFU);

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            buf[index] = '\0';
            index = 0;
            return 1;
        }

        if (index < (max_len - 1U)) {
            buf[index++] = ch;
        } else {
            index = 0;
            buf[0] = '\0';
            return -1;
        }
    }

    return 0;
}
